/**
   *******************************************
   * @file      : ModuleListModel.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULELISTMODEL_H
#define MODULELISTMODEL_H

#include "QAbstractListModel"
#include "QMutex"
#include "QIcon"
#include "QList"

class ModuleListModel : public QAbstractListModel {
Q_OBJECT
public:
    explicit ModuleListModel(QObject *parent = nullptr);

    ~ModuleListModel() override=default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void appendModule(const QString &module_name);

private:
    QMutex m_mutex;
    QList<QString> m_module_names;
};


#endif //MODULELISTMODEL_H
