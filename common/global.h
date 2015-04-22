#ifndef GLOBAL_H
#define GLOBAL_H


extern "C" {
    #include <libnltrack/libnltrack.h>
}
class textures;
class shader;
class GLRenderer;


#include <QFile>
#include <QString>
#include <string>

#include <vector>
#include <bitset>

#define GLM_SWIZZLE GLM_SWIZZLE_XYZ
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QGLWidget>
#include <QLayout>

typedef std::vector<glm::vec4> vec4List;
typedef std::vector<glm::vec3> vec3List;
typedef std::map<int, glm::vec3> vec3Map;

typedef std::bitset<sizeof(int)> IntBits;

class lights;
class CameraBase;
class Mesh;
class NLBanking;
class windowMain;
class plugins;
class saveHandler;
class settings;

namespace NoLimitsRenderer {
    extern void initGlobal();
    extern void initGL();

    extern char* toChar(QString input);
    char *readFile(char *filename);
    extern void setColor(glm::vec4);
    extern void bindShader(shader *curr);
    extern void restoreShader();
    extern void exportMeshObj(Mesh *m, const char *name);
    extern glm::vec4 makeColor(int r, int g, int b, int a);
    extern bool isZero(float a);
    extern bool isInTolerance(float a, double tolerance);

    extern void bindCamera(CameraBase *curr);
    extern void restoreCamera();

    extern NlTrack Track;
    extern textures *Textures;
    extern shader *currentShader;
    extern shader *lastShader;
    extern GLRenderer *glrenderer;

    extern CameraBase *lastCamera;
    extern CameraBase *currentCamera;
    extern CameraBase *camera;

    extern glm::vec4 currentColor;

    extern GLuint texSamplerUse;
    extern GLuint secondTexSamplerUse;

    extern vec4List makeSquare(glm::vec3 size, glm::vec3 translate);
    extern vec4List makeCircle(float radius, int steps, glm::vec3 translate);
    extern vec4List makeFormFormFile(const char *filename);
    extern const char *getNLFilePath(const char* filename);
    extern bool removeDir(const QString &dirName);

    glm::vec3 v3pow(glm::vec3 p, int n);
    float sum(int n_args, ...);

    extern bool pluginsLoaded;

    extern lights *Lights;
    extern NLBanking *TrackBanking;
    extern NLBanking *TrackPOV;
    extern windowMain *mainWindow;
    extern plugins *Plugins;
    extern saveHandler *SaveHandler;
    extern settings *Settings;
}


#endif // GLOBAL_H
