//
// Created by 11851 on 2023/4/10.
//

#include "core/gui/SystemTrayIcon.h"
#include "QApplication"
#include "QMenu"

SystemTrayIcon::SystemTrayIcon(QObject *parent) :
        QSystemTrayIcon(parent),
        m_manager_action(nullptr),
        m_quit_action(nullptr),
        m_left_menu(nullptr),
        m_right_menu(nullptr) {
    setUpUi();
}

void SystemTrayIcon::setUpUi() {
    setIcon(QApplication::windowIcon());
    m_right_menu = new QMenu("Quit");
    m_left_menu = new QMenu("MainWindow");

    auto manager_icon = QIcon();
    manager_icon.addFile(":/icons/icons_home.svg", QSize(16, 16));
    m_manager_action = new QAction(manager_icon, "MainWindow", m_left_menu);

    auto exit_icon = QIcon();
    exit_icon.addFile(":/icons/icons_app_quit.svg", QSize(16, 16));
    m_quit_action = new QAction(exit_icon, "Quit", m_right_menu);

    m_left_menu->addAction(m_manager_action);
    m_right_menu->addAction(m_quit_action);
    setContextMenu(m_right_menu);
    connect(this, &QSystemTrayIcon::activated, this, &SystemTrayIcon::handleActivation);
    connect(m_manager_action, &QAction::triggered, this, &SystemTrayIcon::showMainWindow);
    connect(m_quit_action, &QAction::triggered, this, &SystemTrayIcon::quit);
}

void SystemTrayIcon::handleActivation(QSystemTrayIcon::ActivationReason reason) {
    if (reason == Trigger) {
        m_left_menu->exec(QCursor::pos());
    }
}

SystemTrayIcon::~SystemTrayIcon() {
    m_left_menu->deleteLater();

}
