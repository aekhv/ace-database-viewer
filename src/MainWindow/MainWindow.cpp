/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include "DataViewDialog/DataViewDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_rootItem(nullptr) // Important! No data loaded yet.
{
    ui->setupUi(this);

    setWindowTitle("Ace Database Viewer");

    // File menu actions
    connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionExportAll, &QAction::triggered, this, &MainWindow::exportAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // Help menu actions
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);

    // Tree view item activation (opens data view dialog)
    connect(ui->treeView, &QTreeView::activated, this, &MainWindow::dataView);

    // Allow drag & drop events
    setAcceptDrops(true);

    // SQL core object
    m_sqlCore = new SqlCore(this);

    // Tree model object
    m_treeModel = new TreeModel(this);
    ui->treeView->setModel(m_treeModel);

    // Resize first column for better view
    ui->treeView->header()->resizeSection(0, width() / 2);

    // Update opened database information
    updateInfoLabel();
}

MainWindow::~MainWindow()
{
    if (m_rootItem)
        delete m_rootItem;

    delete ui;
}

void MainWindow::openFile()
{
    const QStringList docs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    const QString path = QFileDialog::getOpenFileName(this,
                                                      "Open file",
                                                      docs.first(),
                                                      "Supported files (*.fdb *.pcr);;All files (*.*)");
    if (!path.isEmpty())
        open(path);
}

void MainWindow::exportAll()
{
    if (!m_rootItem || (m_sqlCore->fileCount() == 0)) {
        QMessageBox::information(this, "Export all", "There are no files to export.");
        return;
    }

    const QStringList docs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    const QString path = QFileDialog::getExistingDirectory(this,
                                                           "Export all",
                                                           docs.first());
    if (path.isEmpty())
        return;

    bool ok = true;
    exportTreeItems(QDir(path), m_rootItem, &ok);
    if (ok)
        QMessageBox::information(this,
                                 "Information",
                                 "Completed successfully.");
    else
        QMessageBox::warning(this,
                             "Warning",
                             "Completed with errors!");
}

void MainWindow::about()
{
    QMessageBox::information(this, "About",
                             "<b>Ace Database Viewer 1.0</b><br><br>"
                             "Copyright (C) 2024 by Alexander E. &lt;aekhv@vk.com&gt;<br>"
                             "<a href=https://github.com/aekhv/ace-database-viewer>https://github.com/aekhv/ace-database-viewer</a><br><br>"
                             "Binary Editor for Qt<br>"
                             "<a href=https://github.com/simsys/qhexedit2>https://github.com/simsys/qhexedit2</a>");
}

void MainWindow::dataView(const QModelIndex &index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    if (item->isFoler())
        return;

    QByteArray rawData = m_sqlCore->rawData(item);
    QByteArray rawProfile = m_sqlCore->rawProfile(item);

    if (item->size() != rawData.size())
        QMessageBox::warning(this,
                             "Warning",
                             QString("Wrong data size detected!\nReceived: %1 bytes, expected: %2 bytes.")
                                 .arg(rawData.size())
                                 .arg(item->size()));

    DataViewDialog dialog(item->name(), // Window title
                          item->type() == TreeItem::Text, // View as hex dump or plain text
                          rawData,
                          rawProfile,
                          this);
    dialog.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        const QList<QUrl> list = event->mimeData()->urls();
        if (list.count() == 1) {
            const QString name = list.first().toLocalFile();
            if (name.endsWith(".fdb", Qt::CaseInsensitive)
                || name.endsWith(".pcr", Qt::CaseInsensitive))
                event->acceptProposedAction();
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QList<QUrl> list = event->mimeData()->urls();
    if (list.count() == 1) {
        const QString name = list.first().toLocalFile();
        if (name.endsWith(".fdb", Qt::CaseInsensitive)
            || name.endsWith(".pcr", Qt::CaseInsensitive))
            open(name);
    }
}

void MainWindow::open(const QString &path)
{
    if (m_rootItem) {
        m_treeModel->setRootItem(nullptr);
        delete m_rootItem;
        m_rootItem = nullptr;
    }

    if (!m_sqlCore->open(path)) {
        updateInfoLabel();
        QMessageBox::critical(this, "Error!", m_sqlCore->lastErrorMsg());
        return;
    }

    // New root item (hidden in TreeView)
    m_rootItem = new TreeItem(-1, "ROOT", nullptr);

    // First child item will be named as file name
    QFileInfo info(path);
    TreeItem *dbNameItem = new TreeItem(0, info.completeBaseName(), m_rootItem);
    m_rootItem->append(dbNameItem);

    // Database tree enumeration
    m_sqlCore->enumerate(dbNameItem);

    m_treeModel->setRootItem(m_rootItem);
    QModelIndex rootIndex = m_treeModel->index(0, 0, QModelIndex());
    ui->treeView->setCurrentIndex(rootIndex);
    ui->treeView->expand(rootIndex);

    updateInfoLabel();
}

void MainWindow::updateInfoLabel()
{
    if (!m_rootItem) {
        ui->infoLabel->setText("No information available");
        return;
    }

    const QStringList units = QStringList() << "bytes" << "kB" << "MB" << "GB" << "TB";
    double size = (double)m_sqlCore->totalSize();
    qint64 n = m_sqlCore->totalSize();
    int i = 0;

    for (; n >= 1024; n /= 1024, i++)
        size /= 1024.0;

    QString info = QString("Files: %1, folders: %2, total size: %3 %4")
                       .arg(m_sqlCore->fileCount())
                       .arg(m_sqlCore->folderCount())
                       .arg(size, 0, 'f', i < 1 ? 0 : 2)
                       .arg(units.at(i));

    ui->infoLabel->setText(info);
}

void MainWindow::exportTreeItems(QDir dir, TreeItem *parent, bool *ok)
{
    for (int i = 0; i < parent->childCount(); i++) {
        TreeItem *child = parent->childItem(i);
        if (child->isFoler()) {
            if (dir.mkdir(child->name())) {
                dir.cd(child->name());
                // Recursion to export subdirs
                exportTreeItems(dir, child, ok);
                dir.cdUp();
            } else
                *ok = false;
        } else {
            QFile f(dir.absolutePath() + QDir::separator() + child->name());
            if (f.open(QIODevice::WriteOnly)) {
                if (f.write(m_sqlCore->rawData(child)) != child->size())
                    *ok = false;
                f.close();
            } else
                *ok = false;
        }
    }
}
