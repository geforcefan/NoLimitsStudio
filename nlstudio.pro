#-------------------------------------------------
#
# Project created by QtCreator 2012-09-28T09:14:36
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = NoLimitsStudio
TEMPLATE = app

QMAKE_CXXFLAGS_DEBUG += -O2
QMAKE_CXXFLAGS_RELEASE += -O2


greaterThan(QT_MAJOR_VERSION, 4) {
    QT *= printsupport
}


CONFIG += exceptions \
          rtti

mac {
    CONFIG += c++11

    INCLUDEPATH += /System/Library/Frameworks/OpenGL.framework/Headers \
                 /Volumes/Daten/NoLimitsTools/nlstudio/plugins/trackbuilder/ \
                /usr/local/include/
    LIBS += -framework OpenGL -framework Foundation -framework Cocoa -framework AppKit -larchive

    QMAKE_MAC_SDK = macosx10.9
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.9
    LIBS += -stdlib=libc++ -mmacosx-version-min=10.9

    OBJECTIVE_SOURCES +=

    OTHER_FILES += \
        shaders/osxMain.frag \
        shaders/osxMain.vert

    RESOURCES += \
        shaders/osxShaders.qrc
}

win32 {
    CONFIG+= exceptions rtti
    LIBS += -lopengl32 -lgdi32 -luser32 -lkernel32 -larchive
    HEADERS += \
        glew/wglew.h \
        glew/glxew.h \
        glew/glew.h
    SOURCES +=  \
        glew/glew.c
}

SOURCES += \
    window/windowmain.cpp \
    nlstudio.cpp \
    maingl/glwidget.cpp \
    maingl/glthread.cpp \
    common/camera.cpp \
    renderer/rendererenvironment.cpp \
    libnltrack/tube.c \
    libnltrack/trck.c \
    libnltrack/std.c \
    libnltrack/segm.c \
    libnltrack/rasc.c \
    libnltrack/nlim.c \
    libnltrack/libnltrack.c \
    libnltrack/info.c \
    libnltrack/fund.c \
    libnltrack/fren.c \
    libnltrack/envc.c \
    libnltrack/bezr.c \
    renderer/renderertrack.cpp \
    renderer/renderer.cpp \
    common/nlsegment.cpp \
    common/global.cpp \
    common/textures.cpp \
    common/shader.cpp \
    common/extrude.cpp \
    common/mesh.cpp \
    common/vertexbufferobject.cpp \
    common/objloader.cpp \
    common/objloaderbuffer.cpp \
    common/lights.cpp \
    maingl/glrenderer.cpp \
    renderer/renderertrack/renderertrackstyles.cpp \
    renderer/rendererfooter.cpp \
    renderer/renderersupports.cpp \
    renderer/rendererterrain.cpp \
    common/nlbanking.cpp \
    common/vectorstuff.cpp \
    common/triangulate.cpp \
    common/plugins.cpp \
    window/modeobject.cpp \
    common/savehandler.cpp \
    common/fvd.cpp \
    common/settings.cpp \
    pugixml/pugixml.cpp \
    window/windowsettings.cpp \
    plugins/trackbuilder/trackbuilder.cpp \
    plugins/trackbuilder/qWidgets/qcustomplot.cpp \
    plugins/trackbuilder/qWidgets/qfvdcurve.cpp \
    plugins/trackbuilder/qWidgets/qpointplot.cpp \
    plugins/trackbuilder/qWidgets/qsectionlist.cpp \
    plugins/trackbuilder/renderer/rendererTrackBuilder.cpp \
    plugins/trackbuilder/common/vertexbufferobjectextended.cpp \
    plugins/trackbuilder/common/eventhandler.cpp \
    plugins/trackbuilder/common/saveloadhandler.cpp \
    plugins/trackbuilder/transition/transitionmanager.cpp \
    plugins/trackbuilder/transition/transitionbasic.cpp \
    plugins/trackbuilder/transition/transition.cpp \
    plugins/trackbuilder/transition/transitionmanagerwidget.cpp \
    plugins/trackbuilder/transition/transitionbump.cpp \
    plugins/trackbuilder/transition/transitionlinear.cpp \
    plugins/trackbuilder/transition/transitionquintic.cpp \
    plugins/trackbuilder/transition/transitiongeneric.cpp \
    plugins/trackbuilder/track/mnode.cpp \
    plugins/trackbuilder/track/track.cpp \
    plugins/trackbuilder/track/section.cpp \
    plugins/trackbuilder/track/secstraight.cpp \
    plugins/trackbuilder/track/widgets/sectionwidgetstraight.cpp \
    plugins/trackbuilder/track/widgets/sectionwidget.cpp \
    plugins/trackbuilder/track/widgets/sectionwidgetinfo.cpp \
    common/camerabase.cpp \
    plugins/trackbuilder/common/trackcamera.cpp \
    plugins/trackbuilder/track/seccurved.cpp \
    plugins/trackbuilder/track/widgets/sectionwidgetcurved.cpp \
    plugins/trackbuilder/track/secforced.cpp \
    plugins/trackbuilder/track/widgets/sectionwidgetforced.cpp \
    common/nlcamera.cpp

