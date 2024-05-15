#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rotatedlabel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QMainWindow{background: 'white';}");

/*    // set default state for scene (waiting state)
    for(uint i=1; i<21; ++i)
    {
        setValveOn(i,true);
    }
*/
    settings = new QSettings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

    QString sceneVariantString = settings->value("SceneVariant","standard").toString();

    Plc = new PLC();
    QUrl PlcUrl;
    PlcUrl.setHost(settings->value("IP","192.168.1.11").toString());
    PlcUrl.setPort(settings->value("Port","503").toInt());
    Plc->setAddress(PlcUrl);
    QThread::msleep(1000);

//    plcController = new PlcController(this);

    connect(Plc, SIGNAL(stateUpdated(PLC_State)), this, SLOT(showUpdatedState(PLC_State))); //доб
    //connect(&logTimer, SIGNAL(timeout()), this, SLOT(logTimerHandler()));//доб
    connect(Plc, SIGNAL(connectionStateChanged(QString)), this, SLOT(changeStatusActive(QString)));//доб

    QTimer::singleShot(500, this, SLOT(connectPLC()));
}

MainWindow::~MainWindow()
{
    saveSettings();
    this->disconnectPLC();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("UIParameters");

    //....................134
}


void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("UIParameters");

    //....................158
}

void MainWindow::showUpdatedState(PLC_State state)
{

    // ------------------------ Update status UI widgets ------------
//    ui->LE_AFlow_sensor_1->setText(QString::number(static_cast<double>(state.Cons_high), 'f', 1));
//    ui->LE_AFlow_sensor_2->setText(QString::number(static_cast<double>(state.Cons_low), 'f', 1));

//    ui->LE_AFlow_sensor_3->setText(QString::number(static_cast<double>(state.PDiff_Prs04), 'f', 1));

    ui->LE_O2_box->setText(QString::number(static_cast<double>(state.O2_00), 'f', 2));
    //ui->LE_O2_camera_low->setText(QString::number(static_cast<double>(state.O2_00), 'f', 2));
    ui->LE_O2_room->setText(QString::number(static_cast<double>(state.O2_01), 'f', 2));
    //ui->LE_O2_camera_low->setEnabled(false);
    //ui->LE_O2_cleaner->setText(QString::number(static_cast<double>(state.O2_02), 'f', 2));
    ui->LE_O2_filter_1->setText(QString::number(static_cast<double>(state.O2_02), 'f', 2));
    ui->LE_O2_filter_2->setText(QString::number(static_cast<double>(state.O2_03), 'f', 2));

    ui->LE_Pdiff_1->setText(QString::number(static_cast<double>(state.PDiff_filter_rough), 'f', 1));
    ui->LE_Pdiff_2->setText(QString::number(static_cast<double>(state.PDiff_filter_fine), 'f', 1));
    ui->LE_Pdiff_3->setText(QString::number(static_cast<double>(state.PDiff_filter_cleaner), 'f', 1));

    ui->LE_P_box->setText(QString::number(static_cast<int>(state.P_camera)));
    ui->LE_T_box->setText(QString::number(static_cast<double>(state.T_camera),'f', 1));
    ui->LE_T_platform->setText(QString::number(static_cast<double>(state.T_platform),'f', 1));

    ui->LE_Flow_speed_platform->setText(QString::number(static_cast<double>(state.Flow_speed_platform),'f', 1));

    //ui->LE_Flow_speed->setText(QString::number(static_cast<double>(state.Flow_speed),'f', 1));
    ui->LE_speedFan->setText(QString::number(static_cast<int>(state.InvertorM1_freq)));
    //ui->LE_InvertorM2_freq->setText(QString::number(static_cast<int>(state.InvertorM2_freq)));


    //scene->setTurbineSpeedM1(-state.InvertorM1_freq);   // "-" - for CCW rotation on scene
    //scene->setTurbineSpeedM2(-state.InvertorM2_freq);


//    if((-state.InvertorM1_freq)>0)
//    {
//        m_dps = (-state.InvertorM1_freq);
//        ui->Fan->startRotate();
//    }

//    scene->setFilterCleanerValue(state.PDiff_filter_cleaner_display);
//    scene->setFilterRoughValue(state.PDiff_filter_rough_display);
//    scene->setFilterFineValue(state.PDiff_filter_fine_display);

    setValveOn(1,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_01));
    setValveOn(2,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_02));
    setValveOn(3,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_03));
    setValveOn(4,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_04));
    setValveOn(5,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_05_08));
    setValveOn(6,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_06));
    setValveOn(7,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_07));
    setValveOn(9,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_09));
    setValveOn(10,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_10));
    setValveOn(11,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_11));
    setValveOn(12,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_12));
    setValveOn(13,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_13));
    setValveOn(14,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_14));
    setValveOn(15,IntBits(state.DO_State_00_01).test(PLC_DO_VALVE_15));
    setValveOn(16,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_16 - 16));
    setValveOn(17,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_17 - 16));
    setValveOn(18,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_18 - 16));
    setValveOn(19,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_19 - 16));


