/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef DATAVIEWDIALOG_H
#define DATAVIEWDIALOG_H

#include <QDialog>
#include "qhexedit.h"
#include "ProfileModel/ProfileModel.h"
#include <QPlainTextEdit>
#include <QMenu>

namespace Ui {
class DataViewDialog;
}

class DataViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewDialog(const QString &fname,
                            bool plainText,
                            const QByteArray &rawData,
                            const QByteArray &rawProfile,
                            QWidget *parent = nullptr);
    ~DataViewDialog();

private:
    Ui::DataViewDialog *ui;
    QHexEdit *m_hexEdit;
    QPlainTextEdit *m_textEdit;
    ProfileModel *m_profileModel;
    QMenu *m_contextMenu;

    QString m_fname;
    QByteArray m_rawData;

private slots:
    void contextMenuRequested(QPoint pos);
    void copyAsText();
    void copyAllAsText();
    void exportToFile();
};

#endif // DATAVIEWDIALOG_H
