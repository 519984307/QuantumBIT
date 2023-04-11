//
// Created by 11851 on 2023/4/7.
//

#ifndef BASEMODULE_H
#define BASEMODULE_H

#include "core/fsm/fsm.h"
#include "QObject"
#include "spdlog/spdlog.h"
#include "QList"
#include "nlohmann/json.hpp"
#include "QUuid"
#include "core/main/QuantumBIT.h"
#include "QRecursiveMutex"


namespace baseModule {
    enum State {
        DeActivated,
        Idle,
        Locked
    };

    enum BaseName {
        Gui,
        Hardware,
        Logic
    };

    struct Message {
        QString title;
        QString message;
    };

    class BaseModule : public QObject {
    Q_OBJECT
    public:
        BaseModule(const std::string &module_name, BaseName base_name, QObject *parent = nullptr);

        virtual ~BaseModule();

        virtual QList<BaseModule *> dependency()=0;

    public slots:

        void activate();

        void deActivate();

        void lock();

        void unLock();

        void moveToMainThread();

    public:
        BaseName baseName();

        [[nodiscard]] State state() const;

        [[nodiscard]] const QUuid &moduleUuid() const;

        [[nodiscard]] bool isThreaded() const;

        QThread *moduleThread();

    protected:

        virtual void onActivate() = 0;

        virtual void onDeactivate() = 0;

        virtual void onLock() = 0;

        virtual void onUnLock() = 0;

    protected:

        void sendBalloonMessage(const Message &message);

        void sendPopUpMessage(const Message &message);

        nlohmann::json m_status_variables;
        nlohmann::json m_default_variables;
        std::shared_ptr<spdlog::logger> m_logger;
        QuantumBIT *m_main;
        bool m_is_threaded; // whether the module shall be started in its own thread
    private:
        void loadStatusVariable();

        void dumpStatusVariable() const;

        BaseName m_base_name;
        QUuid m_uuid;
        State m_state;
    };
}

#endif //BASEMODULE_H
