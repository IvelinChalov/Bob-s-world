#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T11:30:24
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES += \
    conrolsdialog.cpp \
    custombutton.cpp \
    deletesystem.cpp \
    dialog.cpp \
    entity.cpp \
    errorsystem.cpp \
    form2.cpp \
    gameform.cpp \
    glwidget.cpp \
    loadlevelsystem.cpp \
    main.cpp \
    mainwindow.cpp \
    mycontactlistener.cpp \
    popupmenu.cpp \
    savelevelsystem.cpp \
    source.cpp \
    stackedwidget.cpp \
    startform.cpp \
    testframe.cpp

HEADERS  += \
    component.h \
    conrolsdialog.h \
    custombutton.h \
    deletesystem.h \
    dialog.h \
    Entity.h \
    errorsystem.h \
    form2.h \
    gameform.h \
    glwidget.h \
    loadlevelsystem.h \
    mainwindow.h \
    mycontactlistener.h \
    popupmenu.h \
    savelevelsystem.h \
    stackedwidget.h \
    startform.h \
    testframe.h

FORMS    += \
    conrolsdialog.ui \
    dialog.ui \
    form2.ui \
    frame.ui \
    gameform.ui \
    mainwindow.ui \
    popupmenu.ui \
    stackedwidget.ui \
    startform.ui \
    testframe.ui

LIBS += -L"D:\Box2D_v2.3.0\Box2D\Build\Box2D" -lBox2D
LIBS += -L"C:\freetype-2.6.2\BUILD" -lfreetype

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

DESTDIR = $$PWD #Changes the exe location

RESOURCES += \
    pictures.qrc
