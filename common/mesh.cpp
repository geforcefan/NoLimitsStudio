#include "Mesh.h"

#include <QDebug>

Mesh::Mesh(void)
{
}

Mesh::~Mesh(void)
{
}


void Mesh::addVertex(glm::vec3 _vertex) {
    verticies.push_back(_vertex.x);
    verticies.push_back(_vertex.y);
    verticies.push_back(_vertex.z);
}
void Mesh::addNormal(glm::vec3 _normal) {
    normals.push_back(_normal.x);
    normals.push_back(_normal.y);
    normals.push_back(_normal.z);
}
void Mesh::addTexCoord(glm::vec2 _texcoord) {
    texcoord.push_back(_texcoord.x);
    texcoord.push_back(_texcoord.y);
}
void Mesh::addColor(glm::vec4 _color) {
    colors.push_back(_color.r);
    colors.push_back(_color.g);
    colors.push_back(_color.b);
    colors.push_back(_color.a);
}
void Mesh::addFaceIndicies(short int index0, short int index1, short int index2) {
    indicies.push_back(index0);
    indicies.push_back(index1);
    indicies.push_back(index2);
}

void Mesh::addVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
{

    //qDebug() << "Mesh::addVertex";

    verticies.push_back(p.x);
    verticies.push_back(p.y);
    verticies.push_back(p.z);
    indicies.push_back(verticies.size()-1);

    normals.push_back(n.x);
    normals.push_back(n.y);
    normals.push_back(n.z);

    texcoord.push_back(t.x);
    texcoord.push_back(t.y);

    colors.push_back(NoLimitsRenderer::currentColor.r);
    colors.push_back(NoLimitsRenderer::currentColor.g);
    colors.push_back(NoLimitsRenderer::currentColor.b);
    colors.push_back(NoLimitsRenderer::currentColor.a);
}

void Mesh::setColor(glm::vec4 c) {
    for(int i = 0; i < colors.size(); i+=4) {
        colors[i] = c.r;
        colors[i+1] = c.g;
        colors[i+2] = c.b;
        colors[i+3] = c.a;
    }
}

void Mesh::addPoint(glm::vec3 p, glm::vec3 n, glm::vec2 t, glm::vec4 c)
{
    verticies.push_back(p.x);
    verticies.push_back(p.y);
    verticies.push_back(p.z);
    indicies.push_back(verticies.size()-1);

    normals.push_back(n.x);
    normals.push_back(n.y);
    normals.push_back(n.z);

    texcoord.push_back(t.x);
    texcoord.push_back(t.y);

    colors.push_back(c.r);
    colors.push_back(c.g);
    colors.push_back(c.b);
    colors.push_back(c.a);
}

void Mesh::addVerts(Mesh* m) {
    for(int i=0; i < m->indicies.size(); i++) indicies.push_back(verticies.size() + m->indicies[i]);
    for(int i=0; i < m->verticies.size(); i++) verticies.push_back(m->verticies[i]);
    for(int i=0; i < m->normals.size(); i++) normals.push_back(m->normals[i]);
    for(int i=0; i < m->texcoord.size(); i++) texcoord.push_back(m->texcoord[i]);
    for(int i=0; i < m->colors.size(); i++) colors.push_back(m->colors[i]);
}

void Mesh::set(
        std::vector<glm::vec3> p,
        std::vector<glm::vec3> n,
        std::vector<glm::vec2> t,
        std::vector<glm::vec4> c,
        std::vector<unsigned short> i
        ) {
    /*verticies = p;
    normals = n;
    texcoord = t;
    colors = c;
    indicies = i;*/

    qDebug() << "Mesh::set";
}

void Mesh::set(
        std::vector<glm::vec3> p,
        std::vector<glm::vec3> n,
        std::vector<glm::vec2> t,
        glm::vec4 c,
        std::vector<unsigned short> i
        ) {

    /*verticies = p;
    for(int i = 0; i < verticies.size(); i++) colors.push_back(c);
    normals = n;
    texcoord = t;
    indicies = i;*/

    qDebug() << "Mesh::set";
}

void Mesh::clear() {
    verticies.clear();
    indicies.clear();
    normals.clear();
    texcoord.clear();
    colors.clear();
}

void Mesh::applyMatrix(glm::mat4 _m) {
    for(int i=0; i < verticies.size(); i+=3) {
        glm::vec4 newPos = _m * glm::vec4(verticies[i], verticies[i+1], verticies[i+2], 1.0f);
        verticies[i] = newPos.x;
        verticies[i+1] = newPos.y;
        verticies[i+2] = newPos.z;
    }
}

void Mesh::applyMatrix(glm::mat3 _m) {
    for(int i=0; i < verticies.size(); i+=3) {
        glm::vec3 newPos = _m * glm::vec3(verticies[i], verticies[i+1], verticies[i+2]);
        verticies[i] = newPos.x;
        verticies[i+1] = newPos.y;
        verticies[i+2] = newPos.z;
    }
}
