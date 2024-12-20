# Ace Database Viewer

## Description
This tiny application allows to view and unpack ACE Lab PC-3000 database files (*.pcr & *.fdb).

![](/img/screenshot.png)

## Download
See latest release [here](https://github.com/aekhv/ace-database-viewer/releases).

## Discuss and bug report
[https://forum.hddguru.com/viewtopic.php?f=7&t=44656](https://forum.hddguru.com/viewtopic.php?f=7&t=44656)

## How to build
I wish I could say all you need is Qt and Firebird library... But it's not true, he-he... Unfortunately ACE Lab (R) uses too old Firebird 2.x engine for their database and this engine is 32-bit only. This means Firebird library must be 32-bit too. And this also means your Qt application also must be 32-bit. And this also means you have to use old 32-bit MinGW available in Qt5 only. Yes, you can forget about Qt6. I recommend Qt 5.15.2 + MinGW 8.1 32-bit.

### Requirements
1. Last available Qt5 + MinGW 32-bit. Don't forget about Qt sources, you'll need them too.
2. Firebird 2.x sources, for example from [there](https://www.firebirdsql.org/en/firebird-2-5-4/#Win32). All you need is `Firebird-2.5.4.26856_0_Win32.exe`, just install it with default options.
3. QHexEdit2 widget from [there](https://github.com/simsys/qhexedit2).

### Adding Firebird support to Qt
To add Firebird (IBASE) support to Qt use `make-qt-ibase.cmd` command file. Do not forget to run this file from MinGW 32-bit environment.

### QHexEdit2 widget compilation
To compile QHexEdit2 widget type following commands from MinGW 32-bit environment:
```
qmake qhexedit.pro
mingw32-make
```

## Known troubleshooting
If you see error message - "driver not loaded" - try to copy `fbclient.dll` from the Firebird binaries to the folder of your application.