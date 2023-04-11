//
// Created by 11851 on 2023/4/9.
//

#include "core/module/LogicModule.h"

using namespace baseModule;

LogicModule::LogicModule(const std::string &module_name, QObject *parent) :
        BaseModule(module_name, Logic, parent) {

}

LogicModule::~LogicModule() {

}
