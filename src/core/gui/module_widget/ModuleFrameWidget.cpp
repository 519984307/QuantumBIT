/**
   *******************************************
   * @file      : ModuleFrameWidget.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "ModuleFrameWidget.h"
#include "QToolButton"

ModuleFrameWidget::ModuleFrameWidget(const QString &title, const QIcon &icon, QWidget *parent) :
        QWidget(parent),
        m_button(new QToolButton()) {
    m_button->setText(title);
    m_button->setIcon(icon);
    m_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void ModuleFrameWidget::setModuleName(const QString &name) {
    m_button->setText(name);
}

void ModuleFrameWidget::setModuleIcon(const QIcon &icon) {
    m_button->setIcon(icon);
}

QToolButton *ModuleFrameWidget::getButton() {
    return m_button;
}
