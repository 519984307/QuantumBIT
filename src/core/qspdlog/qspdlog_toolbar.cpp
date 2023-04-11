//
// Created by 11851 on 2023/3/8.
//


#include "QComboBox"
#include "QCompleter"
#include "QLayout"
#include "QLineEdit"
#include "QRegularExpression"
#include "QSettings"
#include "QStringListModel"

#include "qspdlog_toolbar.hpp"

QSpdLogToolBar::QSpdLogToolBar(QWidget *parent) :
        QToolBar(parent),
        m_filterWidget(new QLineEdit(this)),
        m_clearHistory(new QAction("clear History", this)),
        m_autoScrollPolicy(new QComboBox(this)),
        m_completerData(new QStringListModel(this)),
        m_completer(new QCompleter(m_completerData, this)) {
    addWidget(m_filterWidget);
    m_filterWidget->setObjectName("filterText");
    m_caseAction = addAction("Aa");
    m_caseAction->setCheckable(true);
    m_caseAction->setObjectName("caseSensitiveAction");

    m_regexAction = addAction(".*");
    m_regexAction->setCheckable(true);
    m_regexAction->setObjectName("regexAction");

    m_clearHistory->setObjectName("clearHistoryAction");

    m_styleAction = addAction("Set style");
    m_styleAction->setObjectName("styleAction");

    m_autoScrollPolicy->setObjectName("_autoScrollPolicy");
    m_autoScrollPolicy->addItems({
                                         "Manual Scroll",
                                         "Scroll To Bottom",
                                         "Smart Scroll"
                                 });
    addWidget(m_autoScrollPolicy);

    auto lineEdit = static_cast<QLineEdit *>(m_filterWidget);
    lineEdit->setPlaceholderText("Filter");

    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    lineEdit->setCompleter(m_completer);

    connect(
            lineEdit, &QLineEdit::editingFinished, this, [this]() {
                QStringListModel *model = static_cast<QStringListModel *>(m_completerData);
                QString text = static_cast<QLineEdit *>(m_filterWidget)->text();
                if (text.isEmpty() || model->stringList().contains(text)) return;

                if (model->insertRow(model->rowCount())) {
                    QModelIndex index = model->index(model->rowCount() - 1, 0);
                    model->setData(index, text);
                }
                saveCompleterHistory();
            }
    );

    connect(m_caseAction, &QAction::toggled, this, &QSpdLogToolBar::filterChanged);
    connect(m_regexAction, &QAction::toggled, this, &QSpdLogToolBar::filterChanged);
    connect(m_styleAction, &QAction::toggled, this, [this]() { emit styleChangeRequested(); });
    connect(m_autoScrollPolicy, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &QSpdLogToolBar::autoScrollPolicyChanged);
    connect(this, &QSpdLogToolBar::filterChanged, this, &QSpdLogToolBar::checkInputValidity);
    connect(m_clearHistory, &QAction::triggered, this, &QSpdLogToolBar::clearCompleterHistory);
    loadCompleterHistory();
}

QSpdLogToolBar::~QSpdLogToolBar() {}

#pragma region QAbstractSpdLogToolBar

QLineEdit *QSpdLogToolBar::filter() {
    return static_cast<QLineEdit *>(m_filterWidget);
}

QAction *QSpdLogToolBar::caseSensitive() {
    return m_caseAction;
}

QAction *QSpdLogToolBar::regex() { return m_regexAction; }

QAction *QSpdLogToolBar::clearHistory() {
    return m_clearHistory;
}

QAction *QSpdLogToolBar::style() { return m_styleAction; }

QComboBox *QSpdLogToolBar::autoScrollPolicy() { return m_autoScrollPolicy; }

#pragma endregion

QSpdLogToolBar::FilteringSettings QSpdLogToolBar::filteringSettings() const {
    return {
            static_cast<QLineEdit *>(m_filterWidget)->text(),
            m_regexAction->isChecked(),
            m_caseAction->isChecked()
    };
}

void QSpdLogToolBar::checkInputValidity() {
    FilteringSettings settings = filteringSettings();
    if (!settings.isRegularExpression) {
        m_filterWidget->setPalette(QWidget::palette());
        m_filterWidget->setToolTip("");
        return;
    }
    QRegularExpression regex(settings.text);
    if (regex.isValid()) {
        m_filterWidget->setPalette(QWidget::palette());
        m_filterWidget->setToolTip("");
        return;
    }

    QPalette palette = m_filterWidget->palette();
    palette.setColor(QPalette::Text, Qt::red);
    m_filterWidget->setPalette(palette);
    m_filterWidget->setToolTip(regex.errorString());
}

void QSpdLogToolBar::clearCompleterHistory() {
    QStringListModel *model = static_cast<QStringListModel *>(m_completerData);
    model->setStringList({});
    saveCompleterHistory();
}

void QSpdLogToolBar::loadCompleterHistory() {
    QStringListModel *model = static_cast<QStringListModel *>(m_completerData);
    model->setStringList(
            QSettings("./qspdlog_filter_history", QSettings::NativeFormat).value("completerHistory").toStringList()
    );
}

void QSpdLogToolBar::saveCompleterHistory() {
    QStringListModel* model=static_cast<QStringListModel*>(m_completerData);
    QSettings("./qspdlog_filter_history",QSettings::NativeFormat).setValue("completerHistory",model->stringList());
}

extern QAbstractSpdLogToolBar* createToolBar(){return new QSpdLogToolBar();}