/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include "TreeItem/TreeItem.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr);

    void setRootItem(TreeItem *item);

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    TreeItem *m_rootItem;
};

#endif // TREEMODEL_H
