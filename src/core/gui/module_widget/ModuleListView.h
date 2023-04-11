/**
   *******************************************
   * @file      : ModuleListView.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULELISTVIEW_H
#define MODULELISTVIEW_H

#include "QListView"

class ModuleListView : public QListView {
Q_OBJECT
public:
    ModuleListView(QWidget *parent = nullptr);

    ~ModuleListView() override;

    void mouseMoveEvent(QMouseEvent *e) override;

    void leaveEvent(QEvent *event) override;
};


#endif //MODULELISTVIEW_H
