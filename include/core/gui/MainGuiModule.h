/**
   *******************************************
   * @file      : MainGuiModule.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MAINGUIMODULE_H
#define MAINGUIMODULE_H

#include "core/module/GuiModule.h"
#include "MainWindow.h"
#include "QIcon"

class MainGuiModule : public baseModule::GuiModule {
Q_OBJECT
public:
    MainGuiModule();

    ~MainGuiModule() override;

    void addModuleWidget(const QString &title, const QIcon &icon, QWidget *module_widget);

    QList<BaseModule *> dependency() override;

protected:
    void onActivate() override;

    void onDeactivate() override;

    void onLock() override;

    void onUnLock() override;


    void show() override;

    MainWindow *m_main_window;

};


#endif //MAINGUIMODULE_H
