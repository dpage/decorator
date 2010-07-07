///////////////////////////////////////////////////////////////////////////////
//
// ConfigWindow class: Handles the configuration
// Copyright (C) Dave Page, 2010
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QtGui/QDialog>

namespace Ui
{
    class ConfigWindow;
}

class ConfigWindow : public QDialog
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();

private:
    Ui::ConfigWindow *ui;

private slots:
    void AddFolder();
    void RemoveFolder();
    void FolderSelected();
    void Ok();
    void Cancel();
};

#endif // CONFIGWINDOW_H
