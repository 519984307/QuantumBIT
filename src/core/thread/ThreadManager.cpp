//
// Created by 11851 on 2023/4/7.
//

#include "core/thread/ThreadManager.h"
#include "QMutexLocker"
#include "typeinfo"

ThreadManager *ThreadManager::getInstance() {
    static ThreadManager *instance = new ThreadManager();
    return instance;
}

ThreadManager::ThreadManager() : QAbstractListModel(), m_logger(getLogger("ThreadManager")) {
}

QThread *ThreadManager::getNewThread(const QString &name) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);

    if (std::find(m_thread_names.begin(), m_thread_names.end(), name) == m_thread_names.end()) {
        auto *new_thread = new QThread(this);
        m_logger->debug("creating thread: {}", name.toStdString());
        new_thread->setObjectName(name);
        registerThread(new_thread);
        return new_thread;
    } else {
        return nullptr;
    }

}

void ThreadManager::registerThread(QThread *thread) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    auto row = m_thread.size();
    beginInsertRows(QModelIndex(), row, row);
    m_thread.push_back(thread);
    m_thread_names.push_back(std::move(thread->objectName()));
    connect(thread, &QThread::finished, this, [=, this]() {
        this->unregisterThread(thread->objectName());
    }, Qt::QueuedConnection);

}

void ThreadManager::unregisterThread(const QString &name) {
    auto index = m_thread_names.indexOf(name);
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    auto thread = getThreadByName(name);
    if (thread->isRunning()) {
        quitThread(thread);
        return;
    }
    m_logger->debug("cleaning up thread {}", name.toStdString());
    beginRemoveRows(QModelIndex(), index, index);
    m_thread.removeAt(index);
    m_thread_names.removeAt(index);
    thread->deleteLater();
    endRemoveRows();

}

void ThreadManager::quitThread(QThread *thread) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    if (thread) {
        thread->quit();
    } else {
        m_logger->error("you tried to quitting a nonexistent thread.");
    }
}

void ThreadManager::quitThread(const QString &name) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    auto thread = getThreadByName(name);
    quitThread(thread);
}

void ThreadManager::joinThread(QThread *thread, int timeout) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    if (thread) {
        m_logger->debug("waiting for thread {} to end.", thread->objectName().toStdString());
        if (timeout == -1) {
            thread->wait();
        } else {
            thread->wait(timeout);
        }
    } else {
        m_logger->error("you tried to waiting for a nonexistent thread.");
    }
}

void ThreadManager::joinThread(const QString &name, int timeout) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    auto thread = getThreadByName(name);
    joinThread(thread, timeout);
}

void ThreadManager::quitAllThread(int timeout) {
    QMutexLocker<QRecursiveMutex> locker(&m_recursive_mutex);
    m_logger->debug("quit all threads");
    for (auto thread: m_thread) {
        thread->quit();
        if (!thread->wait(timeout)) {
            m_logger->error("waiting for thread {} time out", thread->objectName().toStdString());
        }
    }
}

QThread *ThreadManager::getThreadByName(const QString &name) {
    auto index = m_thread_names.indexOf(name);
    return m_thread.at(index);
}

int ThreadManager::rowCount(const QModelIndex &parent) const {
    return m_thread.size();
}

QVariant ThreadManager::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 0) {
        return "Thread Name";
    }
    return {};
}

QVariant ThreadManager::data(const QModelIndex &index, int role) const {
    auto row = index.row();
    if (index.isValid() && role == Qt::DisplayRole && row >= 0 && role < m_thread.size()) {
        if (index.column() == 0) {
            return m_thread_names.at(row);
        }
    }
    return {};
}

Qt::ItemFlags ThreadManager::flags(const QModelIndex &index) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


