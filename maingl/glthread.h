/*
 *   Example implementation of a QGLWidget with a separat rendering thread
 *   and GL picking capabilites.
 *
 *   This is the GL rendering thread interface.
 *
 *   Copyright (C) 2005 by
 *   Michael Hanke        michael.hanke@gmail.com
 *
 *   This example combines two other examples on using Qt for OpenGL
 *   - QGLWidget example (single threaded, GL picking)
 *     from the book 'C++  GUI Programming with Qt 3' by
 *     Jasmin Blanchette and Mark Summerfield
 *     The example code 'cube' available from here:
 *        ftp://ftp.trolltech.com/misc/qtbook-examples.zip
 *   - Qt Quarterly article 'Glimpsing the Third Dimension' on multithreaded
 *     OpenGL with Qt (see here: http://doc.trolltech.com/qq/qq06-glimpsing.html)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef GLTHREAD_H
#define GLTHREAD_H

#include <common/global.h>
#include <common/shader.h>
#include <common/VertexBufferObject.h>
#include <common/lights.h>

#include <qgl.h>
#include <qthread.h>
#include <qsize.h>

#include <vector>

#include <QMouseEvent>

#include <QPoint>
#include <QCursor>
#include <QTimer>
#include <QMainWindow>

class GLWidget;
class GLRenderer;
class Camera;

class GLThread : public QThread
{
    Q_OBJECT
public:
    GLThread(GLWidget& _glw);

    void run();
    void stop();
    void init();

    bool running;

    // Resize
    void resizeViewport(const QSize& _size);

    // Events
    bool eventFilterFake(QObject *obj, QEvent *event);

    // GL Widget
    GLWidget& glw;

    // Viewport Size
    QSize viewport_size;

    // Renderer
    GLRenderer *Renderer;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    volatile bool render_flag;
    volatile bool resize_flag;

    // FPS
    QTimer *m_fpsTimer;
    int numFrames;

    // Main Window
    QMainWindow *window;
public slots:
    void showFramesPerSecond();
};


#endif // GLTHREAD_H

