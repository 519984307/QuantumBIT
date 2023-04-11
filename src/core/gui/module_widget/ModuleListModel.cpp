/**
   *******************************************
   * @file      : ModuleListModel.cpp
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#include "ModuleListModel.h"
#include "QPair"

ModuleListModel::ModuleListModel(QObject *parent) : QAbstractListModel(parent) {

}

int ModuleListModel::rowCount(const QModelIndex &parent) const {
    return m_module_names.size();
}

QVariant ModuleListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    auto row = index.row();
    if (row >= m_module_names.size()) {
        return {};
    }
    if (role == Qt::DisplayRole) {

    }
    return {};
}

Qt::ItemFlags ModuleListModel::flags(const QModelIndex &index) const {
    return Qt::ItemNeverHasChildren | Qt::ItemIsEnabled;
}

void ModuleListModel::appendModule(const QString &module_name) {


}
