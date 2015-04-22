#-------------------------------------------------
#
# Project created by QtCreator 2012-11-09T12:59:15
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = trackbuilder
TEMPLATE = lib

DEFINES += TRACKBUILDER_LIBRARY

mac {
    QMAKE_LIBDIR += /Users/ercanakyurek/Desktop/build-nlstudiolib-Qt_4_8_4_System-Debug/
    INCLUDEPATH += /Users/ercanakyurek/Desktop/nlstudio/ \
                   /System/Library/Frameworks/OpenGL.framework/Headers \
                   /Users/ercanakyurek/Desktop/build-nlstudiolib-Qt_4_8_4_System-Debug/

    LIBS += -framework OpenGL -framework Foundation -framework Cocoa -rdynamic -lnolimitsstudio -larchive
}

win32 {
    #QMAKE_LFLAGS += -shared
    #CONFIG+= exceptions rtti dll
    #INCLUDEPATH += "C:\Users\Ercan\Desktop\nlstudio\"
    #QMAKE_CXXFLAGS += -I"C:\Users\Ercan\Desktop\nlstudio" -I"C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Release"
    #QMAKE_LIBDIR += "C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Release"
    #LIBS += -lgdi32 -luser32 -lkernel32 -lnolimitsstudio -lglu32 -lopengl32 -lglew32

    QMAKE_LFLAGS += -shared
    CONFIG+= exceptions rtti dll
    INCLUDEPATH += "C:\Users\Ercan\Desktop\nlstudio\"
    QMAKE_CXXFLAGS += -I"C:\Users\Ercan\Desktop\nlstudio" -I"C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Debug"
    QMAKE_LIBDIR += "C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Debug"
    LIBS += -lgdi32 -luser32 -lkernel32 -lnolimitsstudio -lglu32 -lopengl32 -lglew32

    #QMAKE_LFLAGS += -shared
    #CONFIG+= exceptions rtti dll
    #INCLUDEPATH += "C:\Users\Ercan\angepasst\nlstudio\"
    #QMAKE_CXXFLAGS += -I"C:\Users\Ercan\angepasst\nlstudio" -I"C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Debug"
    #QMAKE_LIBDIR += "C:\Users\Ercan\Desktop\nlstudio-build-Qt_4_8_3-Debug"
    #LIBS += -lgdi32 -luser32 -lkernel32 -lnolimitsstudio -lglu32 -lopengl32 -lglew32
}

HEADERS += \
    trackbuilder.h \
    qWidgets/qcustomplot.h \
    qWidgets/qfvdcurve.h \
    qWidgets/qpointplot.h \
    qWidgets/qsectionlist.h \
    sections/section.h \
    sections/sectionfvd/sectionfvd.h \
    sections/sectioninital/sectioninitial.h \
    sections/sectioninital/qsectioninital.h \
    sections/sectionlist.h \
    sections/sectionfvd/qsectionfvd.h \
    renderer/rendererTrackBuilder.h \
    sections/qsection.h \
    segment/segmentsettings.h \
    segment/segmentstation.h \
    common/vertexbufferobjectextended.h \
    trackbuilder_global.h \
    common/eventhandler.h \
    sections/sectiongeometric/sectiongeometric.h \
    sections/sectiongeometric/qsectiongeometric.h \
    qWidgets/exportoptions.h \
    common/saveloadhandler.h

SOURCES += \
    trackbuilder.cpp \
    qWidgets/qcustomplot.cpp \
    qWidgets/qfvdcurve.cpp \
    qWidgets/qpointplot.cpp \
    qWidgets/qsectionlist.cpp \
    sections/section.cpp \
    sections/sectionfvd/sectionfvd.cpp \
    sections/sectioninital/sectioninitial.cpp \
    sections/sectioninital/qsectioninital.cpp \
    sections/sectionlist.cpp \
    sections/sectionfvd/qsectionfvd.cpp \
    renderer/rendererTrackBuilder.cpp \
    sections/sectionfvd/sectionfvdquaternionrolllength.cpp \
    sections/sectionfvd/sectionfvdquaternionrolltime.cpp \
    sections/qsection.cpp \
    segment/segmentsettings.cpp \
    segment/segmentstation.cpp \
    common/vertexbufferobjectextended.cpp \
    common/eventhandler.cpp \
    sections/sectionfvd/sectionfvdeulerrolllength.cpp \
    sections/sectiongeometric/sectiongeometric.cpp \
    sections/sectiongeometric/qsectiongeometric.cpp \
    sections/sectiongeometric/sectiongeometricquaternion.cpp \
    qWidgets/exportoptions.cpp \
    common/saveloadhandler.cpp

FORMS += \
    qWidgets/qfvdcurve.ui \
    qWidgets/qpointplot.ui \
    qWidgets/qsectionlist.ui \
    sections/sectioninital/qsectioninital.ui \
    sections/sectionfvd/qsectionfvd.ui \
    sections/qsection.ui \
    segment/segmentsettings.ui \
    segment/segmentstation.ui \
    sections/sectiongeometric/qsectiongeometric.ui \
    qWidgets/exportoptions.ui

RESOURCES += \
    trackbuilder.qrc \
    models/models.qrc

OTHER_FILES += \
    codefragments.txt
