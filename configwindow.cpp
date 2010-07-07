///////////////////////////////////////////////////////////////////////////////
//
// ConfigWindow class: Handles the configuration
// Copyright (C) Dave Page, 2010
//
///////////////////////////////////////////////////////////////////////////////

#include "configwindow.h"
#include "ui_configwindow.h"

#include <QtGui>
#include <QApplication>
#include <QFileDialog>

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);

    //Connect the Signals to Slots
    connect(ui->btnAddFolder, SIGNAL(pressed()), this, SLOT(AddFolder()));
    connect(ui->btnRemoveFolder, SIGNAL(pressed()), this, SLOT(RemoveFolder()));
    connect(ui->btnOk, SIGNAL(pressed()), this, SLOT(Ok()));
    connect(ui->btnCancel, SIGNAL(pressed()), this, SLOT(Cancel()));
    connect(ui->lstFolders, SIGNAL(itemSelectionChanged()), this, SLOT(FolderSelected()));

    // Get the current settings

    // Timeout
    QSettings settings("HKEY_CURRENT_USER", QSettings::NativeFormat);

    QString key = "SOFTWARE/Decorator/Timeout";
    ui->sbTimeout->setValue(settings.value(key, 5).toInt());

    // Folders
    key = "SOFTWARE/Decorator/Folders";
    QStringList folders = settings.value(key, getenv("SystemRoot")).toString().split(";");

    for (int i = 0; i < folders.size(); ++i)
    {
        if (!folders.at(i).isEmpty())
            new QListWidgetItem(folders.at(i), ui->lstFolders);
    }

    // Enable/disable the Remove Folder button
    FolderSelected();
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

//
// Add a folder to the list
//
void ConfigWindow::AddFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Select an image directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly);

    if (!folder.isEmpty())
        new QListWidgetItem(folder.replace("/", "\\"), ui->lstFolders);
}

//
// Remove a folder from the list
//
void ConfigWindow::RemoveFolder()
{
    QListWidgetItem *selection = ui->lstFolders->currentItem();

    if (selection)
        delete selection;
}

//
// Enable/disable the Remove Folder button
void ConfigWindow::FolderSelected()
{
    QListWidgetItem *selection = ui->lstFolders->currentItem();

    if (!selection)
        ui->btnRemoveFolder->setEnabled(false);
    else
        ui->btnRemoveFolder->setEnabled(true);
}

//
// Accept the changes
//
void ConfigWindow::Ok()
{
    // Write the timeout
    QSettings settings("HKEY_CURRENT_USER", QSettings::NativeFormat);

    QString key = "SOFTWARE/Decorator/Timeout";
    settings.setValue(key, ui->sbTimeout->value());

    // Write the folders
    QString folders;
    for(int i=0; i < ui->lstFolders->count(); i++)
    {
        QListWidgetItem *folder = ui->lstFolders->item(i);
        if (!folder->text().isEmpty())
            folders += folder->text() + ";";
    }

    key = "SOFTWARE/Decorator/Folders";
    settings.setValue(key, folders);

    // All done
    accept();
}

//
// Cancel the changes
//
void ConfigWindow::Cancel()
{
    reject();
}