//    scene->setValveOn(16,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_16 - 16));
//    scene->setValveOn(17,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_17 - 16));
//    scene->setValveOn(18,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_18 - 16));
//    scene->setValveOn(19,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_19 - 16));
//    scene->setValveOn(20,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_20 - 16));
//    scene->setValveOn(21,IntBits(state.DO_State_02_03).test(PLC_DO_VALVE_21 - 16));
    //TODO : add second register for valves indication


    setRelayAirOn(IntBits(state.DI_State_00_01).test(PLC_DI_RELAY_AIR));
    setRelayGasOn(IntBits(state.DI_State_00_01).test(PLC_DI_RELAY_GAS));


    PowderBox(-IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_01_CAMERA));
    PowderDrop1(-IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_02_CAMERA));
    PowderDrop2(-IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_03_CAMERA));
    PowderFill(-IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_01_FEEDER));
    PowderFilter1(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_01_FILTER));
    PowderFilter2(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_02_FILTER));
//    PowderScrew(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_03_FEEDER));
//    scene->setPowderLossPrs03Full(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_03_FILTER));
//    scene->setPowderLossPrs04Full(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_FILTER_ROUGH));

//    scene->setPowderLossCleanerFull(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_CLEANER));
//    scene->setPowderLossFilterFull(IntBits(state.DI_State_00_01).test(PLC_DI_POWDER_FILTER_ROUGH));


    ui->LE_O2_box->setToolTip(" Рабочее:" + QString::number(state.O2_00_WorkLevel) +
                                     " Гистерезис:" + QString::number(state.O2_00_Gisteresis) +
                                     " Критическое:" + QString::number(state.O2_Critical) +
                                     " Крит. гистерезис:" + QString::number(state.O2_Critical_Gisteresis));
    ui->LE_O2_room->setToolTip(" Рабочее:" + QString::number(state.O2_01_WorkLevel) +
                                     " Гистерезис:" + QString::number(state.O2_01_Gisteresis) +
                                     " Критическое:" + QString::number(state.O2_Critical) +
                                     " Крит. гистерезис:" + QString::number(state.O2_Critical_Gisteresis));
    ui->LE_O2_filter_1->setToolTip(" Рабочее:" + QString::number(state.O2_02_WorkLevel) +
                                     " Гистерезис:" + QString::number(state.O2_02_Gisteresis) +
                                     " Критическое:" + QString::number(state.O2_Critical) +
                                     " Крит. гистерезис:" + QString::number(state.O2_Critical_Gisteresis));
    ui->LE_O2_filter_2->setToolTip(" Рабочее:" + QString::number(state.O2_03_WorkLevel) +
                                     " Гистерезис:" + QString::number(state.O2_03_Gisteresis) +
                                     " Критическое:" + QString::number(state.O2_Critical) +
                                     " Крит. гистерезис:" + QString::number(state.O2_Critical_Gisteresis));
