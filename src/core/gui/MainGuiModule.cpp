/**
   *******************************************
   * @file      : MainGuiModule.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "core/gui/MainGuiModule.h"

using namespace baseModule;

MainGuiModule::MainGuiModule() :
        baseModule::GuiModule("MainGui", baseModule::Gui),
        m_main_window(new MainWindow) {

}

MainGuiModule::~MainGuiModule() {
    m_main_window->deleteLater();
}

void MainGuiModule::addModuleWidget(const QString &title, const QIcon &icon, QWidget *module_widget) {
    m_main_window->addModule(title, icon, module_widget);
}

QList<BaseModule *> MainGuiModule::dependency() {
    return {};
}

void MainGuiModule::onActivate() {

}

void MainGuiModule::onDeactivate() {

}

void MainGuiModule::onLock() {

}

void MainGuiModule::onUnLock() {

}

void MainGuiModule::show() {
    if (m_main_window) {
        m_main_window->show();
        m_main_window->activateWindow();
    }

}
