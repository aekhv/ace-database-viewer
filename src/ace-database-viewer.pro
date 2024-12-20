QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataViewDialog/DataViewDialog.cpp \
    ProfileItem/ProfileItem.cpp \
    ProfileModel/ProfileModel.cpp \
    TreeModel/TreeModel.cpp \
    main.cpp \
    MainWindow/MainWindow.cpp \
    SqlCore/SqlCore.cpp \
    TreeItem/TreeItem.cpp

HEADERS += \
    DataViewDialog/DataViewDialog.h \
    MainWindow/MainWindow.h \
    ProfileItem/ProfileItem.h \
    ProfileModel/ProfileModel.h \
    SqlCore/SqlCore.h \
    TreeItem/TreeItem.h \
    TreeModel/TreeModel.h

FORMS += \
    DataViewDialog/DataViewDialog.ui \
    MainWindow/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS += \
    icons/database.ico

INCLUDEPATH += $$PWD/../../qhexedit2-master/src

LIBS += -L$$PWD/../../qhexedit2-master/lib -llibqhexedit4

VERSION = "1.0.0.0"
QMAKE_TARGET_PRODUCT = "Ace Database Viewer"
QMAKE_TARGET_DESCRIPTION = "ACE Lab (R) PC-3000 database viewer & extractor"
