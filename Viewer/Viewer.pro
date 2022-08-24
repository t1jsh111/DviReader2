QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    documentwidget.cpp \
    dvidocument.cpp \
    dvidocumentpage.cpp \
    fontmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    resolutionmanager.cpp

HEADERS += \
    documentwidget.h \
    dvidocument.h \
    dvidocumentpage.h \
    fontmanager.h \
    mainwindow.h \
    resolutionmanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DviInterpreter/release/ -lDviInterpreter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DviInterpreter/debug/ -lDviInterpreter
else:unix: LIBS += -L$$OUT_PWD/../DviInterpreter/ -lDviInterpreter

INCLUDEPATH += $$PWD/../DviInterpreter
DEPENDPATH += $$PWD/../DviInterpreter

INCLUDEPATH  += /usr/local/opt/freetype/include/freetype2
LIBS += -lfreetype
LIBS += -L/usr/local/opt/freetype/lib -lfreetype

OTHER_FILES += $$PWD/Resources/Fonts
macx: {
APP_QML_FILES.files = $$OTHER_FILES
APP_QML_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += APP_QML_FILES
}
