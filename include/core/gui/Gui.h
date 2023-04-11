/**
   *******************************************
   * @file      : Gui.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef GUI_H
#define GUI_H

#include "QObject"
#include "core/main/QuantumBIT.h"
#include "core/module/BaseModule.h"
#include "SystemTrayIcon.h"
#include "MainGuiModule.h"
#include "core/logger/logger.h"
#include "QList"

class Gui : public QObject {
Q_OBJECT
public:
    explicit Gui(QuantumBIT *main_object, const QString &style_sheet_path = "", QObject *parent = nullptr);

    ~Gui() override = default;

    static void setStyleSheet(const QString &file_path);

    static void closeWindow();

    void appendStackedWidget(QWidget *widget);

    void showSystemTray();

    void hideSystemTray();

    void closeSystemTray();

    void trayIconBubble(const baseModule::Message &message);


public slots:

    void activateMainGui();

    void deActivateMainGui();


signals:

    void popUpMessageSignal(const baseModule::Message &message);

    void balloonMessageSignal(const baseModule::Message &message);

public slots:

    void popUpMessageSlot(const baseModule::Message &message);

    void balloonMessageSlot(const baseModule::Message &message);


private:
    QuantumBIT *m_main;

    static void initAppIcon();

    SystemTrayIcon *m_tray_icon;

    MainGuiModule *m_main_gui;

    std::shared_ptr<spdlog::logger> m_logger;

    QList<QMetaObject::Connection> m_connections;
};


#endif //GUI_H