//    ui->LE_O2_room->setToolTip(" Рабочее:" + QString::number(state.O2_04_WorkLevel) +
//                                     " Гистерезис:" + QString::number(state.O2_04_Gisteresis) +
//                                     " Критическое:" + QString::number(state.O2_Critical) +
//                                     " Крит. гистерезис:" + QString::number(state.O2_Critical_Gisteresis));
    ui->LE_P_box->setToolTip(" Рабочее:" + QString::number(state.P_work) +
                                " Гистерезис:" + QString::number(state.P_Work_Gisteresis));



//    if(static_cast<double>(state.T_regulation) > 0)
//    {
//        setPlatformHeating(SCENE_HEATING_ON);
//    }
//    else
//    {
//        if(static_cast<double>(state.T_platform) > this->safety_platform_temp)
//            scene->setPlatformHeating(SCENE_HEATING_OFF_HOT);
//        else
//            scene->setPlatformHeating(SCENE_HEATING_OFF_COLD);
//    }


    // ------------------------ System status show ------------------
    QString statusStr = "";
    switch(state.system_state)
    {
    case 0:
        statusStr = "Ожидание";
        ui->PB_Starting->setEnabled(true);
        ui->PB_Stop->setEnabled(false);
        ui->PB_Pause->setEnabled(false);
        ui->PB_VacuumCleaner_on->setEnabled(true);
        ui->PB_VacuumCleaner_off->setEnabled(true);
//        ui->tableWidget->setEnabled(true);
        ui->L_SystemState->setStyleSheet("background-color: rgb(255, 255, 255);");  /*white*/
        break;
    case 10:
        statusStr = "Откачка";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
//        ui->Fan_2->startRotate();
        break;
    case 11:
        statusStr = "Продувка (ожидание уровня кислорода в магистралях ниже критического)";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
//        ui->Fan_2->stopRotate();
//        ui->Fan->startRotate();
        break;
    case 12:
        statusStr = "Продувка камеры (ожидание уровня кислорода в камере ниже критического)";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
        break;
    case 13:
        statusStr = "Откачка системы (ожидание уровня кислорода в магистралях и камере ниже рабочего)";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
        break;
    case 14:
        statusStr = "Откачка системы (ожидание давления в камере больше рабочего значения)";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
        break;
    case 15:
        statusStr = "Откачка системы (ожидание сброса давления в камере ниже рабочего значения)";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
        break;
    case 20:
        statusStr = "В работе";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(true);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(170, 250, 173);");  /*green*/
        break;
    case 40:
        statusStr = "Окончание работы";
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(false);
        ui->PB_Pause->setEnabled(false);
        ui->PB_VacuumCleaner_on->setEnabled(true);
        ui->PB_VacuumCleaner_off->setEnabled(true);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
//        ui->Fan_2->stopRotate();
//        ui->Fan->stopRotate();
        break;
    case 50:
        statusStr = "Пауза";
        ui->PB_Starting->setEnabled(true);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(false);
        ui->PB_VacuumCleaner_on->setEnabled(true);
        ui->PB_VacuumCleaner_off->setEnabled(true);
//        ui->tableWidget->setEnabled(false);
        ui->L_SystemState->setStyleSheet("background-color: rgb(228, 232, 165);");  /*yellow*/
//        ui->Fan_2->stopRotate();
//        ui->Fan->stopRotate();
        break;
    case 60:
    {
        statusStr = "Аварийный режим - ";

        switch (state.internal_error_code)
        {
        case PLC_PROGRAM_ERROR_OXYGEN:
            statusStr += "Недопустимое содержание кислорода в камере";
            break;
        case PLC_PROGRAM_ERROR_MAX_PRESSURE:
            statusStr += "Давление в камере выше допустимого";
            break;
        case PLC_PROGRAM_ERROR_MIN_PRESSURE:
            statusStr += "Давление в камере ниже допустимого";
            break;
        case PLC_PROGRAM_ERROR_NO_AIR:
            statusStr += "Отсутствует подача воздуха";
            break;
        case PLC_PROGRAM_ERROR_NO_GAS:
            statusStr += "Отсутствует подача защитного газа";
            break;
        case PLC_PROGRAM_ERROR_NONE:
            break;
        default:
            break;
        }

        ui->L_SystemState->setStyleSheet("background-color: rgb(227, 113, 113);");  /*red*/

        ui->PB_Starting->setEnabled(true);
        ui->PB_Stop->setEnabled(true);
        ui->PB_Pause->setEnabled(false);
        ui->PB_VacuumCleaner_on->setEnabled(true);
        ui->PB_VacuumCleaner_off->setEnabled(true);


//        ui->Fan_2->stopRotate();
//        ui->Fan->stopRotate();
//        ui->tableWidget->setEnabled(false);
        break;
    }
    default:
        statusStr = "N/A";
        ui->L_SystemState->setStyleSheet("background-color: rgb(255, 255, 255);");
        break;

    }
    ui->L_SystemState->setText("Статус системы: " + statusStr);


    // ------------------------ Logging -----------------------------
