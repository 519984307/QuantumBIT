//
// Created by 11851 on 2023/4/7.
//

#ifndef QUANTUMBIT_H
#define QUANTUMBIT_H

#include "core/logger/logger.h"
#include "core/thread/ThreadManager.h"
#include "QObject"
#include "core/gui/Gui.h"

class QuantumBIT : public QObject {
public:
    QuantumBIT();

    static QuantumBIT *getInstance();

    bool isRunning() const;

    ~QuantumBIT();

    void run();

    ThreadManager *threadManager();

    Gui *gui();


private:
    static QuantumBIT *m_instance;
    ThreadManager *m_thread_manager;

    bool m_is_running;
    QMutex m_run_mutex;
    QMutex m_quit_mutex;
    std::shared_ptr<spdlog::logger> m_logger;
    Gui *m_gui;
};


#endif //QUANTUMBIT_H
