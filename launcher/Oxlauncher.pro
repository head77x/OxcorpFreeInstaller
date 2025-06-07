QT += core gui widgets network

CONFIG += c++17 release

INCLUDEPATH += . \
    ../../vcpkg/installed/x64-windows/include

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ConfigManager.cpp

HEADERS += \
    mainwindow.h \
    ConfigManager.h

# libzip 경로
LIBS += -Ld:/Works/vcpkg/installed/x64-windows/lib -lzip

# libzip 헤더
INCLUDEPATH += d:/Works/vcpkg/installed/x64-windows/include/zip

RC_FILE = icon.rc