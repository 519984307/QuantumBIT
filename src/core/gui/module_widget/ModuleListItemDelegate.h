/**
   *******************************************
   * @file      : ModuleListItemDelegate.h
   * @author    : yifan
   * @brief     : None
   * @attention : None
   * @date      : 2023/4/10
   ********************************************
   */


#ifndef MODULELISTITEMDELEGATE_H
#define MODULELISTITEMDELEGATE_H

#include "QStyledItemDelegate"

class ModuleListItemDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    ModuleListItemDelegate(QObject *parent = nullptr);

    ~ModuleListItemDelegate() override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif //MODULELISTITEMDELEGATE_H
