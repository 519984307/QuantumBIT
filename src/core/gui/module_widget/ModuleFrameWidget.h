/**
   *******************************************
   * @file      : ModuleFrameWidget.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULEFRAMEWIDGET_H
#define MODULEFRAMEWIDGET_H

#include "QWidget"
#include "core/module/BaseModule.h"
#include "QToolButton"

class ModuleFrameWidget : public QWidget {
Q_OBJECT
public:
    ModuleFrameWidget(const QString &title, const QIcon &icon, QWidget *parent = nullptr);

    ~ModuleFrameWidget() override=default;

    void setModuleName(const QString &name);

    void setModuleIcon(const QIcon &icon);

    QToolButton *getButton();

private:

    QToolButton *m_button;
};


#endif //MODULEFRAMEWIDGET_H
