//
// Created by 11851 on 2023/3/8.
//

#ifndef QSPDLOG_QSPDLOG_TOOLBAR_HPP
#define QSPDLOG_QSPDLOG_TOOLBAR_HPP

#include "QToolBar"
#include "qspdlog/qabstract_spdlog_toolbar.hpp"

class QWidget;
class QAction;
class QCompleter;
class QAbstractItemModel;
class QSettings;

class QSpdLogToolBar:public QToolBar, public QAbstractSpdLogToolBar{
    Q_OBJECT
public:
    struct FilteringSettings{
        QString text; // The text to filter by
        bool isRegularExpression; // Whether the text is a regular expression
        bool isCaseSensitive; // Whether the filter is case-sensitive
    };
public:
    QSpdLogToolBar(QWidget* parent= nullptr);
    ~QSpdLogToolBar();

#pragma region QAbstractSpdlogToolBar
    QLineEdit* filter()override;
    QAction* caseSensitive()override;
    QAction* regex()override;
    QAction* clearHistory()override;
    QAction* style()override;
    QComboBox* autoScrollPolicy()override;

#pragma endregion

    FilteringSettings filteringSettings()const;
    void checkInputValidity();
    void clearCompleterHistory();

    signals:
    void styleChangeRequested();
    void filterChanged();
    void autoScrollPolicyChanged(int index);

private:
    void loadCompleterHistory();
    void saveCompleterHistory();
private:
    QWidget* m_filterWidget;
    QAction* m_caseAction;
    QAction* m_regexAction;
    QAction* m_clearHistory;
    QAction* m_styleAction;
    QComboBox* m_autoScrollPolicy;
    QAbstractItemModel* m_completerData;
    QCompleter* m_completer;
};


#endif //QSPDLOG_QSPDLOG_TOOLBAR_HPP
