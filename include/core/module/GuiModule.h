//
// Created by 11851 on 2023/4/9.
//

#ifndef GUIBASE_H
#define GUIBASE_H

#include "core/module/BaseModule.h"

namespace baseModule {
    class GuiModule;
}

class baseModule::GuiModule : public baseModule::BaseModule {
Q_OBJECT
public:
    GuiModule(const std::string &module_name, BaseName base_name, QObject *parent = nullptr);

    virtual ~GuiModule();

    virtual void show() = 0;

protected:
    QWidget *m_widget;
};


#endif //GUIBASE_H
