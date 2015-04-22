#ifndef OBJLOADERBUFFER_H
#define OBJLOADERBUFFER_H

#include <common/objloader.hpp>
#include <common/Mesh.h>
#include <common/VertexBufferObject.h>

class objLoaderBuffer : public VertexBufferObject
{
public:
    objLoaderBuffer();
    objLoaderBuffer(const char *filename);
    objLoaderBuffer(const char *filename, std::string objectName);
    void load(const char *filename, std::string objectName);

    void finalizeData();
    objLoaderBuffer *clone();

    Mesh *mesh;
private:
};

#endif // OBJLOADERBUFFER_H
