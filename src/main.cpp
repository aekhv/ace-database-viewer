/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "MainWindow/MainWindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/database.ico"));

    MainWindow w;
    w.show();

    QCommandLineParser parser;
    parser.process(app);

    QStringList args = parser.positionalArguments();
    if (!args.isEmpty())
        w.open(args.first());

    return app.exec();
}
