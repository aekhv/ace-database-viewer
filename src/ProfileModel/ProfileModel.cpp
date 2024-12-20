/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "ProfileModel.h"
#include <QDebug>

ProfileModel::ProfileModel(const QByteArray &rawProfile, QObject *parent)
    : QAbstractTableModel{parent}
{
    m_profileList = ProfileItem::fromRawData(rawProfile);
}

int ProfileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_profileList.count();
}

int ProfileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString parameter = m_profileList.at(index.row()).parameter.toUpper();
        QVariant value = m_profileList.at(index.row()).value;

        switch (index.column()) {
        case 0:
            return parameter;
        case 1:
            if (value.type() == QVariant::UInt)
                return QString("%1 (0x%2)")
                    .arg(QString::number(value.toUInt()))
                    .arg(value.toUInt(), 8, 16, QChar('0'));
            else
                return value;
        default:
            return QString();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return QVariant( Qt::AlignLeft | Qt::AlignVCenter );
    }

    return QVariant();
}

QVariant ProfileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return QString("Parameter");
            case 1:
                return QString("Value");
            default:
                return QString();
            }
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return QVariant( Qt::AlignLeft | Qt::AlignVCenter );
    }

    return QVariant();
}
