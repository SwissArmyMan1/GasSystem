#include "plc.h"

PLC::PLC(QObject *parent) : QObject(parent)
{
    /* Initialise attributes with start values */
    this->address.setHost("192.168.1.1");
    this->address.setPort(503);

    this->serverID = 1;
    this->msecsStateUpdatePeriod = 500;
    this->msecsStateUpdateStartOffset = 500;

    /* Init modbus device interface */
    modbusDevice = new QModbusTcpClient(this);

    /* CONNECTIONS */
    connect(modbusDevice, &QModbusClient::errorOccurred,
            [this](QModbusDevice::Error) {
                emit errorOccured(modbusDevice->errorString());
            });


    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerHandler()));

    if (!modbusDevice) {
        emit errorOccured(tr("Could not create Modbus client."));
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &PLC::onModbusDeviceStateChanged);
    }
}

PLC::~PLC()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void PLC::setAddress(QUrl addr)
{
    this->address = addr;
}

void PLC::setStateUpdatePeriod(uint msecsPeriod)
{
    this->msecsStateUpdatePeriod = msecsPeriod;
}

void PLC::setStateUpdateStartOffset(uint msecsOffset)
{
    this->msecsStateUpdateStartOffset = msecsOffset;
}

void PLC::setServerID(uint id)
{
    this->serverID = id;
}

PLC_State PLC::getCurrentState()
{
    return this->currentState;
}

void PLC::PLCconnect()
{
    qDebug() << "PLC::PLCconnect >> " << this->address;
    if (!modbusDevice)
    {
        qDebug() << "PLC::PLCconnect >> " << "No modbus device created";
        return;
    }

    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, this->address.port());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, this->address.host());

        modbusDevice->setTimeout(500);
        modbusDevice->setNumberOfRetries(3);
        modbusDevice->connectDevice();

    } else {
        modbusDevice->disconnectDevice();
    }
}

void PLC::PLCdisconnect()
{
    qDebug() << "PLC::PLCdisconnect from" << this->address;
    modbusDevice->disconnectDevice();
}

void PLC::updateTimerHandler()
{
    readModbusRegisters(0, sizeof(PLC_State)/2);    // read full state struct (one modbus register - 2 bytes)
}

void PLC::onModbusDeviceStateChanged(int state)
{

    if(state == QModbusDevice::ConnectedState){
        emit connectionStateChanged(tr("Connected!"));
        connectionRetryCounter = 0;

        //writeModbusRegister(0, currRegisterValue);

        updateTimer.start(msecsStateUpdateStartOffset);
        updateTimer.setInterval(msecsStateUpdatePeriod);

    } else if(state == QModbusDevice::UnconnectedState) {

        updateTimer.stop();

        qDebug() << modbusDevice->errorString();

        /* Connection externally closed */
        if(modbusDevice->error() == 3) {

            if(connectionRetryCounter < 5) {
                this->PLCconnect();
                qDebug() << connectionRetryCounter;
                connectionRetryCounter++;
            }
            emit connectionStateChanged(tr("Connect failed: ") + modbusDevice->errorString());
        }
        /* Connection closed forced (without errors) */
        else
        {
            emit connectionStateChanged(tr("Disconnected"));
        }
    } else {
        emit connectionStateChanged(tr("Connecting..."));
    }
}

void PLC::readModbusRegisters(uint startAddress, uint nBytes)
{
    if (!modbusDevice)
            return;

    if (auto *reply = modbusDevice->sendReadRequest(QModbusDataUnit(QModbusDataUnit::InputRegisters, startAddress, nBytes), this->serverID)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &PLC::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        emit errorOccured(tr("Read error: ") + modbusDevice->errorString());
    }
}

void PLC::writeModbusRegister(int startAddress, quint16 value)
{
    if (!modbusDevice)
        return;

    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, startAddress, 1);
    dataUnit.setValue(0, value);

    // STOP state read
    updateTimer.stop();

    //sending and waiting for right reply (without errors)
    if (auto *reply = modbusDevice->sendWriteRequest(dataUnit, this->serverID)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
//                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
//                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
//                        5000);

                } else if (reply->error() != QModbusDevice::NoError) {
//                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
//                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);

                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }

    } else {
//        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);

    }

    // RESTART state read
    updateTimer.start();
}

void PLC::writeModbusRegisters(int startAddress, QVector<quint16> data)
{
    if (!modbusDevice)
        return;

    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, startAddress, data);

    // STOP state read
    updateTimer.stop();

    //sending and waiting for right reply (without errors)
    if (auto *reply = modbusDevice->sendWriteRequest(dataUnit, this->serverID)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }

    }

    // RESTART state read
    updateTimer.start();
}

void PLC::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        inputPLCdata.clear();
        for (uint i = 0; i < unit.valueCount(); i++) {
            quint16 value = unit.value(i);
            inputPLCdata.append((char*)&value,2);
        }

        QDataStream ds(&inputPLCdata, QIODevice::ReadOnly);

        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //        ds >> currentState.DI_00;     //--> single writes from dataStream to struct
        //        ds >> currentState.DI_01;     //--> field after field
        //        ds >> currentState.DO_00;
        //        ds >> currentState.DO_01;
        //        ds >> currentState.O2_00;
        //        ds >> currentState.O2_01;
        //        ds >> currentState.O2_02;
        //        ds >> currentState.O2_03;
        //        ds >> currentState.O2_04;
        ds.readRawData(reinterpret_cast<char*>(&currentState),
                       sizeof(PLC_State));                      // all in one write


//        qDebug() << "PLC::readReady >> " << currentState.DI_00 << currentState.DI_01 << currentState.DO_00 << currentState.DO_01
//                 << currentState.O2_00 << currentState.O2_01 << currentState.O2_02 << currentState.O2_03 << currentState.O2_04;
        emit stateUpdated(currentState);

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        emit errorOccured(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        emit errorOccured(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}
