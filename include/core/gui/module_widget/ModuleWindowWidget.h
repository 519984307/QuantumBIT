/**
   *******************************************
   * @file      : ModuleWindowWidget.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULEWINDOWWIDGET_H
#define MODULEWINDOWWIDGET_H

#include "QStackedWidget"
#include "QWidget"

class ModuleWindowWidget : public QWidget {
Q_OBJECT
public:
    explicit ModuleWindowWidget(QWidget *parent = nullptr);

    ~ModuleWindowWidget() override = default;

    void addModule(QWidget *module_window);

public slots:

    void switchModuleWindow(int index);

private:
    void setUpUi();

    QStackedWidget *m_module_widgets;
};


#endif //MODULEWINDOWWIDGET_H
