/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef PROFILEMODEL_H
#define PROFILEMODEL_H

#include <QAbstractTableModel>
#include "ProfileItem/ProfileItem.h"

class ProfileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProfileModel(const QByteArray &rawProfile, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QVector<ProfileItem> m_profileList;
};

#endif // PROFILEMODEL_H
