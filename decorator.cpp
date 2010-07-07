///////////////////////////////////////////////////////////////////////////////
//
// Decorator class: Handles the timer, tray and screen decoration
// Copyright (C) Dave Page, 2010
//
///////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include <QApplication>

#include <windows.h>

#include "decorator.h"
#include "configwindow.h"

//
// Constructor
//
Decorator::Decorator()
: QWidget(NULL)
{
    // Create the menu actions
    createActions();

    // Create the tray icon
    createTrayIcon();

    // Set up the timer
    timer = new QTimer(this);
    timer->setSingleShot(TRUE);
    connect(timer, SIGNAL(timeout()), this, SLOT(Decorate()));

    // Decorate the desktop
    Decorate();
}

//
// Create the menu actions
//
void Decorator::createActions()
{
    // Update
    updateAction = new QAction(tr("&Update Wallpaper"), this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(Decorate()));

    // Configure
    configAction = new QAction(tr("&Configure Decorator"), this);
    connect(configAction, SIGNAL(triggered()), this, SLOT(Configure()));

    // Quit
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(Quit()));
}

//
// Create the tray icon
//
void Decorator::createTrayIcon()
{
    // Get some randomness
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    // Check we can find the system tray
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(0, tr("Decorator"), tr("The system tray could not be found on this system. Closing Decorator."));
        QCoreApplication::exit(-1);
    }

    // Build the menu
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(updateAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(configAction);
    trayIconMenu->addAction(quitAction);

    // Create the tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    // Setup the icon itself. For convenience, we'll also use it for the dialogue.
    QIcon icon(":DecoratorTray.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->show();
}

//
// Get the timeout value
//
int Decorator::getTimeout()
{
   QSettings settings("HKEY_CURRENT_USER", QSettings::NativeFormat);

   QString key = "SOFTWARE/Decorator/Timeout";
   return settings.value(key, 5).toInt();
}

//
// Process the Decorate slot
//
void Decorator::Decorate()
{
    QString title;
    QString msg;

    // Get the configured folders
    QSettings settings("HKEY_CURRENT_USER", QSettings::NativeFormat);

    QString key = "SOFTWARE/Decorator/Folders";
    QStringList folders = settings.value(key, getenv("SystemRoot")).toString().split(";");
    QStringList fileList;

    // Supported file types
    QStringList filters;
    filters.append("*.bmp");
    filters.append("*.jpg");
    filters.append("*.png");
    filters.append("*.gif");

    for (int i = 0; i < folders.size(); ++i)
    {
        if (!folders.at(i).isEmpty())
        {
            QDir dir(folders.at(i));
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            dir.setNameFilters(filters);

            QFileInfoList files = dir.entryInfoList();

            for (int i = 0; i < files.size(); ++i)
            {
                QFileInfo fileInfo = files.at(i);
                fileList.append(fileInfo.absoluteFilePath().replace("/", "\\"));
            }
        }
    }

    // Get a random file
    QString file = fileList.at(qrand() % fileList.count());

    // Do we need to convert the file?
    if (!file.endsWith(".bmp"))
    {
        QString tmpFile = getenv("TEMP");
        tmpFile += "Decorator.bmp";

        QImage image(file);
        QImageWriter writer(tmpFile);
        writer.setFormat("bmp");
        writer.write(image);

        file = tmpFile;
    }

    // Display the file
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)file.utf16(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);

    // Schedule another re-decoration
    timer->start(getTimeout() * 60000);
}

//
// Process the Configure slot
//
void Decorator::Configure()
{
    if (timer->isActive())
        timer->stop();

    // Show the config dialogue
    ConfigWindow configWindow(this);
    configWindow.exec();

    // Schedule another re-decoration
    timer->start(getTimeout() * 60000);
}

//
// Process the Quit slot
//
void Decorator::Quit()
{
    // Kill the timer
    if (timer->isActive())
        timer->stop();

    // Kill the icon
    if (trayIcon)
        trayIcon->hide();

    // Kill the app
    QCoreApplication::exit(-1);
}
