#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <common/global.h>

class Mesh;

Mesh *loadOBJ(
    const char * path
);

Mesh *loadOBJ(
    const char * path,
    std::string objectName
);

#endif
