#ifndef PLC_H
#define PLC_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QModbusClient>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QUrl>


/* ========================================================================== */

typedef struct PLC_State
{
    char    PLC_date[12];
    quint32 PLC_version;

    quint8  DI_00,
            DI_01,
            DI_02,
            DI_03,
            DI_04,
            DI_05,

            DO_00,
            DO_01,
            DO_02,
            DO_03,
            DO_04,
            DO_05,

            AI_00,
            AI_01,
            AI_02,
            AI_03,
            AI_04,
            AI_05,

            AO_00,
            AO_01,
            AO_02,
            AO_03,
            AO_04,
            AO_05;

    quint16 system_state;
    quint16 internal_error_code;

    quint16 DI_State_00_01,
            DI_State_02_03,
            DI_State_04_05,

            DO_State_00_01,
            DO_State_02_03,
            DO_State_04_05;


    float   InvertorM1_freq,
            O2_00,      // O2 #1
            O2_01,      // O2 #2
            O2_02,      // O2 #3
            O2_03,      // O2 #4
            O2_04,      // O2 #5

            P_camera,
            Flow_speed,
            PDiff_filter_rough,
            PDiff_filter_fine,
            PDiff_filter_cleaner,
            Cons_high,
            Cons_low,
            T_platform,
            T_camera,

            O2_camera_sum,
            PDiff_filter_rough_display,
            PDiff_filter_fine_display,
            PDiff_filter_cleaner_display,
            Flow_speed_platform,
            T_regulation,
            O2_00_WorkLevel,
            O2_01_WorkLevel,
            O2_02_WorkLevel,
            O2_03_WorkLevel,
            O2_04_WorkLevel,
            O2_00_Gisteresis,
            O2_01_Gisteresis,
            O2_02_Gisteresis,
            O2_03_Gisteresis,
            O2_04_Gisteresis,
            O2_Cleaner_Gisteresis,
            O2_Critical,
            O2_Critical_Gisteresis,
            P_work,
            P_Work_Gisteresis,
            // ----- new
            InvertorM2_freq,
            PDiff_Prs01,
            PDiff_Prs02,
            PDiff_Prs03,
            PDiff_Prs04,
            NA_01,
            NA_02,
            NA_03,
            NA_04,
            NA_05,
            NA_06;

    quint16 PRS_state;


} PLC_State;

enum E_PLC_DI_BITS {
    PLC_DI_CONSUMPTION_HIGH,
    PLC_DI_CONSUMPTION_LOW,
    PLC_DI_RELAY_AIR,
    PLC_DI_RELAY_GAS,

    PLC_DI_POWDER_01_FEEDER,
    PLC_DI_POWDER_01_CAMERA,
    PLC_DI_POWDER_01_FILTER,
    PLC_DI_POWDER_02_FEEDER,

    PLC_DI_POWDER_02_CAMERA,
    PLC_DI_POWDER_02_FILTER,
    PLC_DI_POWDER_03_FEEDER,
    PLC_DI_POWDER_03_CAMERA,
    PLC_DI_POWDER_03_FILTER,
    PLC_DI_POWDER_CLEANER,
    PLC_DI_POWDER_FILTER_ROUGH,


    PLC_DI_SIZE
};
enum E_PLC_DO_BITS {
    PLC_DO_VALVE_01,
    PLC_DO_VALVE_02,
    PLC_DO_VALVE_03,
    PLC_DO_VALVE_04,
    PLC_DO_VALVE_05_08,
    PLC_DO_VALVE_06,
    PLC_DO_VALVE_07,
    PLC_DO_VALVE_09,
    PLC_DO_VALVE_10,
    PLC_DO_VALVE_11,

    PLC_DO_INVERTOR1_FW = 11,

    PLC_DO_VALVE_12 = 12,
    PLC_DO_VALVE_13 = 13,
    PLC_DO_VALVE_14 = 14,
    PLC_DO_VALVE_15 = 15,
    PLC_DO_VALVE_16 = 16,
    PLC_DO_VALVE_17 = 17,
    PLC_DO_VALVE_18 = 18,
    PLC_DO_VALVE_19 = 19,
    PLC_DO_VALVE_20 = 20,
    PLC_DO_VALVE_21 = 21,

    PLC_DO_INVERTOR2_FW = 23,

    PLC_DO_SIZE
};
enum E_PLC_CONTROL_BITS {
    PLC_CONTROL_STARTING,
    PLC_CONTROL_CLEANER_ON,
    PLC_CONTROL_CLEANER_OFF,
    PLC_CONTROL_ENDING,
    PLC_CONTROL_PAUSE,

    PLC_CONTROL_SIZE
};
enum E_PLC_PROGRAM_ERROR {
    PLC_PROGRAM_ERROR_NONE,
    PLC_PROGRAM_ERROR_OXYGEN,
    PLC_PROGRAM_ERROR_MAX_PRESSURE,
    PLC_PROGRAM_ERROR_MIN_PRESSURE,
    PLC_PROGRAM_ERROR_NO_AIR,
    PLC_PROGRAM_ERROR_NO_GAS,

    PLC_PROGRAM_ERROR_SIZE

};

/* ========================================================================== */

class PLC : public QObject
{
    Q_OBJECT
public:
    explicit    PLC(QObject *parent = nullptr);
                ~PLC();
    void        setAddress(QUrl addr);
    void        setStateUpdatePeriod(uint msecsPeriod);
    void        setStateUpdateStartOffset(uint msecsOffset);
    void        setServerID(uint id);
    PLC_State   getCurrentState();
    void        writeModbusRegister(int startAddress, quint16 value);
    void        writeModbusRegisters(int startAddress, QVector<quint16> data);

public slots:
    void        PLCconnect();
    void        PLCdisconnect();

private:
    PLC_State   currentState;
    QUrl        address;
    uint        serverID;
    uint        msecsStateUpdatePeriod;
    uint        msecsStateUpdateStartOffset;

    QTimer          updateTimer;
    QModbusClient   *modbusDevice;
    QModbusReply    *lastRequest;
    quint16     currRegisterValue;
    QByteArray  inputPLCdata;
    quint16     connectionRetryCounter;

private slots:
    void        updateTimerHandler();
    void        onModbusDeviceStateChanged(int state);
    void        readModbusRegisters(uint startAddress, uint nBytes);
    void        readReady();


signals:
    void    connectionStateChanged(QString message);
    void    errorOccured(QString message);
    void    stateUpdated(PLC_State state);

};

#endif // PLC_H
