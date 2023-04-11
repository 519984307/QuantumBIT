//
// Created by 11851 on 2023/4/9.
//

#ifndef LOGICMODULE_H
#define LOGICMODULE_H

#include "core/module/BaseModule.h"

namespace baseModule {
    class LogicModule;
}

class baseModule::LogicModule : public baseModule::BaseModule {
Q_OBJECT
public:
    explicit LogicModule(const std::string &module_name, QObject *parent = nullptr);

    virtual ~LogicModule();

};

#endif //LOGICMODULE_H
