/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef SQLCORE_H
#define SQLCORE_H

#include <QObject>
#include <QtSql>
#include "TreeItem/TreeItem.h"

class SqlCore : public QObject
{
    Q_OBJECT
public:
    explicit SqlCore(QObject *parent = nullptr);
    QString lastErrorMsg() const { return m_db.lastError().databaseText(); }
    bool open(const QString &path);
    void enumerate(TreeItem *parentItem);
    QByteArray rawData(TreeItem *item);
    QByteArray rawProfile(TreeItem *item);

    // Statistics
    int fileCount() { return m_fileCounter; }
    int folderCount() { return m_folderCounter; }
    qint64 totalSize() { return m_totalSize; }

private:
    int m_fileCounter, m_folderCounter;
    qint64 m_totalSize;
    QSqlDatabase m_db;
    QByteArray blobData(int id, const QString &blobName);
};

#endif // SQLCORE_H
