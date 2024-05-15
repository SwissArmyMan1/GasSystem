#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QMainWindow>
#include <QModbusClient>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QSettings>
#include <QTableWidgetItem>
#include <bitset>
#include <QPropertyAnimation>

#include <QLabel>
#include <plc.h>
#include <QPainter>


#include "plccontroller.h"
//#include "valves.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef std::bitset<CHAR_BIT * sizeof(int)> IntBits;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void setValveOn(unsigned int id, bool enabled);
    void setRelayAirOn(bool enabled);
    void setRelayGasOn(bool enabled);
    void PowderBox(bool enabled);
    void PowderDrop1(bool enabled);
    void PowderDrop2(bool enabled);
    void PowderFill(bool enabled);
    void PowderFilter1(bool enabled);
    void PowderFilter2(bool enabled);

    void changeStatusActive(QString message);

private slots:
    void clearControlBits();
      void setControlsStatusActive(bool enabled);

    void loadSettings();
    void saveSettings();
    void showUpdatedState(PLC_State state);
    void connectPLC();
    void disconnectPLC();

    void on_PB_Starting_clicked();
    void on_PB_Pause_clicked();
    void on_PB_Stop_clicked();
    void on_PB_Heater_on_clicked();
    void on_PB_Heater_off_clicked();
    void on_PB_VacuumCleaner_on_clicked();
    void on_PB_VacuumCleaner_off_clicked();

    QVector<quint16> convertFloatToModbusRegistersVector(float value);
//    void on_DSB_SetFlowSpeed_editingFinished();
//    void on_DSB_SetPlatformTemp_editingFinished();

private:
    Ui::MainWindow *ui;
//    PlcController *plcController = nullptr;
//    void Valve1();
    void Background();
    QSettings *settings;//доб
    PLC *Plc; //доб. 10.05

    bool settingsLoadedOnStart = false;

};
#endif // MAINWINDOW_H
