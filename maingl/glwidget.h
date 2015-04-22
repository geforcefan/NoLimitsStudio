/*
 *   Example implementation of a QGLWidget with a separat rendering thread
 *   and GL picking capabilites.
 *
 *   This is the QGLWidget subclass interface.
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
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <common/global.h>
#include <common/VertexBufferObject.h>

#include <maingl/glthread.h>
#include <qgl.h>
#include <qmutex.h>
#include <qwaitcondition.h>

#include <QMouseEvent>
#include <QResizeEvent>

class GLWidget : public QGLWidget
{
public:
    GLWidget(const QGLFormat &format, QWidget *parent = 0, const char *name = 0);

    void initRendering();
    void finishRendering();
    void lockGLContext();
    void unlockGLContext();
    QWaitCondition& renderCondition();
    QMutex& renderMutex();

    QPoint mapPoint(QPoint p);
    void mouseMoveEvent(QMouseEvent *e);

    GLThread & getGLThread();
    bool eventFilterFake(QObject *obj, QEvent *event);

public slots:
    void render();

protected:
    void closeEvent(QCloseEvent* _e);
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent* _e);

private:
    // The rendering thread
    QMutex render_mutex;
    QWaitCondition render_condition;
    GLThread glt;
};

#endif // GLWIDGET_H
