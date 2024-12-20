/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "SqlCore.h"
#include <QtZlib/zlib.h>

SqlCore::SqlCore(QObject *parent)
    : QObject{parent},
    m_fileCounter(0),
    m_folderCounter(0),
    m_totalSize(0)
{
    m_db = QSqlDatabase::addDatabase("QIBASE");
}

bool SqlCore::open(const QString &path)
{
    m_fileCounter = 0;
    m_folderCounter = 0;
    m_totalSize = 0;

    if (m_db.isOpen())
        m_db.close();

    m_db.setDatabaseName(path);
    m_db.setUserName("SYSDBA");
    m_db.setPassword("masterkey");

    return m_db.open();
}

void SqlCore::enumerate(TreeItem *parentItem)
{
    if (!parentItem)
        return;

    QSqlQuery query;
    QString queryText;

    // Folder enumeration
    queryText = QString("SELECT ID,FOLDERNAME "
                        "FROM FOLDERS "
                        "WHERE PARENTID=%1;")
                    .arg(parentItem->id());
    if (!query.exec(queryText)) {
        qDebug() << query.lastError();
        return;
    }

    while (query.next())
    {
        int id = query.value("ID").toInt();
        QString name = query.value("FOLDERNAME").toString();

        // If name is empty we will use ID as name
        if (name.isEmpty())
            name = QString("%1").arg(id, 8, 16, QChar('0'));

        // qDebug() << id << name;

        // New folder item
        TreeItem *childItem = new TreeItem(id, name, parentItem);
        parentItem->append(childItem);

        m_folderCounter++;

        // Recursion to enumerate subfolders
        enumerate(childItem);
    }

    // File enumeration
    queryText = QString("SELECT ID,MODULENAME,KIND,DATASIZE,CREATEDDATE "
                        "FROM DATA "
                        "WHERE FOLDERID=%1;")
                    .arg(parentItem->id());
    if (!query.exec(queryText)) {
        qDebug() << query.lastError();
        return;
    }

    while (query.next()) {
        int id = query.value("ID").toInt();
        int size = query.value("DATASIZE").toInt();
        int type = query.value("KIND").toInt();
        QString name = query.value("MODULENAME").toString();
        QDateTime ctime = query.value("CREATEDDATE").toDateTime();

        if (name.isEmpty())
            name = QString("%1").arg(id, 8, 16, QChar('0'));

        // qDebug() << id << name << size;

        // New file item
        TreeItem *childItem = new TreeItem(id, size, (TreeItem::DataType)type, name, ctime, parentItem);
        parentItem->append(childItem);

        m_fileCounter++;
        m_totalSize += size;
    }
}

QByteArray SqlCore::rawData(TreeItem *item)
{
    // Compressed raw data
    QByteArray in = blobData(item->id(), "DATA");

    if (in.size() < (int)sizeof(unsigned long)) {
        qDebug() << "BLOB size too small!";
        return QByteArray();
    }

    // Uncompressed data length
    unsigned long *length = (unsigned long *)in.data();

    // Uncompressed raw data
    QByteArray out(*length, 0);

    // Zlib magic happens here
    int err = uncompress((uchar *)out.data(), // Destination (uncompressed) buffer
                         length,
                         (uchar *)in.data() + sizeof(unsigned long), // Source (compressed) buffer
                         in.size() - sizeof(unsigned long));

    if (err != Z_OK) {
        qDebug() << "BLOB uncompress error!";
        return QByteArray();
    }

    return out;
}

QByteArray SqlCore::rawProfile(TreeItem *item)
{
    return blobData(item->id(), "PROFILE");
}

QByteArray SqlCore::blobData(int id, const QString &blobName)
{
    QSqlQuery query;
    QString queryText;

    queryText = QString("SELECT %1 "
                        "FROM DATA "
                        "WHERE ID=%2;")
                    .arg(blobName)
                    .arg(id);
    if (!query.exec(queryText)) {
        qDebug() << query.lastError();
        return QByteArray();
    }

    if (query.first())
        return query.value(blobName).toByteArray();
    else
        return QByteArray();
}