//    if(ReadyToLog == true)
//    {
//        QString logString = "";
//        logString += " System_State=" + QString::number(state.system_state);
//        logString += " Error_Code=" + QString::number(state.internal_error_code);
//        logString += " DI_00_01=" + QString::number(state.DI_State_00_01);
//        logString += " DI_02_03=" + QString::number(state.DI_State_02_03);
//        logString += " DI_04_05=" + QString::number(state.DI_State_04_05);
//        logString += " DO_00_01=" + QString::number(state.DO_State_00_01);
//        logString += " DO_02_03=" + QString::number(state.DO_State_02_03);
//        logString += " DO_04_05=" + QString::number(state.DO_State_04_05);
//        logString += " Invertor=" + QString::number(state.InvertorM1_freq, 'g', 6);
//        logString += " O2_00=" + QString::number(state.O2_00, 'g', 6);
//        logString += " O2_01=" + QString::number(state.O2_01, 'g', 6);
//        logString += " O2_02=" + QString::number(state.O2_02, 'g', 6);
//        logString += " O2_03=" + QString::number(state.O2_03, 'g', 6);
//        logString += " O2_04=" + QString::number(state.O2_04, 'g', 6);
//        logString += " P_camera=" + QString::number(state.P_camera, 'g', 6);
//        logString += " Flow_Speed=" + QString::number(state.Flow_speed, 'g', 6);
//        logString += " PDiff_filter_rough=" + QString::number(state.PDiff_filter_rough, 'g', 6);
//        logString += " PDiff_filter_fine=" + QString::number(state.PDiff_filter_fine, 'g', 6);
//        logString += " PDiff_filter_cleaner=" + QString::number(state.PDiff_filter_cleaner, 'g', 6);
//        logString += " Cons_high=" + QString::number(state.Cons_high, 'g', 6);
//        logString += " Cons_low=" + QString::number(state.Cons_low, 'g', 6);
//        logString += " T_platform=" + QString::number(state.T_platform, 'g', 6);
//        logString += " T_camera=" + QString::number(state.T_camera, 'g', 6);
//        logString += " O2_camera_sum=" + QString::number(state.O2_camera_sum, 'g', 6);
//        logString += " PDiff_filter_rough_display=" + QString::number(state.PDiff_filter_rough_display, 'g', 6);
//        logString += " PDiff_filter_fine_display=" + QString::number(state.PDiff_filter_fine_display, 'g', 6);
//        logString += " PDiff_filter_cleaner_display=" + QString::number(state.PDiff_filter_cleaner_display, 'g', 6);

//        logString += " Flow_speed_platform=" + QString::number(state.Flow_speed_platform, 'g', 6);
//        logString += " T_regulation=" + QString::number(state.T_regulation, 'g', 6);

//        logger->writeLog(logString);

//        ReadyToLog = false;
//        logTimer.start();
//    }

}

