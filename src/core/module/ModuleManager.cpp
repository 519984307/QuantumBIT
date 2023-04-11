//
// Created by 11851 on 2023/4/9.
//

#include "core/module/ModuleManager.h"
#include "core/logger/logger.h"
#include "core/thread/ThreadManager.h"
#include "QApplication"

ModuleManager::ModuleManager(QObject *parent) : QObject(parent), m_logger(getLogger("ModuleManager")) {
}

ModuleManager::~ModuleManager() = default;

void ModuleManager::appendModule(baseModule::BaseModule *base_module) {
    if (std::find(m_module_list.begin(), m_module_list.end(), base_module) != m_module_list.end()) {
        m_logger->error("module {} is already in the list.", base_module->objectName().toStdString());
        return;
    }
    m_module_list.push_back(base_module);
    base_module->setParent(this);
    m_logger->debug("add module {}", base_module->objectName().toStdString());

    for (auto child: base_module->dependency()) {
        appendModule(child);
    }
}

void ModuleManager::removeModule(baseModule::BaseModule *base_module) {
    auto pos = std::find(m_module_list.begin(), m_module_list.end(), base_module);
    if (pos != m_module_list.end()) {
        deActivateModule(base_module);
        for (auto child: base_module->dependency()) {
            removeModule(child);
        }
        m_module_list.erase(pos);
    }

}

void ModuleManager::activateModule(baseModule::BaseModule *base_module) {
    if (!base_module) { return; }

    auto thread_manager = ThreadManager::getInstance();
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "activateModule", Qt::BlockingQueuedConnection);
        if (base_module->state() == baseModule::DeActivated) {
            m_logger->error("activate module {} failed", base_module->objectName().toStdString());
            return;
        }

        QMutexLocker<QRecursiveMutex> locker(&m_mutex);
        if (base_module->state() == baseModule::Idle) {
            if (base_module->baseName() == baseModule::Gui) {
                static_cast<baseModule::GuiModule *>(base_module)->show();
            }
            return;
        }

        m_logger->debug("activate module {}", base_module->objectName().toStdString());
        for (auto child: base_module->dependency()) {
            activateModule(child);
        }

        if (base_module->isThreaded()) {
            auto thread = thread_manager->getNewThread(base_module->objectName());
            base_module->moveToThread(thread);
            thread->start();
            QMetaObject::invokeMethod(base_module, "activate", Qt::BlockingQueuedConnection);
            if (base_module->state() != baseModule::Idle) {
                QMetaObject::invokeMethod(base_module, "moveToMainThread", Qt::BlockingQueuedConnection);
                thread_manager->quitThread(thread);
                thread_manager->joinThread(thread, -1);
            }
        } else {
            base_module->activate();
        }
    }
}

void ModuleManager::deActivateModule(baseModule::BaseModule *base_module) {
    if (!base_module) { return; }
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "deActivateModule", Qt::BlockingQueuedConnection);
        if (base_module->state() != baseModule::DeActivated) {
            m_logger->error("deactivate module {} failed", base_module->objectName().toStdString());
            return;
        }


    }
    auto thread_manager = ThreadManager::getInstance();
    QMutexLocker<QRecursiveMutex> locker(&m_mutex);
    if (base_module->state() == baseModule::DeActivated) {
        return;
    }
    m_logger->debug("deactivate module {}", base_module->objectName().toStdString());
    for (auto child: base_module->dependency()) {
        deActivateModule(child);
    }
    if (base_module->isThreaded()) {
        auto thread = thread_manager->getThreadByName(base_module->objectName());
        QMetaObject::invokeMethod(base_module, "deActivate", Qt::BlockingQueuedConnection);
        QMetaObject::invokeMethod(base_module, "moveToMainThread", Qt::BlockingQueuedConnection);
        thread_manager->quitThread(thread);
        thread_manager->joinThread(thread, -1);
    } else {
        base_module->deActivate();
    }
    QApplication::processEvents();
    if (base_module->state() != baseModule::DeActivated) {
        m_logger->error("deactivate module {} failed", base_module->objectName().toStdString());
    }
}

void ModuleManager::activateAllModule() {
    for (auto mod: m_module_list) {
        activateModule(mod);
    }
}

void ModuleManager::deActivateAllModule() {
    QMutexLocker<QRecursiveMutex> locker(&m_mutex);
    for (auto mod: m_module_list) {
        deActivateModule(mod);
    }

}

void ModuleManager::clearModuleList() {
    m_module_list.clear();
}
