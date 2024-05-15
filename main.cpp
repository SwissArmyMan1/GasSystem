#include "mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    /* App global parameters */
    a.setApplicationName("GasSystemMonitor");
    a.setApplicationVersion("0.0.1");
    a.setOrganizationName("LaserApr");

    w.show();

    /* QSS import from file */
    QFile styleF;
    styleF.setFileName(QCoreApplication::applicationDirPath() + "/qss/style.css");
    styleF.open(QFile::ReadOnly);   // CSS by file
    QString qssStr = styleF.readAll();
    // CSS by const string
    //QString qssStr = "*{margin: 0px;}QMainWindow {border: 2px solid #8f8f91;background-image: url('./img/Metallic Surface.jpg');}QPushButton {border: 1px solid #8f8f91;/*border-radius: 6px;*/background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde);/*padding: 20px;*/}QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dadbde, stop: 1 #f6f7fa);}QPushButton:flat {border: none;/* no border for a flat push button */}QPushButton:default {    border-color: navy;/* make the default button prominent */}QLineEdit {border-color: navy;border: 1px solid #8f8f91;/*background-color: #f6f7fa;*/background-color: rgba(255,255,255,0.5);}#buttonMinimize {border: none;background-color: rgba(0,0,0,0);image: url('./img/img_min.png');}#buttonMinimize:hover {image: url('./img/img_min_hover.png');}#buttonMinimize:pressed {image: url('./img/img_min_active.png');}#buttonMaximize {border: none;background-color: rgba(0,0,0,0);image: url('./img/img_max.png');}#buttonMaximize:hover {image: url('./img/img_max_hover.png');}#buttonMaximize:pressed {image: url('./img/img_max_active.png');}#buttonClose {border: none;background-color: rgba(0,0,0,0);image: url('./img/img_close.png');}#buttonClose:hover {image: url('./img/img_close_hover.png');}#buttonClose:pressed {image: url('./img/img_close_active.png');}#buttonOpenFile {border: none;background-color: rgba(0,0,0,0);image: url('./img/open.png');width: 50px;height: 50px;border-radius: 30px;}#buttonOpenFile:hover {image: url('./img/open_hover.png');}#buttonOpenFile:pressed {background-color: rgba(255,255,255,0.4);}QSlider::groove:vertical {border: 1px solid #bbb;background: white;width: 10px;border-radius: 4px;}QSlider::add-page:vertical {background: qlineargradient(x1: 0, y1: 0,    x2: 1, y2: 0,stop: 0 #66e, stop: 1 #bbf);background: qlineargradient(x1: 1, y1: 1, x2: 0.2, y2: 0,stop: 0 #bbf, stop: 1 #55f);border: 1px solid #777;width: 10px;border-radius: 4px;}QSlider::sub-page:vertical {background: #fff;border: 1px solid #777;width: 10px;border-radius: 4px;}QSlider::handle:vertical {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #eee, stop:1 #ccc);border: 1px solid lightgray;height: 11px;width: 20px;margin-left: -15px;margin-right: -15px;border-radius: 0px;image: url('./img/metal_button.jpg');}QSlider::handle:vertical:hover {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #fff, stop:1 #ddd);}QSlider::sub-page:vertical:disabled {background: #bbb;border-color: #999;}QSlider::add-page:vertical:disabled {background: #eee;border-color: #999;}QSlider::handle:vertical:disabled {background: #eee;border: 1px solid #aaa;border-radius: 4px;image: url('./img/img_close.png');}QTabWidget {/* The tab widget frame */}QTabWidget:pane {border: none;background-color: rgba(255,255,255,0.5);}#tab_01 {}#tab_02 {}QTabWidget::tab-bar {    alignment: center;}QTabBar::tab {background-color: rgba(0,0,0,0.2);/*background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,stop: 0.5 #D8D8D8, stop: 1.0 #FFFFFF);*/border: 0px solid #C4C4C3;border-bottom-color: #C2C7CB;/* same as the pane color */    /*border-top-left-radius: 4px;border-top-right-radius: 4px;*/width: 80px;height: 50px;padding: 2px;}QTabBar::tab:selected {background-color: rgba(255,255,255,0.5);/*background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #fafafa, stop: 0.4 #f4f4f4,stop: 0.5 #e7e7e7, stop: 1.0 #FFFFFF);*/}QTabBar::tab:selected:hover {background-color: rgba(255,255,255,0.8);}QTabBar::tab:!selected:hover {background-color: rgba(0,0,0,0.1);}QTabBar::tab:selected {border-color: #9B9B9B;border-bottom: 0px;border-bottom-color: #C2C7CB;/* same as pane color */}QGroupBox { font-size: 12pt;font-weight: bold;margin-top: 10px;background-color: rgba(255,255,255,0.5);}QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center;/* position at the top center */padding: 0px 0px;}#widget2D3D {background-color: rgb(221,221,221);}QCheckBox::indicator {width: 50px;height: 25px;}QCheckBox::indicator:checked {image: url('./img/switch_3D.png');}QCheckBox::indicator:unchecked { image: url('./img/switch_2D.png');}";
    qApp->setStyleSheet(qssStr);    // apply to whole Application

    return a.exec();
}