void MainWindow::connectPLC()
{
    Plc->PLCconnect();
}
void MainWindow::disconnectPLC()
{
    Plc->PLCdisconnect();
}

void MainWindow::clearControlBits()
{
    Plc->writeModbusRegister(0x7C, 0);
    QThread::usleep(200);
}

void MainWindow::setControlsStatusActive(bool enabled)
{
    if(enabled)
    {
//        ui->LE_AFlow_sensor_1->setEnabled(true);
//        ui->LE_AFlow_sensor_2->setEnabled(true);
        ui->LE_O2_box->setEnabled(true);
//        ui->LE_O2_camera_low->setEnabled(true);
        ui->LE_O2_filter_1->setEnabled(true);
        ui->LE_O2_filter_2->setEnabled(true);
        ui->LE_O2_room->setEnabled(true);
        ui->LE_Pdiff_3->setEnabled(true);
        ui->LE_Pdiff_1->setEnabled(true);
        ui->LE_Pdiff_2->setEnabled(true);
        ui->LE_P_box->setEnabled(true);
        ui->LE_T_box->setEnabled(true);
        ui->LE_T_platform->setEnabled(true);
//        ui->LE_Flow_speed->setEnabled(true);
//        ui->LE_InvertorM1_freq->setEnabled(true);
        ui->LE_Flow_speed_platform->setEnabled(true);

//        ui->DSB_SetFlowSpeed->setEnabled(true);
//        ui->DSB_SetPlatformTemp->setEnabled(true);
    }
    else
    {
        ui->PB_Starting->setEnabled(false);
        ui->PB_Stop->setEnabled(false);
        ui->PB_Pause->setEnabled(false);
        ui->PB_Heater_on->setEnabled(false);
        ui->PB_Heater_off->setEnabled(false);
        ui->PB_VacuumCleaner_on->setEnabled(false);
        ui->PB_VacuumCleaner_off->setEnabled(false);
//        ui->tableWidget->setEnabled(false);

//        ui->LE_AFlow_sensor_1->setEnabled(false);
        ui->LE_O2_filter_2->setEnabled(false);
        ui->LE_O2_box->setEnabled(false);
//        ui->LE_O2_camera_low->setEnabled(false);
        ui->LE_O2_filter_1->setEnabled(false);
        ui->LE_O2_filter_2->setEnabled(false);
        ui->LE_O2_room->setEnabled(false);
        ui->LE_Pdiff_3->setEnabled(false);
        ui->LE_Pdiff_1->setEnabled(false);
        ui->LE_Pdiff_2->setEnabled(false);
        ui->LE_P_box->setEnabled(false);
        ui->LE_T_box->setEnabled(false);
        ui->LE_T_platform->setEnabled(false);
//        ui->LE_Flow_speed->setEnabled(false);
//        ui->LE_InvertorM1_freq->setEnabled(false);
        ui->LE_Flow_speed_platform->setEnabled(false);

//        ui->DSB_SetFlowSpeed->setEnabled(false);
//        ui->DSB_SetPlatformTemp->setEnabled(false);

//        scene->setTurbineSpeedM1(0);   // "-" - for CCW rotation on scen

//        scene->setValveOn(1,false);
//        scene->setValveOn(2,false);
//        scene->setValveOn(3,false);
//        scene->setValveOn(4,false);
//        scene->setValveOn(5,false);
//        scene->setValveOn(6,false);
//        scene->setValveOn(7,false);
//        scene->setValveOn(9,false);
//        scene->setValveOn(10,false);
//        scene->setValveOn(11,false);

//        scene->setRelayAirOn(false);
//        scene->setRelayGasOn(false);

//        scene->setPowderFeeder01Full(false);
//        scene->setPowderLossCameraFull(false);
//        scene->setPowderLossFilterFull(false);
//        scene->setPowderLossCleanerFull(false);

//        scene->setFilterCleanerValue(0);
//        scene->setFilterFineValue(0);
//        scene->setFilterRoughValue(0);

//        scene->setPlatformHeating(SCENE_HEATING_OFF_COLD);

        ui->L_SystemState->setText("Статус системы: НЕТ ПОДКЛЮЧЕНИЯ");
        ui->L_SystemState->setStyleSheet("background-color: rgb(227, 113, 113);");  /*red*/
    }
}
void MainWindow::on_PB_Starting_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<0);

    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_Heater_on_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<1);

    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_Heater_off_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<2);

    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_Stop_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<3);


    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_Pause_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<4);


    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_VacuumCleaner_on_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<5);


    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::on_PB_VacuumCleaner_off_clicked()
{
    //clearControlBits();
    quint16 RegisterValue = 0;
    RegisterValue |= (1<<6);


    Plc->writeModbusRegister(0x7C, RegisterValue);
    QThread::usleep(200);
}

