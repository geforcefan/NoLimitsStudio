#include "objloaderbuffer.h"

objLoaderBuffer::objLoaderBuffer() {
    VertexBufferObject();
    mesh = new Mesh();
}

objLoaderBuffer::objLoaderBuffer(const char *filename) {
    VertexBufferObject();
    mesh = new Mesh();
    load(filename, "");
}

objLoaderBuffer::objLoaderBuffer(const char *filename, std::string objectName) {
    VertexBufferObject();
    mesh = new Mesh();
    load(filename, objectName);
}

void objLoaderBuffer::load(const char *filename, std::string objectName) {
    VertexBufferObject();
    delete mesh;
    mesh = loadOBJ(filename, objectName);
    //finalizeData();
}

void objLoaderBuffer::finalizeData() {
    setData(mesh);
}

objLoaderBuffer *objLoaderBuffer::clone() {
    objLoaderBuffer *out = new objLoaderBuffer();
    out->mesh = this->mesh;
    return out;
}
