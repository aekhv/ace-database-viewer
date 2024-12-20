/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QtCore>

class TreeItem
{
public:
    enum DataType { RawData, Firmware, RomDump, RamDump, Adaptives, Track, Text, InternalData };

    // Creates new folder item
    explicit TreeItem(int id,
                      const QString &name,
                      TreeItem *parentItem);
    // Creates new file item
    explicit TreeItem(int id,
                      int size,
                      DataType type,
                      const QString &name,
                      const QDateTime &ctime,
                      TreeItem *parentItem);
    ~TreeItem();

    // Appends a child item
    void append(TreeItem *child) { m_childItems.append(child); };

    TreeItem *childItem(int row) { return m_childItems.value(row, nullptr); }
    TreeItem *parentItem() { return m_parentItem; }

    int childCount() { return m_childItems.count(); }
    int row();

    int id() { return m_id; }
    int size() { return m_size; }
    DataType type() { return m_type; }
    QString name() { return m_name; }
    QDateTime ctime() { return m_ctime; }
    bool isFoler() { return m_folder; }

    static QStringList headers();
    static QString DataTypeToText(DataType type);
    QVariant data(int column);

private:
    QVector<TreeItem*> m_childItems;
    TreeItem *m_parentItem;

    int m_id;           // !!!! Source database record ID !!!
    int m_size;         // File size in bytes
    DataType m_type;    // Data type: ROM/RAM dump, firmware, track etc.
    QString m_name;     // File or folder name
    QDateTime m_ctime;  // File creation time
    bool m_folder;      // Is this item file or folder?
};

#endif // TREEITEM_H
