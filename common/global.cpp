#include <common/global.h>
#include <common/textures.h>
#include <common/shader.h>
#include <common/camerabase.h>
#include <common/lights.h>
#include <common/nlbanking.h>
#include <common/Mesh.h>
#include <common/plugins.h>
#include <common/savehandler.h>
#include <common/settings.h>
#include <maingl/glrenderer.h>

#include <qfile.h>
#include <qtextstream.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include <stdarg.h>
#include <common/NLSegment.h>

#include <QDir>
#include <QFile>

namespace NoLimitsRenderer {
    NlTrack Track;
    textures *Textures;
    shader *currentShader;
    shader *lastShader;
    GLRenderer *glrenderer;
    glm::vec4 currentColor;

    CameraBase *lastCamera;
    CameraBase *currentCamera;

    GLuint texSamplerUse;
    GLuint secondTexSamplerUse;

    lights *Lights;
    CameraBase *camera;
    NLBanking *TrackBanking;
    NLBanking *TrackPOV;
    windowMain *mainWindow;
    plugins *Plugins;
    saveHandler *SaveHandler;
    settings *Settings;

    bool pluginsLoaded;

    void initGlobal() {
        initNewNlTrack(&Track);
        currentColor = glm::vec4(1.0f);
        Lights = new lights();
        TrackBanking = new NLBanking();
        TrackPOV = new NLBanking();
        TrackPOV->resetTimeValue = false;

        SaveHandler = new saveHandler();
        Settings = new settings();

        // Setup inital Lights
        Lights->addLight(
                    glm::vec3(200.0f, 500.0f, 200.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                    glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)
                    );
    }

    void initGL() {
        Textures = new textures();
    }

    char* toChar(QString input) {
        char* cstr;
        std::string fname = input.toStdString();
        cstr = new char [fname.size()+1];
        strcpy( cstr, fname.c_str() );
        return cstr;
    }

    char *readFile(char *filename) {
        QString fileout;
        QFile file(filename);
        if (!file.open (QIODevice::ReadOnly)) qDebug("File %s not found", filename);
        QTextStream stream ( &file );
        QString line;
        while( !stream.atEnd() ) {
            line = stream.readLine();
            fileout.append("\n").append(line);
        }
        file.close(); // when your done.

        return toChar(fileout);
    }

    void setColor(glm::vec4 color) {
        currentColor = color;
    }

