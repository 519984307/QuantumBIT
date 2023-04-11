/**
   *******************************************
   * @file      : MainWindow.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "core/gui/module_widget/ModuleViewWidget.h"
#include "core/gui/module_widget/ModuleWindowWidget.h"

class MainWindow : public QMainWindow {
    friend class MainGuiModule;

Q_OBJECT
public:
    MainWindow();

    ~MainWindow() override = default;

    void addModule(const QString &title, const QIcon &icon, QWidget *widget);

private:
    void setUpUi();

    ModuleViewWidget *m_module_view;
    ModuleWindowWidget *m_module_window;


};


#endif //MAINWINDOW_H
