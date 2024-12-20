/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "TreeModel.h"
#include <QFileIconProvider>

TreeModel::TreeModel(QObject *parent) :
    QAbstractItemModel{parent},
    m_rootItem(nullptr)
{}

void TreeModel::setRootItem(TreeItem *item)
{
    beginResetModel();
    m_rootItem = item;
    endResetModel();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem = nullptr;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = nullptr;
    if (parentItem)
        childItem = parentItem->childItem(row);
    else
        return QModelIndex();

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = nullptr;

    if (childItem)
        parentItem = childItem->parentItem();
    else
        return QModelIndex();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeItem *parentItem = nullptr;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    if (parentItem)
        return parentItem->childCount();
    else
        return 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return TreeItem::headers().count();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    // Icons
    if (role == Qt::DecorationRole) {

        QFileIconProvider iconProvider;
        QFileInfo fileInfo(item->name());

        switch (index.column()) {
        case 0:
            if (item->isFoler())
                return iconProvider.icon(QFileIconProvider::Folder);
            else
                return iconProvider.icon(fileInfo);
        default:
            return QVariant();
        }

    }

    // Titles
    if (role == Qt::DisplayRole)
        return item->data(index.column());

    return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        // Header caption
        if (role == Qt::DisplayRole)
            return TreeItem::headers().value(section, QString());
        // Header alignment
        if (role == Qt::TextAlignmentRole)
            return QVariant( Qt::AlignLeft | Qt::AlignVCenter );
    }

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}
