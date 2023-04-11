//
// Created by 11851 on 2023/4/9.
//

#include "core/module/GuiModule.h"

using namespace baseModule;

GuiModule::GuiModule(const std::string &module_name, baseModule::BaseName base_name, QObject *parent)
        : BaseModule(module_name, base_name, parent), m_widget(nullptr) {
    m_is_threaded = true;
}

QWidget *GuiModule::guiWidget() {
    return m_widget;
}

GuiModule::~GuiModule() {

}
