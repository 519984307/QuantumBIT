/**
   *******************************************
   * @file      : MainWindow.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "core/gui/MainWindow.h"
#include "core/qspdlog/qspdlog.hpp"
#include "QGridLayout"

MainWindow::MainWindow() :
        QMainWindow(),
        m_module_view(new ModuleViewWidget()),
        m_module_window(new ModuleWindowWidget()) {
    setUpUi();
}

void MainWindow::setUpUi() {
    auto layout = new QGridLayout();
    setLayout(layout);
    layout->addWidget(m_module_view, 0, 0);
    layout->addWidget(m_module_window, 0, 1);
    auto log_widget = new QSpdLog();
    addModule("Log", QIcon(":/icons/icons_log_50.png"), log_widget);
}

void MainWindow::addModule(const QString &title, const QIcon &icon, QWidget *widget) {
    m_module_view->addModule(title, icon);
    m_module_window->addModule(widget);
}
