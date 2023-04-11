//
// Created by 11851 on 2023/4/7.
//

#include "core/main/QuantumBIT.h"
#include "QMutexLocker"
#include "QCoreApplication"
#include "QApplication"


void QuantumBIT::run() {
    QMutexLocker<QMutex> locker(&m_run_mutex);
    if (m_is_running) {
        m_logger->error("application is running");
        return;
    }

    m_logger->info("starting application ...");



}
