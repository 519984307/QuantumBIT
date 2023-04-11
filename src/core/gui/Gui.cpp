/**
   *******************************************
   * @file      : Gui.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "core/gui/Gui.h"
#include "QApplication"
#include "QDir"
#include "QMessageBox"
#include "fstream"

Gui::Gui(QuantumBIT *main_object, const QString &style_sheet_path, QObject *parent) :
        QObject(parent),
        m_main(main_object),
        m_tray_icon(new SystemTrayIcon(this)),
        m_main_gui(new MainGuiModule()),
        m_logger(getLogger("Gui")) {
    qApp->setQuitOnLastWindowClosed(false);

    initAppIcon();
    if (!style_sheet_path.isEmpty()) {
        setStyleSheet(style_sheet_path);
    }


    m_connections.push_back(
            connect(this, &Gui::popUpMessageSignal, this, &Gui::popUpMessageSlot, Qt::QueuedConnection));
    m_connections.push_back(
            connect(this, &Gui::balloonMessageSignal, this, &Gui::balloonMessageSlot, Qt::QueuedConnection));

    m_connections.push_back(connect(m_tray_icon, &SystemTrayIcon::showMainWindow, this, &Gui::activateMainGui,
                                    Qt::QueuedConnection));
    m_connections.push_back(connect(m_tray_icon, &SystemTrayIcon::quit, this, &Gui::deActivateMainGui,
                                    Qt::QueuedConnection));

    showSystemTray();
}

void Gui::setStyleSheet(const QString &file_path) {
    if (QFileInfo::exists(file_path)) {
        QFile file(file_path);
        file.open(QFile::ReadOnly | QFile::Text);
        QString style_sheet = QString::fromUtf8(file.readAll());
        qApp->setStyleSheet(style_sheet);
    } else {
        spdlog::get("Gui")->warn("open style sheet file failed");
    }
}

void Gui::closeWindow() {
    qApp->closeAllWindows();

}

void Gui::showSystemTray() {
    m_tray_icon->show();
}

void Gui::hideSystemTray() {
    m_tray_icon->hide();
}

void Gui::closeSystemTray() {
    hideSystemTray();
    for (auto &connection: m_connections) {
        disconnect(connection);
    }
}

void Gui::trayIconBubble(const baseModule::Message &message) {
    if (m_tray_icon) {
        m_tray_icon->showMessage(message.title, message.message, QIcon(), 15 * 1000);
    }
}

void Gui::activateMainGui() {
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "activateMainGui", Qt::BlockingQueuedConnection);
        return;
    }

    if (m_main_gui->state() != baseModule::DeActivated) {
        m_main_gui->show();
    }
    m_logger->debug("activating main gui module");
    m_main_gui->activate();
    qApp->processEvents();
}

void Gui::deActivateMainGui() {
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "deActivateMainGui", Qt::BlockingQueuedConnection);
        return;
    }
    if (m_main_gui->state() == baseModule::DeActivated) {
        return;
    }
    m_main_gui->deActivate();
    qApp->processEvents();
}

void Gui::popUpMessageSlot(const baseModule::Message &message) {
    if (thread() != QThread::currentThread()) {
        emit popUpMessageSignal(message);
        return;
    }
    QMessageBox::information(nullptr, message.title, message.message, QMessageBox::Ok);
}

void Gui::balloonMessageSlot(const baseModule::Message &message) {
    if (!SystemTrayIcon::supportsMessages()) {
        m_logger->warn("{}: {}", message.title.toStdString(), message.message.toStdString());
        return;
    }
    if (thread() != QThread::currentThread()) {
        emit balloonMessageSignal(message);
        return;
    }
    trayIconBubble(message);
}

void Gui::initAppIcon() {
    auto app_icon = QIcon(":/logo/logo-qudi.svg");
    qApp->setWindowIcon(app_icon);
}