    glm::vec4 makeColor(int r, int g, int b, int a) {
        return glm::vec4((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/255.0f);
    }

    void bindShader(shader *curr) {
        lastShader = currentShader;
        currentShader = curr;
        currentShader->bind();
    }

    void restoreShader() {
        bindShader(lastShader);
    }

    void bindCamera(CameraBase *curr) {
        lastCamera = camera;
        camera = curr;
    }

    void restoreCamera() {
        bindCamera(lastCamera);
    }

    vec4List makeCircle(float radius, int steps, glm::vec3 translate) {
        vec4List out;
        for( int n = 0; n < steps; ++n ) {
            float const t = 2*M_PI*(float)n/(float)steps;
            out.push_back(glm::vec4(translate.x + sin(t)*radius, translate.y + cos(t)*radius, translate.z, 1.0f));
        }
        return out;
    }

    vec4List makeSquare(glm::vec3 size, glm::vec3 translate) {
        vec4List out;
        out.push_back(glm::vec4(translate.x + -size.x/2, translate.y + -size.y/2, translate.z + size.z/2, 1.0f));
        out.push_back(glm::vec4(translate.x + -size.x/2, translate.y +  size.y/2, translate.z + size.z/2, 1.0f));
        out.push_back(glm::vec4(translate.x +  size.x/2, translate.y +  size.y/2, translate.z + size.z/2, 1.0f));
        out.push_back(glm::vec4(translate.x +  size.x/2, translate.y + -size.y/2, translate.z + size.z/2, 1.0f));
        return out;
    }

    const char *getNLFilePath(const char *filename) {
#ifdef Q_WS_WIN
        //return toChar(QString("C:\\Program Files (x86)\\NoLimits Coasters v1.6\\").append(QString(filename)).replace("\x2f", "\\"));
        return toChar(QString("E:\\Applications\\NoLimits\\").append(QString(filename)).replace("\x2f", "\\"));
#else
        return toChar(QString("/Applications/NoLimits/").append(QString(filename)).replace("\x5C", "/"));
#endif
    }

    vec4List makeFormFormFile(const char *filename) {
        vec4List out;

        QFile file(filename);
        qDebug("Loading Form file %s", filename);
        if (!file.open (QIODevice::ReadOnly)) qDebug("File %s not found", filename);

        QTextStream stream ( &file );
        QStringList sl;
        while( !stream.atEnd() ) sl = stream.readLine().split("\r");

        float x,y,z;
        int index;

        for(int i=1; i < sl.size(); i++) {
            char *line = toChar(sl[i]);
            sscanf(line, "%d\t%f cm\t%f cm\t%f cm", &index, &x, &y, &z);
            out.push_back(glm::vec4(y, x, z, 1.0f));
        }
        file.close();

        return out;
    }

    int lastVerticies=0;
    void exportMeshObj(Mesh *m, const char *name) {
        /*std::fstream outputFile;
        outputFile.open (toChar(QString("/Users/ercanakyurek/Daten/glTest/object.obj")), std::fstream::in | std::fstream::out | std::fstream::app);

        outputFile << "o " << name << "\n";

        for(int i=0; i < m->verticies.size(); i++) outputFile << "v " << m->verticies[i].x << " " << m->verticies[i].y << " " << m->verticies[i].z << "\n";
        for(int i=0; i < m->texcoord.size(); i++) outputFile << "vt " << m->texcoord[i].x << " " << m->texcoord[i].y << "\n";
        for(int i=0; i < m->normals.size(); i++) outputFile << "vn " << m->normals[i].x << " " << m->normals[i].y << " " << m->normals[i].z << "\n";
        outputFile << "s off\n";
        for(int i=0; i < m->verticies.size(); i+=3) {
            int i0 = (i) + 0 + 1 + lastVerticies;
            int i1 = (i) + 1 + 1 + lastVerticies;
            int i2 = (i) + 2 + 1 + lastVerticies;

            outputFile << "f ";
            outputFile << i0 << "/" << i0 << "/" << i0 << " ";
            outputFile << i1 << "/" << i1 << "/" << i1 << " ";
            outputFile << i2 << "/" << i2 << "/" << i2 << " ";
            outputFile << "\n";
        }
        lastVerticies += m->verticies.size();

        outputFile.close();*/
    }

    glm::vec3 v3pow(glm::vec3 p, int n) {
        return glm::vec3(pow(p.x, n), pow(p.y, n), pow(p.z, n));
    }

    float sum(int n_args, ...) {
        /*register int i;
        int max;
        float sum;
        va_list ap;

        va_start(ap, n_args);
        max = va_arg(ap, int);
        for(i = 0; i < n_args; i++) {
            glm::vec3 a = va_arg(ap, glm::vec3);
            sum = a.x + a.y + a.z;
        }

        va_end(ap);
        return max;*/
    }

    bool removeDir(const QString &dirName) {
        bool result = true;
        QDir dir(dirName);

        if (dir.exists(dirName)) {
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if (info.isDir()) {
                    result = removeDir(info.absoluteFilePath());
                }
                else {
                    result = QFile::remove(info.absoluteFilePath());
                }

                if (!result) {
                    return result;
                }
            }
            result = dir.rmdir(dirName);
        }

        return result;
    }

    bool isZero(float a) {
        if (a <  std::numeric_limits<float>::epsilon() &&
            a > -std::numeric_limits<float>::epsilon()) {
            return true;
        }
        return false;
    }

    bool isInTolerance(float a, double tolerance) {
        if (a <  tolerance &&
            a > -tolerance) {
            return true;
        }
        return false;
    }
}
