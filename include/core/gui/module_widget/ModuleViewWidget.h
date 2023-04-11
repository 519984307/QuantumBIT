/**
   *******************************************
   * @file      : ModuleViewWidget.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULEVIEWWIDGET_H
#define MODULEVIEWWIDGET_H

#include "QWidget"
#include "QButtonGroup"
#include "QVBoxLayout"

class ModuleViewWidget : public QWidget {
Q_OBJECT
public:
    explicit ModuleViewWidget(QWidget *parent = nullptr);

    ~ModuleViewWidget() override = default;

    void addModule(const QString &title, const QIcon &icon);

signals:

    void showWindow(int index);

private:
    void setUpUi();

    QButtonGroup *m_module_group;
    int m_module_count;
    QVBoxLayout *m_layout;
};


#endif //MODULEVIEWWIDGET_H
