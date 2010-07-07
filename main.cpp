///////////////////////////////////////////////////////////////////////////////
//
// Main: Handles the program entry
// Copyright (C) Dave Page, 2010
//
///////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include "decorator.h"
#include "configwindow.h"

int main(int argc, char *argv[])
{
    // Setup the app
    Q_INIT_RESOURCE(Decorator);
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // Run the decorator
    new Decorator();

    // KTHXBYE
    return a.exec();
}
