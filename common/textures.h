#ifndef TEXTURES_H
#define TEXTURES_H

#include <common/global.h>
#include <map>

class textures
{
private:
    std::map<std::string, int> _textures;
public:
    textures();
    GLuint getTexture(char *name);
    GLuint loadTexture(char *name, char *_filename);
    GLuint loadTexture(char *name, char *_filename, bool fromResource);
    void bindTexture(GLuint tex);
    void bindSecondTexture(GLuint tex);

    void releaseTexture();
    void releaseSecondTexture();

    GLuint texSamplerUse, secondTexSamplerUse;
};

#endif // TEXTURES_H
