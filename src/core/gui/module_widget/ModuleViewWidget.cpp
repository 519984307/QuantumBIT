/**
   *******************************************
   * @file      : ModuleViewWidget.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "core/gui/module_widget/ModuleViewWidget.h"
#include "QToolButton"
#include "QVBoxLayout"

ModuleViewWidget::ModuleViewWidget(QWidget *parent) :
        QWidget(parent),
        m_module_group(new QButtonGroup(this)),
        m_module_count(0),
        m_layout(new QVBoxLayout(this)) {
    setUpUi();

}

void ModuleViewWidget::addModule(const QString &title, const QIcon &icon) {
    auto module_button = new QToolButton();
    m_layout->addWidget(module_button, 0, Qt::AlignCenter);
    module_button->setText(title);
    module_button->setIcon(icon);
    module_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_module_group->addButton(module_button, m_module_count++);
}

void ModuleViewWidget::setUpUi() {
    setLayout(m_layout);
    m_module_group->setExclusive(true);
    connect(m_module_group, &QButtonGroup::idClicked, this, &ModuleViewWidget::showWindow);
}



