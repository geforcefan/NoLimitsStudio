#ifndef EXTRUDE_H
#define EXTRUDE_H

#include <common/global.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>

#include <common/VertexBufferObject.h>

class extrude
{
public:
    extrude(vec4List _points, vec4List _form, vec4List _pointColors, bool _closed);
    extrude(vec4List _points, std::vector<vec4List> _form, bool _closed);
    extrude(vec4List _points, vec4List _form, bool _closed);
    extrude();
    void render();
    Mesh *mesh;
    Mesh *capMesh;

    // Object Vertex
    VertexBufferObject *objectVBO;
private:
    vec4List points;
    std::vector<vec4List> form;
    std::vector<glm::vec4> pointColors;

    std::vector<vec4List> objects;

    bool closed;
    bool generated;

    void generate();
};

#endif // EXTRUDE_H