void MainWindow::changeStatusActive(QString message)
{
    if(message == tr("Connected!"))
    {
        setControlsStatusActive(true);
        if(!settingsLoadedOnStart) {
            loadSettings();
            settingsLoadedOnStart = true;
        }
        qDebug() << "COnnected";
        QTimer::singleShot(2000, this, SLOT(autoWritePLCParametersOnStart()));
//        logTimer.start();
    }
    else if(message == tr("Disconnected"))
    {
        setControlsStatusActive(false);
    }
}

QVector<quint16> MainWindow::convertFloatToModbusRegistersVector(float value)
{
    QVector<quint16> register_values;

    // convert to raw data
    QByteArray byte_array;
    QDataStream ds_in(&byte_array, QIODevice::WriteOnly);

    ds_in.setByteOrder(QDataStream::LittleEndian);
    ds_in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    ds_in.writeRawData(reinterpret_cast<char*>(&value),
                   sizeof(float));

    // group data to registers
    QDataStream ds_out(&byte_array, QIODevice::ReadOnly);
    ds_out.setByteOrder(QDataStream::LittleEndian);
    ds_out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    quint16 temp_int16;
    ds_out >> temp_int16; register_values.append(temp_int16);
    ds_out >> temp_int16; register_values.append(temp_int16);

    return register_values;
}

//void MainWindow::on_DSB_SetFlowSpeed_editingFinished()
//{
//    //clearControlBits();
//    float value = static_cast<float>(ui->speedFan->value());

//    QVector<quint16> register_values =
//            this->convertFloatToModbusRegistersVector(value);

//    Plc->writeModbusRegisters(0x7E, register_values);
//    QThread::usleep(200);
//}

//void MainWindow::on_DSB_SetPlatformTemp_editingFinished()
//{
//    //clearControlBits();
//    float value = static_cast<float>(ui->DSB_SetPlatformTemp->value());

//    QVector<quint16> register_values =
//            this->convertFloatToModbusRegistersVector(value);

//    Plc->writeModbusRegisters(0x80, register_values);
//    QThread::usleep(200);
//}

//void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
//{
//    int row     = item->row(),
//        column  = item->column();

//    qDebug() << row << column;
//    bool ok;
//    float value = item->text().toFloat(&ok);
//    if(ok)
//    {
//        // set value via modbus by index
//        int baseAddress = 0x82;
//        int index = column*5 + row;

//        QVector<quint16> register_values = this->convertFloatToModbusRegistersVector(value);
//        Plc->writeModbusRegisters(baseAddress+index*2, register_values);
//        QThread::usleep(300);

//        ui->tableWidget->item(row,column)->setBackgroundColor(Qt::white);
//    }
//    else
//    {
//        ui->tableWidget->item(row,column)->setBackgroundColor(QColor(255,200,200,255));
//    }
//}

