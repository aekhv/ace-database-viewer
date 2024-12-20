/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SqlCore/SqlCore.h"
#include "TreeModel/TreeModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void open(const QString &path);

private slots:
    void openFile();
    void exportAll();
    void about();
    void dataView(const QModelIndex &index);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;
    SqlCore *m_sqlCore;
    TreeItem *m_rootItem;
    TreeModel *m_treeModel;

    void updateInfoLabel();
    void exportTreeItems(QDir dir, TreeItem *parent, bool *ok);
};
#endif // MAINWINDOW_H
