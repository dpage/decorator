///////////////////////////////////////////////////////////////////////////////
//
// Decorator class: Handles the timer, tray and screen decoration
// Copyright (C) Dave Page, 2010
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DECORATOR_H
#define DECORATOR_H

#include <QtGui>

class Decorator : public QWidget
{
    Q_OBJECT

public:
    Decorator();

private:
    void createActions();
    void createTrayIcon();

    int getTimeout();

    QAction *updateAction;
    QAction *configAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QTimer *timer;

private slots:
    void Decorate();
    void Configure();
    void Quit();
};

#endif // DECORATOR_H
