//
// Created by 11851 on 2023/3/8.
//
#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"

QSpdLogProxyModel::QSpdLogProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {
    setFilterKeyColumn(-1);
}