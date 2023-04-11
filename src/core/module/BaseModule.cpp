//
// Created by 11851 on 2023/4/7.
//

#include "core/module/BaseModule.h"
#include "core/logger/logger.h"
#include "QDir"
#include "fstream"
#include "QMutexLocker"
#include "QApplication"

using namespace baseModule;

BaseModule::BaseModule(const std::string &module_name, baseModule::BaseName base_name, QObject *parent) :
        QObject(parent),
        m_base_name(base_name),
        m_main(QuantumBIT::getInstance()),
        m_is_threaded(false),
        m_uuid(QUuid::createUuid()),
        m_state(DeActivated),
        m_logger(getLogger(module_name.c_str())) {
    setObjectName(module_name);
}

BaseModule::~BaseModule() {
}

void BaseModule::activate() {
    if (m_state == DeActivated) {
        loadStatusVariable();
        m_state = Idle;
    }
}

void BaseModule::deActivate() {
    if (m_state == Idle) {
        dumpStatusVariable();
        m_state = DeActivated;
    }

}

void BaseModule::lock() {
    if (m_state == Idle) {
        onLock();
    } else {
        m_logger->error("try to lock a module not in Idle state");
    }
}

void BaseModule::unLock() {
    if (m_state == Locked) {
        onUnLock();
    } else {
        m_logger->error("try to unlock a module not in Locked state");
    }
}

BaseName BaseModule::baseName() {
    return m_base_name;
}

void BaseModule::loadStatusVariable() {
    QString file_name =
            QDir::homePath() + QDir::separator() + ".quantum_bit" + QDir::separator() + objectName() + ".json";
    if (QFileInfo::exists(file_name)) {
        std::ifstream in(file_name.toStdString());
        if (in.is_open()) {
            in >> m_status_variables;
            in.close();
            return;
        }
    }
    m_logger->warn("load status variable failed, using default variables instead");
    m_status_variables = m_default_variables;
}

void BaseModule::dumpStatusVariable() const {
    QString file_name =
            QDir::homePath() + QDir::separator() + ".quantum_bit" + QDir::separator() + objectName() + ".json";
    QDir dir;
    auto success = dir.mkpath(dir.filePath(file_name));
    if (success) {
        std::ofstream out(file_name.toStdString());
        if (out.is_open()) {
            out << m_status_variables;
            out.close();
            return;
        }
    }
    m_logger->warn("dump status variables failed");
}

const QUuid &BaseModule::moduleUuid() const {
    return m_uuid;
}

bool BaseModule::isThreaded() const {
    return m_is_threaded;
}

void BaseModule::moveToMainThread() {
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "moveToMainThread", Qt::BlockingQueuedConnection);
    } else {
        moveToThread(qApp->thread());
    }
}

QThread *BaseModule::moduleThread() {
    if (m_is_threaded) {
        return thread();
    }
    return nullptr;
}

State BaseModule::state() const {
    return m_state;
}

void BaseModule::sendBalloonMessage(const Message &message) {
    if (!m_main) {
        return;
    }
    if (!m_main->gui()) {
        auto logger = getLogger("balloon-message");
        logger->warn("{}: {}", message.title.toStdString(), message.message.toStdString());
        return;
    }
    m_main->gui()->balloonMessageSlot(message);
}

void BaseModule::sendPopUpMessage(const Message &message) {
    if (!m_main) {
        return;
    }
    if (!m_main->gui()) {
        auto logger = getLogger("pop-up-message");
        logger->warn("{}: {}", message.title.toStdString(), message.message.toStdString());
        return;
    }
    m_main->gui()->popUpMessageSlot(message);
}


