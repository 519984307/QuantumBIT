#pragma once
class QLineEdit;
class QAction;
class QComboBox;
class QSpdLog;

class QAbstractSpdLogToolBar{
public:
    explicit QAbstractSpdLogToolBar()=default;
    virtual ~QAbstractSpdLogToolBar();

    /* set the parent*/
    void setParent(QSpdLog* parent);

    /*set the filter text editing widget, the widget is used to filter the messages
     * in the widget*/
    virtual QLineEdit* filter()=0;

    /*get the case-sensitive action. The action is used to toggle the case-sensitivity
     * of the filtering*/
    virtual QAction* caseSensitive()=0;

    /*get the regular expression action
     * The action is used to toggle the regulation expression mode of the filtering*/
    virtual QAction* regex()=0;

    /*get the clear history action*/
    virtual QAction* clearHistory()=0;
    /*Get the style action
     * The action is used to toggle the style of the widget*/
    virtual QAction* style()=0;

    /*get the auto scroll policy of combo box
     * The combo box is used to select the auto scroll policy*/
    virtual QComboBox* autoScrollPolicy()=0;

private:
    QSpdLog* m_parent;
};

extern QAbstractSpdLogToolBar* createToolBar();