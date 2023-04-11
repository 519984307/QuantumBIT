/**
   *******************************************
   * @file      : ModuleWindowWidget.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "core/gui/module_widget/ModuleWindowWidget.h"
#include "QGridLayout"

ModuleWindowWidget::ModuleWindowWidget(QWidget *parent) :
        QWidget(parent),
        m_module_widgets(new QStackedWidget()) {
    setUpUi();
}

void ModuleWindowWidget::addModule(QWidget *module_window) {
    m_module_widgets->addWidget(module_window);
}

void ModuleWindowWidget::switchModuleWindow(int index) {
    if (index >= 0 && index < m_module_widgets->count()) {
        m_module_widgets->setCurrentIndex(index);
    }
}

void ModuleWindowWidget::setUpUi() {
    auto layout = new QGridLayout();
    layout->addWidget(m_module_widgets);
    setLayout(layout);
}

