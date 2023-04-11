//
// Created by 11851 on 2023/4/10.
//

#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include "QSystemTrayIcon"
#include "QAction"

class SystemTrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
    explicit SystemTrayIcon(QObject *parent = nullptr);

    ~SystemTrayIcon();

signals:

    void showMainWindow();

    void quit();

public slots:

    void handleActivation(QSystemTrayIcon::ActivationReason);

private:
    void setUpUi();

    QAction *m_manager_action;
    QAction *m_quit_action;
    QMenu *m_left_menu;
    QMenu *m_right_menu;
};


#endif //SYSTEMTRAYICON_H
