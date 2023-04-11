//
// Created by 11851 on 2023/4/9.
//

#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include "QObject"
#include "QList"
#include "core/module/BaseModule.h"
#include "core/module/LogicModule.h"
#include "core/module/GuiModule.h"
#include "QRecursiveMutex"
#include "spdlog/spdlog.h"


class ModuleManager final : public QObject {
Q_OBJECT
public:
    explicit ModuleManager(QObject *parent = nullptr);

    ~ModuleManager() final;

    void appendModule(baseModule::BaseModule *base_module);

    void removeModule(baseModule::BaseModule *base_module);

    void clearModuleList();

public slots:

    void activateModule(baseModule::BaseModule *base_module);

    void deActivateModule(baseModule::BaseModule *base_module);

public:

    void activateAllModule();

    void deActivateAllModule();


private:
    std::shared_ptr<spdlog::logger> m_logger;
    QRecursiveMutex m_mutex;
    QList<baseModule::BaseModule *> m_module_list;
};


#endif //MODULEMANAGER_H