HEADERS  += \
    window/windowmain.h \
    glm/glm.hpp \
    glm/ext.hpp \
    maingl/glwidget.h \
    maingl/glthread.h \
    common/camera.h \
    renderer/rendererenvironment.h \
    libnltrack/libnltrack.h \
    renderer/renderertrack.h \
    renderer/renderer.h \
    common/global.h \
    common/nlsegment.h \
    common/textures.h \
    common/shader.h \
    common/extrude.h \
    common/vectorstuff.h \
    common/mesh.h \
    common/vertexbufferobject.h \
    common/objloader.hpp \
    common/objloaderbuffer.h \
    common/lights.h \
    maingl/glrenderer.h \
    common/triangulate.h \
    renderer/renderertrack/renderertrackstyles.h \
    renderer/rendererfooter.h \
    renderer/renderersupports.h \
    renderer/rendererterrain.h \
    common/nlbanking.h \
    window/sidewidget.h \
    common/plugins.h \
    sdk/NoLimitsStudio.h \
    window/modeobject.h \
    common/savehandler.h \
    common/fvd.h \
    common/settings.h \
    pugixml/pugixml.hpp \
    pugixml/pugiconfig.hpp \
    window/windowsettings.h \
    plugins/trackbuilder/trackbuilder.h \
    plugins/trackbuilder/qWidgets/qcustomplot.h \
    plugins/trackbuilder/qWidgets/qfvdcurve.h \
    plugins/trackbuilder/qWidgets/qpointplot.h \
    plugins/trackbuilder/qWidgets/qsectionlist.h \
    plugins/trackbuilder/renderer/rendererTrackBuilder.h \
    plugins/trackbuilder/common/vertexbufferobjectextended.h \
    plugins/trackbuilder/trackbuilder_global.h \
    plugins/trackbuilder/common/eventhandler.h \
    plugins/trackbuilder/common/saveloadhandler.h \
    plugins/trackbuilder/transition/transitionmanager.h \
    plugins/trackbuilder/transition/transition.h \
    plugins/trackbuilder/transition/transitionbasic.h \
    plugins/trackbuilder/transition/transitionmanagerwidget.h \
    plugins/trackbuilder/transition/transitionbump.h \
    plugins/trackbuilder/transition/transitionlinear.h \
    plugins/trackbuilder/transition/transitionquintic.h \
    plugins/trackbuilder/transition/transitiongeneric.h \
    plugins/trackbuilder/track/mnode.h \
    plugins/trackbuilder/track/track.h \
    plugins/trackbuilder/track/section.h \
    plugins/trackbuilder/track/secstraight.h \
    plugins/trackbuilder/track/widgets/sectionwidgetstraight.h \
    plugins/trackbuilder/track/widgets/sectionwidget.h \
    plugins/trackbuilder/track/widgets/sectionwidgetinfo.h \
    common/camerabase.h \
    plugins/trackbuilder/common/trackcamera.h \
    plugins/trackbuilder/track/seccurved.h \
    plugins/trackbuilder/track/widgets/sectionwidgetcurved.h \
    plugins/trackbuilder/track/secforced.h \
    plugins/trackbuilder/track/widgets/sectionwidgetforced.h \
    common/nlcamera.h

FORMS    += \
    window/windowmain.ui \
    window/windowsettings.ui \
    plugins/trackbuilder/qWidgets/qfvdcurve.ui \
    plugins/trackbuilder/qWidgets/qpointplot.ui \
    plugins/trackbuilder/qWidgets/qsectionlist.ui \
    plugins/trackbuilder/transition/transitionmanagerwidget.ui \
    plugins/trackbuilder/track/widgets/sectionwidgetstraight.ui \
    plugins/trackbuilder/track/widgets/sectionwidgetinfo.ui \
    plugins/trackbuilder/track/widgets/sectionwidgetcurved.ui \
    plugins/trackbuilder/track/widgets/sectionwidgetforced.ui

OTHER_FILES += \
    shaders/main.frag \
    shaders/main.vert \
    shaders/scattering.frag \
    shaders/water.vert \
    shaders/shadow.frag \
    shaders/shadow.vert \
    shaders/ShadowMapping.vert \
    shaders/ShadowMapping.frag \
    shaders/SimpleTexture.frag \
    shaders/Passthrough.vert

RESOURCES += \
    textures/textures.qrc \
    shaders/shaders.qrc \
    trackstyles/trackstyles.qrc \
    models/models.qrc

OBJECTIVE_SOURCES += \
    common/fullscreen.mm
