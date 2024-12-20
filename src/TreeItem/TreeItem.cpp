/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "TreeItem.h"

TreeItem::TreeItem(int id, const QString &name, TreeItem *parentItem) :
    m_parentItem(parentItem),
    m_id(id),
    m_name(name),
    m_folder(true) // Item is a folder!
{

}

TreeItem::TreeItem(int id, int size, DataType type, const QString &name, const QDateTime &ctime, TreeItem *parentItem) :
    m_parentItem(parentItem),
    m_id(id),
    m_size(size),
    m_type(type),
    m_name(name),
    m_ctime(ctime),
    m_folder(false) // Item is a file!
{

}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

int TreeItem::row()
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
    else
        return 0;
}

QStringList TreeItem::headers()
{
    return QStringList() << "Name"
                         << "Size"
                         << "Data type"
                         << "Creation time";
}

QString TreeItem::DataTypeToText(DataType type)
{
    switch (type) {
    case DataType::RawData:
        return "RAW data";
    case DataType::Firmware:
        return "Firmware";
    case DataType::RomDump:
        return "ROM dump";
    case DataType::RamDump:
        return "RAM dump";
    case DataType::Adaptives:
        return "Adaptives";
    case DataType::Track:
        return "Track";
    case DataType::Text:
        return "Plain text";
    case DataType::InternalData:
        return "Internal data";

    default:
        return "Unknown";
    }
}

QVariant TreeItem::data(int column)
{
    if (m_folder)
        switch (column) {
        case 0:
            return m_name;

        default:
            return QVariant();
        }
    else
        switch (column) {
        case 0:
            return m_name;
        case 1:
            return QString::number(m_size);
        case 2:
            return TreeItem::DataTypeToText(m_type);
        case 3:
            return m_ctime.toString("yyyy.MM.dd hh:mm:ss");

        default:
            return QVariant();
        }
}
