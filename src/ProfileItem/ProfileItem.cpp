/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#include "ProfileItem.h"

static QDateTime fromTDateTime(const double &tDateTime)
{
    // 25569.0 — January 1st, 1970 in TDateTime format
    // 86400.0 — number of seconds in a day
    qint64 secs = (qint64)((tDateTime - 25569.0) * 86400.0);
    return QDateTime::fromSecsSinceEpoch(secs , Qt::UTC);
}

static int loadRawData(QVector<ProfileItem> *vector, const char *buffer, int length)
{
    int offset = 0;

    // Profile header
    raw_profile_header_part_1_t *header_part_1 = (raw_profile_header_part_1_t *)buffer;
    offset += sizeof (raw_profile_header_part_1_t);
    if (length < offset)
    {
        return -1;
    }

    if (length < (offset + header_part_1->headerTextLength))
    {
        return -2;
    }
    QString headerName = QString::fromLatin1(buffer + offset, header_part_1->headerTextLength);
    offset += header_part_1->headerTextLength;

    if (headerName != "root")
    {
        return -3; // incorrect signature
    }

    raw_profile_header_part_2_t *header_part_2 = (raw_profile_header_part_2_t *)(buffer + offset);
    offset += sizeof (raw_profile_header_part_2_t);
    if (length < offset)
    {
        return -4;
    }

    int itemCount = header_part_2->itemCount;
    if (itemCount == 0)
        return itemCount;

    // Profile items
    while (offset < length)
    {
        // Item number
        raw_profile_item_part_1_t *item_part_1 = (raw_profile_item_part_1_t *)(buffer + offset);
        offset += sizeof (raw_profile_item_part_1_t);
        if (length < offset)
        {
            return -5;
        }

        if (length < (offset + item_part_1->numberTextLength))
        {
            return -6;
        }
        QString number = QString::fromLatin1(buffer + offset, item_part_1->numberTextLength);
        offset += item_part_1->numberTextLength;

        // Item parameter
        raw_profile_item_part_2_t *item_part_2 = (raw_profile_item_part_2_t *)(buffer + offset);
        offset += sizeof (raw_profile_item_part_2_t);
        if (length < offset)
        {
            return -7;
        }

        if (length < (offset + item_part_2->parameterTextLength))
        {
            return -8;
        }
        QString parameter = QString::fromLocal8Bit(buffer + offset, item_part_2->parameterTextLength);
        offset += item_part_2->parameterTextLength;

        // Parameter type
        raw_profile_item_part_3_t *item_part_3 = (raw_profile_item_part_3_t *)(buffer + offset);
        offset += sizeof (raw_profile_item_part_3_t);
        if (length < offset)
        {
            return -9;
        }

        quint32 *integerValue = nullptr;
        double *doubleValue = nullptr;
        qint32 *stringLength = nullptr;
        quint8 *booleanValue = nullptr;
        QVariant value;

        switch (item_part_3->parameterType) {

        case ParameterType::Integer:
            integerValue = (quint32 *)(buffer + offset);
            offset += sizeof (quint32);
            if (length < offset)
            {
                return -10;
            }
            value = *integerValue;
            break;

        case ParameterType::Double:
            doubleValue = (double *)(buffer + offset);
            offset += sizeof (double);
            if (length < offset)
            {
                return -11;
            }
            value = *doubleValue;
            break;

        case ParameterType::Date:
            doubleValue = (double *)(buffer + offset);
            offset += sizeof (double);
            if (length < offset)
            {
                return -12;
            }
            value = fromTDateTime(*doubleValue);
            break;

        case ParameterType::String:
            stringLength = (qint32 *)(buffer + offset);
            offset += sizeof (qint32);
            if (length < offset)
            {
                return -13;
            }
            if (length < (offset + (*stringLength)))
            {
                return -14;
            }
            value = QString::fromLocal8Bit(buffer + offset, *stringLength).trimmed();
            offset += *stringLength;
            break;

        case ParameterType::Boolean:
            booleanValue = (quint8 *)(buffer + offset);
            offset += sizeof (quint8);
            if (length < offset)
            {
                return -15;
            }
            value = *booleanValue ? true : false;
            break;

        default:
            value = QString("Unknown type (0x%1)")
                        .arg(item_part_3->parameterType, 4, 16, QChar('0'));
        }

        ProfileItem item;
        item.number = number.trimmed();
        item.parameter = parameter.trimmed();
        item.value = value;

        vector->append(item);
    }

    return itemCount;
}

QVector<ProfileItem> ProfileItem::fromRawData(const QByteArray &data)
{
    QVector<ProfileItem> result;
    loadRawData(&result, data.data(), data.size());
    return result;
}
