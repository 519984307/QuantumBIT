//
// Created by 11851 on 2023/3/8.
//
#include "Core/qspdlog/qabstract_spdlog_toolbar.hpp"
#include "Core/qspdlog/qspdlog.hpp"

QAbstractSpdLogToolBar::~QAbstractSpdLogToolBar() {
    if(m_parent){
        m_parent->removeToolbar(this);
        m_parent= nullptr;
    }
}

void QAbstractSpdLogToolBar::setParent(QSpdLog *parent) {
    m_parent=parent;
}