void MainWindow::setValveOn(unsigned int id, bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;

    switch (id) {
    case 1:
        ui->L_Valve_1->setVisible(visible);
        break;
    case 2:
        ui->L_Valve_2->setVisible(visible);
        break;
    case 3:
        ui->L_Valve_3->setVisible(visible);
        break;
    case 4:
        ui->L_Valve_4->setVisible(visible);
        break;
    case 5:
        ui->L_Valve_5->setVisible(visible);
        break;
    case 6:
        ui->L_Valve_6->setVisible(visible);
        break;
    case 7:
        ui->L_Valve_7->setVisible(visible);
        break;
    case 9:
        ui->L_Valve_8->setVisible(visible);
        break;
    case 10:
        ui->L_Valve_9->setVisible(visible);
        break;
    case 11:
        ui->L_Valve_21->setVisible(visible);
        break;
    case 12:
        ui->L_Valve_22->setVisible(visible);
        break;
    case 13:
        ui->L_Valve_23->setVisible(visible);
        break;
    case 14:
        ui->L_Valve_24->setVisible(visible);
        break;
    case 15:
        ui->L_Valve_25->setVisible(visible);
        break;
    case 16:
        ui->L_Valve_26->setVisible(visible);
        break;
    case 17:
        if(enabled)
            ui->Fan->startRotate();
        else
            ui->Fan->stopRotate();
        break;
    case 18:
        if(enabled)
            ui->Fan_2->startRotate();
        else
            ui->Fan_2->stopRotate();
        break;
    case 19:
        ui->L_Heater->setVisible(visible);
        if(visible)
        {
            ui->PB_Heater_on->setEnabled(false);
            ui->PB_Heater_off->setEnabled(true);
        }
        else
        {
            ui->PB_Heater_on->setEnabled(true);
            ui->PB_Heater_off->setEnabled(false);
        }
        break;
//    case 20:
//        PIValve20->setOpacity(opacity);
//        break;
//    case 21:
//        PIValve21->setOpacity(opacity);
//        break;
    default:
        break;
    }
}

void MainWindow::setRelayAirOn(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    //ui->L_PRelay_1->setVisible(visible);
}

void MainWindow::setRelayGasOn(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    //ui->L_PRelay_2->setVisible(visible);
}

void MainWindow::PowderBox(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderBoxF->setVisible(visible);
}

void MainWindow::PowderDrop1(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderDrop1F->setVisible(visible);
}

void MainWindow::PowderDrop2(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderDrop2F->setVisible(visible);
}

void MainWindow::PowderFill(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderFillF->setVisible(visible);
}

void MainWindow::PowderFilter1(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderFilter1F->setVisible(visible);
}

void MainWindow::PowderFilter2(bool enabled)
{
    bool visible;
    if(enabled)
        visible = true;
    else
        visible = false;
    ui->L_PowderFilter2F->setVisible(visible);
}

//void MainWindow::PowderScrew(bool enabled)
//{
//    bool visible;
//    if(enabled)
//        visible = true;
//    else
//        visible = false;
//    ui->L_PowderScrew->setVisible(visible);
//}

//void setTurbineSpeedM1(float percents)
//{
//    if(percents > 100)
//        percents = 100;
//    else if(percents < -100)
//        percents = -100;
//    TurbineM1_step = turbine_max_angle_step * percents / 100;
//}

//void MainWindow::on_start_clicked()
//{
//    plcController->start();
//    //ui->Fan->startRotate();
//    ui->L_Valve_1->setVisible(true);
//}

//void MainWindow::on_pause_clicked()
//{
//    plcController->pause();
//    ui->Fan->stopRotate();
//}

//void MainWindow::on_stop_clicked()
//{
//    plcController->stop();
//    ui->Fan->stopRotate();
//    ui->L_Valve_1->setHidden(true);
//}

//void MainWindow::on_PB_Start_clicked()
//{
//    plcController->start();
//    ui->Fan->startRotate();
//    ui->Fan_2->startRotate();
//}

//void MainWindow::on_PB_Pause_clicked()
//{
//    plcController->pause();
//    ui->Fan->stopRotate();
//}

//void MainWindow::on_PB_Stop_clicked()
//{
//    plcController->stop();
//    ui->Fan->stopRotate();
//}
