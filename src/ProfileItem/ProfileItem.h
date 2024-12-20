/****************************************************************************
**
** This file is part of the Ace Database Viewer project.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU GPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef PROFILEITEM_H
#define PROFILEITEM_H

#include <QtCore>

class ProfileItem
{
public:
    QString number;
    QString parameter;
    QVariant value;

    static QVector<ProfileItem> fromRawData(const QByteArray &data);
};

/*************************************************/
/********* RAW profile structures BEGIN **********/
/*************************************************/

typedef struct __attribute__ ((packed)) {
    qint8 unknown[3];       // Always 0x01, 0x00, 0x00
    qint32 headerTextLength;
} raw_profile_header_part_1_t;

typedef struct __attribute__ ((packed)) {
    qint32 itemCount;
    qint32 unknown;
} raw_profile_header_part_2_t;

typedef struct __attribute__ ((packed)) {
    qint32 numberTextLength;
} raw_profile_item_part_1_t;

typedef struct __attribute__ ((packed)) {
    qint32 unknown_1;       // Always 0x00000000
    qint32 unknown_2;       // Always 0x00000002
    qint32 unknown_3;       // Always 0x00000001
    qint8  unknown_4[3];    // Always 0x4E, 0x08, 0x00
    qint32 parameterTextLength;
} raw_profile_item_part_2_t;

typedef struct __attribute__ ((packed)) {
    qint32 unknown_1;       // Always 0x00000001
    qint16 parameterType;
    qint8  unknown_2;       // Always 0x00
} raw_profile_item_part_3_t;

enum ParameterType { Integer = 0x0356,
                     Double  = 0x0556,
                     Date    = 0x0756,
                     String  = 0x0856,
                     Boolean = 0x0B56 };

/*************************************************/
/********** RAW profile structures END ***********/
/*************************************************/

#endif // PROFILEITEM_H
