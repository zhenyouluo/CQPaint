TEMPLATE = app

QT += widgets

TARGET = CQPaint

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11 -DGNUPLOT_EXPR -DRGB_UTIL

MOC_DIR = .moc

CONFIG += debug

# Input
SOURCES += \
CQPaintTest.cpp \

HEADERS += \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFile/include \
../../CMath/include \
../../CUtil/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CCommand/lib \
-L../../CFile/lib \
-L../../CConfig/lib \
-L../../CUtil/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../COS/lib \
-lCQPaint \
-lCQUtil -lCImageLib -lCFont -lCConfig \
-lCCommand -lCFile -lCStrUtil -lCUtil -lCOS \
-lCRegExp \
-lpng -ljpeg -ltre -lcurses -lfreetype
