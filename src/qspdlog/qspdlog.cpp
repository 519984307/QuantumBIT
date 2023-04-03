//
// Created by 11851 on 2023/3/8.
//
#include "QAction"
#include "QComboBox"
#include "QHBoxLayout"
#include "QHeaderView"
#include "QLineEdit"
#include "QMenu"
#include "QScrollBar"
#include "QTreeView"
#include "spdlog/logger.h"

#include "qspdlog/qspdlog.hpp"
#include "qspdlog/qabstract_spdlog_toolbar.hpp"
#include "qspdlog_model.hpp"
#include "qspdlog_proxy_model.hpp"
#include "qt_logger_sink.hpp"
#include "qspdlog_style_dialog.hpp"

QSpdLog::QSpdLog(QWidget *parent):
QWidget(parent),
m_sourceModel(new QSpdLogModel),
m_proxyModel(new QSpdLogProxyModel),
m_view(new QTreeView){
    m_view->setModel(m_proxyModel);
    m_view->setObjectName("qspdlogTreeView");

    QHeaderView* header=m_view->header();
    header->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
            header,
            &QHeaderView::customContextMenuRequested,
            this,
            [this,header](const QPoint& pos){
              QMenu contextMenu;
              contextMenu.setObjectName("qspdlogHeaderContextMenu");
              for(int i=0;i<m_sourceModel->columnCount();++i){
                  QString columnHeader=m_sourceModel->headerData(i, Qt::Horizontal).toString();
                  QAction* action=contextMenu.addAction(columnHeader);
                  action->setCheckable(true);
                  action->setChecked(!header->isSectionHidden(i));
                  action->setData(i);
                  connect(
                          action,
                          &QAction::toggled,
                          this,
                          [this, header](bool checked){
                              QAction* action= qobject_cast<QAction*>(sender());
                              if(action){
                                  header->setSectionHidden(action->data().toInt(), !checked);
                              }
                          }
                          );
              }
              contextMenu.exec(header->mapToGlobal(pos));
            }
            );
    m_proxyModel->setSourceModel(m_sourceModel);
    connect(
            m_sourceModel,
            &QAbstractItemModel::rowsAboutToBeInserted,
            this,
            [this](const QModelIndex& parent, int first, int last){
                auto bar=m_view->verticalScrollBar();
                m_scrollsAtBottom= bar != nullptr && (bar->value() == bar->maximum());
            }
            );
    m_view->setRootIsDecorated(false);
    m_sink=std::make_shared<qt_logger_sink_mt>(m_sourceModel);
    setLayout(new QHBoxLayout);
    layout()->setContentsMargins(0,0,0,0);
    layout()->addWidget(m_view);
}

QSpdLog::~QSpdLog() {
    std::static_pointer_cast<qt_logger_sink_mt>(m_sink)-> invalidate();
}

void QSpdLog::clear() {m_sourceModel->clear();}

void QSpdLog::registerToolbar(QAbstractSpdLogToolBar *toolbar) {
    toolbar->setParent(this);
    m_toolbars.push_back(toolbar);
    QLineEdit* filter=toolbar->filter();
    QAction* regex=toolbar->regex();
    QAction* caseSensitive=toolbar->caseSensitive();
    auto style=toolbar->style();
    auto autoScrollPolicyCombo=toolbar->autoScrollPolicy();

    auto updateFilter=[this, filter,regex,caseSensitive](){
        filterData(filter->text(), regex->isChecked(), caseSensitive->isChecked());
    };

    connect(
            filter, &QLineEdit::textChanged,this,updateFilter
            );
    connect(regex,&QAction::toggled, this, updateFilter);
    connect(caseSensitive, &QAction::toggled, this, updateFilter);
    connect(style, &QAction::toggled, this, [this](){
        QSpdLogStyleDialog dialog;
        dialog.setModel(m_sourceModel);
        dialog.setObjectName("qspdlogStyleDialog");
        if(!dialog.exec())return ;
        QSpdLogStyleDialog::Style value=dialog.result();
        m_sourceModel->setLoggerBackground(value.loggerName, value.backgroundColor);
        m_sourceModel->setLoggerForeground(value.loggerName, value.textColor);
    });
    connect(autoScrollPolicyCombo,QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QSpdLog::updateAutoScrollPolicy);
}

void QSpdLog::removeToolbar(QAbstractSpdLogToolBar *toolbar) {
    m_toolbars.erase(
            std::remove(m_toolbars.begin(), m_toolbars.end(), toolbar),
            m_toolbars.end()
            );
}

void QSpdLog::filterData(const QString &text, bool isRegularExpression, bool isCaseSensitive) {
    m_proxyModel->setFilterCaseSensitivity(isCaseSensitive?Qt::CaseSensitive:Qt::CaseInsensitive);
    if(isRegularExpression){
        QRegularExpression regex(text);
        if(!regex.isValid()){
            return;
        }
        m_proxyModel->setFilterRegularExpression(regex);
    }
    else{
        m_proxyModel->setFilterFixedString(text);
    }
}

void QSpdLog::setAutoScrollPolicy(AutoScrollPolicy policy) {
    QObject::disconnect(m_scrollConnection);
    switch (policy) {
        case AutoScrollPolicy::AutoScrollPolicyEnabled:{
            m_scrollConnection=connect(
                    m_sourceModel,
                    &QSpdLogModel::rowsInserted,
                    m_view,
                    &QTreeView::scrollToBottom
                    );
            break;
        }
        case AutoScrollPolicy::AutoScrollPolicyEnabledBottom:{
            m_scrollConnection=connect(
                    m_sourceModel,
                    &QSpdLogModel::rowsInserted,
                    this,
                    [this](){
                        if(m_scrollsAtBottom){
                            m_view->scrollToBottom();
                        }
                    }
                    );
            break;
        }
        default:
            break;
    }
    for(auto& toolbar:m_toolbars){
        QComboBox* policyComboBox=toolbar->autoScrollPolicy();
        if(!policyComboBox)continue;
        auto blocked=policyComboBox->blockSignals(true);
        policyComboBox->setCurrentIndex(static_cast<int>(policy));
        policyComboBox->blockSignals(blocked);
    }
}

void QSpdLog::updateAutoScrollPolicy(int index) {
    AutoScrollPolicy policy=static_cast<AutoScrollPolicy>(index);
    setAutoScrollPolicy(policy);
}

spdlog::sink_ptr QSpdLog::sink() {return m_sink;}

std::size_t QSpdLog::itemsCount() const {
    return static_cast<std::size_t>(m_proxyModel->rowCount());
}

void QSpdLog::setMaxEntries(std::optional<std::size_t> maxEntries) {
    m_sourceModel->setMaxEntries(maxEntries);
}

std::optional<std::size_t> QSpdLog::getMaxEntries() const {
    return m_sourceModel->getMaxEntries();
}

void QSpdLog::setLoggerForeground(std::string_view loggerName, std::optional<QColor> brush) {
    m_sourceModel->setLoggerForeground(loggerName, brush);
}

std::optional<QColor> QSpdLog::getLoggerForeground(std::string_view loggerName) const {
    return m_sourceModel->getLoggerForeground(loggerName);
}

void QSpdLog::setLoggerBackground(std::string_view loggerName, std::optional<QBrush> brush) {
    m_sourceModel->setLoggerBackground(loggerName, brush);
}

std::optional<QBrush> QSpdLog::getLoggerBackground(std::string_view loggerName) const {
    return m_sourceModel->getLoggerBackground(loggerName);
}

