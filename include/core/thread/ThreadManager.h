//
// Created by 11851 on 2023/4/7.
//
#include "QAbstractListModel"
#include "QRecursiveMutex"
#include "QThread"
#include "QList"
#include "optional"
#include "core/logger/logger.h"

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

class ThreadManager : public QAbstractListModel {
Q_OBJECT
public:
    static ThreadManager *getInstance();

    ~ThreadManager() override = default;

    QThread *getNewThread(const QString& name);

public slots:

    void registerThread(QThread *thread);

    void unregisterThread(const QString& name);

    void quitThread(QThread *thread);

    void quitThread(const QString &name);

    void joinThread(QThread *thread, int timeout);

    void joinThread(const QString &name, int timeout);

    void quitAllThread(int timeout);

public:
    QThread *getThreadByName(const QString &name);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;


private:
    ThreadManager();

    QRecursiveMutex m_recursive_mutex;
    QList<QThread *> m_thread;
    QList<QString> m_thread_names;
    std::shared_ptr<spdlog::logger> m_logger;
};


#endif //THREADMANAGER_H
