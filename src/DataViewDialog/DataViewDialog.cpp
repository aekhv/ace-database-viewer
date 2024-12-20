/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "DataViewDialog.h"
#include "ui_DataViewDialog.h"
#include <QFileIconProvider>
#include <QMenuBar>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>

DataViewDialog::DataViewDialog(const QString &fname,
                               bool plainText,
                               const QByteArray &rawData,
                               const QByteArray &rawProfile,
                               QWidget *parent)
    : QDialog(parent),
    ui(new Ui::DataViewDialog),
    m_fname(fname),
    m_rawData(rawData)
{
    ui->setupUi(this);

    // Remove "?" button from window title
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Window title
    setWindowTitle("File Viewer: " + m_fname);

    // Window icon
    QFileInfo fileInfo(m_fname);
    QFileIconProvider iconProvider;
    setWindowIcon(iconProvider.icon(fileInfo));

    // Buttons
    connect(ui->exportButton, &QPushButton::clicked, this, &DataViewDialog::exportToFile);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // Monospace font definition
    QFont mainFont = QApplication::font();
    QFont monoFont = QFont("Courier", mainFont.pointSize());

    // HEX viewer widget
    m_hexEdit = new QHexEdit;
    m_hexEdit->setReadOnly(true);
    m_hexEdit->setFont(monoFont);

    // Hex viewer layout
    QVBoxLayout *vHexLayout = new QVBoxLayout;
    ui->hexPage->setLayout(vHexLayout);
    vHexLayout->setMargin(0);
    vHexLayout->addWidget(m_hexEdit);

    // Text viewer
    m_textEdit = new QPlainTextEdit;
    m_textEdit->setReadOnly(true);
    m_textEdit->setFont(monoFont);
    m_textEdit->setWordWrapMode(QTextOption::NoWrap);

    // Text viewer layout
    QVBoxLayout *vTextLayout = new QVBoxLayout;
    ui->textPage->setLayout(vTextLayout);
    vTextLayout->setMargin(0);
    vTextLayout->addWidget(m_textEdit);

    // Data type selection
    if (plainText) {
        QTextStream ts(m_rawData); // This trick also detects encoding
        m_textEdit->appendPlainText(ts.readAll());
        m_textEdit->moveCursor(QTextCursor::Start);
        ui->stackedWidget->setCurrentWidget(ui->textPage);
    } else {
        m_hexEdit->setData(m_rawData);
        ui->stackedWidget->setCurrentWidget(ui->hexPage);
    }

    // Profile model
    m_profileModel = new ProfileModel(rawProfile, this);
    ui->profileView->setModel(m_profileModel);
    ui->profileView->setWordWrap(false);
    ui->profileView->horizontalHeader()->setStretchLastSection(true);
    ui->profileView->verticalHeader()->setDefaultSectionSize(
        ui->profileView->verticalHeader()->fontMetrics().height());
    ui->profileView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // Resize first column for better view
    const int w = ui->profileView->horizontalHeader()->width();
    ui->profileView->horizontalHeader()->resizeSection(0, w * 40 / 100);

    // Context menu
    QAction *m_copyAction = new QAction("Copy as text");
    connect(m_copyAction, &QAction::triggered, this, &DataViewDialog::copyAsText);
    QAction *m_copyAllAction = new QAction("Copy all as text");
    connect(m_copyAllAction, &QAction::triggered, this, &DataViewDialog::copyAllAsText);
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(m_copyAction);
    m_contextMenu->addAction(m_copyAllAction);
    ui->profileView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->profileView, &QTableView::customContextMenuRequested, this, &DataViewDialog::contextMenuRequested);

    // Default tab
    ui->tabWidget->setCurrentWidget(ui->dataTab);
}

DataViewDialog::~DataViewDialog()
{
    delete ui;
}

void DataViewDialog::contextMenuRequested(QPoint pos)
{
    QModelIndex index = ui->profileView->indexAt(pos);

    if (!index.isValid())
        return;

    m_contextMenu->popup(ui->profileView->viewport()->mapToGlobal(pos));
}

void DataViewDialog::copyAsText()
{
    QModelIndex index = ui->profileView->selectionModel()->currentIndex();

    if (!index.isValid())
        return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(index.data().toString());
}

void DataViewDialog::copyAllAsText()
{
    QModelIndex index = ui->profileView->selectionModel()->currentIndex();

    if (!index.isValid())
        return;

    QStringList list;
    for (int row = 0; row < m_profileModel->rowCount(QModelIndex()); row++) {
        QModelIndex param = m_profileModel->index(row, 0);
        QModelIndex value = m_profileModel->index(row, 1);
        QString line = QString("%1: %2")
                           .arg(param.data().toString())
                           .arg(value.data().toString());
        list.append(line);
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(list.join(QChar::LineFeed));
}

void DataViewDialog::exportToFile()
{
    const QStringList docs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    const QString path = QFileDialog::getSaveFileName(this,
                                                      "Export",
                                                      docs.first() + QDir::separator() + m_fname);
    if (path.isEmpty())
        return;

    QFile f(path);

    if(!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "File creation error!");
        return;
    }

    f.write(m_rawData);
    f.close();
}
