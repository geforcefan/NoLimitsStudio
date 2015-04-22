#include <common/global.h>

#include "extrude.h"

#include <common/vectorstuff.h>
#include <common/triangulate.h>

extrude::extrude() {

}

extrude::extrude(vec4List _points, vec4List _form, bool _closed) {
    generated = false;
    for(int i=0; i < _points.size(); i++) form.push_back(vec4List(_form));
    points = vec4List(_points);
    closed = bool(_closed);
    generate();
}

extrude::extrude(vec4List _points, std::vector<vec4List> _form, bool _closed) {
    generated = false;
    form = std::vector<vec4List>(_form);
    points = vec4List(_points);
    closed = bool(_closed);
    generate();
}

extrude::extrude(vec4List _points, vec4List _form, vec4List _pointColors, bool _closed) {
    generated = false;
    for(int i=0; i < _points.size(); i++) form.push_back(vec4List(_form));
    points = vec4List(_points);
    pointColors = _pointColors;
    closed = bool(_closed);
    generate();
}

void extrude::generate() {
    vectorStuff *VectorStuff = new vectorStuff();
    objectVBO = new VertexBufferObject();
    mesh = new Mesh();
    capMesh = new Mesh();
    Mesh *_mesh = new Mesh();

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec2> texcoord;

    for(int i=0; i < points.size(); i++) {
        vec4List newObject;

        int firstIndex = (i==points.size()-1?i-1:i);
        int secIndex = (i==points.size()-1?i:i+1);

        glm::vec3 point(points[firstIndex].x, points[firstIndex].y, points[firstIndex].z);
        glm::vec3 point2(points[secIndex].x, points[secIndex].y, points[secIndex].z);

        glm::mat4 objMatrix;
        if(i && i!=points.size()-1 && points.size() > 2) {
            glm::vec3 lastPoint(points[firstIndex-1].x, points[firstIndex-1].y, points[firstIndex-1].z);
            objMatrix = VectorStuff->alignToVector(point, point2, points[firstIndex].w, glm::vec3(points[i].x, points[i].y, points[i].z), true, lastPoint );
            //objMatrix = VectorStuff->alignToVector(point, point2, points[firstIndex].w, glm::vec3(points[i].x, points[i].y, points[i].z), false, glm::vec3(0.0f) );
        }
        else objMatrix = VectorStuff->alignToVector(point, point2, points[firstIndex].w, glm::vec3(points[i].x, points[i].y, points[i].z), false, glm::vec3(0.0f) );

        for(int iform = 0; iform < form[i].size(); iform++) {
            glm::vec4 newPoint(form[i][iform].x, form[i][iform].y, form[i][iform].z, 1.0f);
            newPoint = objMatrix * newPoint;

            newObject.push_back(newPoint);
            vertex.push_back(glm::vec3(newPoint.x, newPoint.y, newPoint.z));
            //texcoord.push_back(glm::vec2(i, currentStep));
        }

        // Front and Back Caps
        if(i==0 || i == points.size()-1) {
            vec4List caps = Triangulate::Process(newObject);
            glm::vec3 normal(0,0,0);
            for(int icap = 0; icap < caps.size(); icap+=3) {
                glm::vec3 p1 = glm::vec3(caps[icap+0].x, caps[icap+0].y, caps[icap+0].z);
                glm::vec3 p2 = glm::vec3(caps[icap+1].x, caps[icap+1].y, caps[icap+1].z);
                glm::vec3 p3 = glm::vec3(caps[icap+2].x, caps[icap+2].y, caps[icap+2].z);

                glm::vec3 u = p2-p1;
                glm::vec3 v = p3-p1;
                normal = glm::cross(u,v);
                normal = glm::normalize(normal);

                _mesh->addVertex(p3, normal, glm::vec2(0,0));
                _mesh->addVertex(p2, normal, glm::vec2(0,0));
                _mesh->addVertex(p1, normal, glm::vec2(0,0));
                _mesh->addVertex(p1, normal, glm::vec2(0,0));
                _mesh->addVertex(p2, normal, glm::vec2(0,0));
                _mesh->addVertex(p3, normal, glm::vec2(0,0));
            }
        }
    }

    int numStep = vertex.size() / form[0].size();
    std::vector<MeshFace> faces;
    std::vector<glm::vec3> normals;
    std::map<int, std::vector<int> > vertex2faces;

    for(int i=0; i < numStep-1; i++) {
        int currentBaseIndex = i * form[0].size();
        int nextBaseIndex = (i+1) * form[0].size();
        for(int point = 0; point < form[0].size(); point++) {
            MeshFace face0;
            face0.index0 = (currentBaseIndex + point) + 0;
            face0.index1 = point==form[0].size()-1?currentBaseIndex:(currentBaseIndex + point) + 1;
            face0.index2 = (nextBaseIndex + point) + 0;

            MeshFace face1;
            face1.index0 = (nextBaseIndex + point) + 0;
            face1.index1 = point==form[0].size()-1?nextBaseIndex:(nextBaseIndex + point) + 1;
            face1.index2 = point==form[0].size()-1?currentBaseIndex:(currentBaseIndex + point) + 1;

            if(pointColors.size()) {
                face0.color = pointColors[i];
                face1.color = pointColors[i];
            }

            vertex2faces[face0.index0].push_back(faces.size());
            vertex2faces[face0.index1].push_back(faces.size());
            vertex2faces[face0.index2].push_back(faces.size());


            faces.push_back(face0);
        }
    }

    int num = 1;
    glm::vec3 normal;
    for(int i=0; i < faces.size(); i++) {
        glm::vec3 p1 = glm::vec3(vertex[faces[i].index0].x,
                                 vertex[faces[i].index0].y,
                                 vertex[faces[i].index0].z);

        glm::vec3 p2 = glm::vec3(vertex[faces[i].index1].x,
                                 vertex[faces[i].index1].y,
                                 vertex[faces[i].index1].z);

        glm::vec3 p3 = glm::vec3(vertex[faces[i].index2].x,
                                 vertex[faces[i].index2].y,
                                 vertex[faces[i].index2].z);

        if(num == 1) {
            glm::vec3 u = p2-p1;
            glm::vec3 v = p3-p1;
            normal = glm::normalize(glm::cross(u,v));
            num = 0;
        } else {
            glm::vec3 u = p2-p3;
            glm::vec3 v = p1-p3;
            normal = glm::normalize(glm::cross(u,v));
            num++;
        }

        normals.push_back(normal);
        faces[i].normal = normals.size()-1;
    }

    std::vector<glm::vec3> _normals;
    for(int i=0; i < vertex.size(); i++) {
        glm::vec3 outVec;
        int num=0;
        for(int j=0; j < vertex2faces[i].size(); j++) outVec += normals[faces[vertex2faces[i][j]].normal];
        if(num) outVec /= (float)vertex2faces[i].size();
        float d = outVec.length();
        if(d) outVec /= (float)d;

       _normals.push_back(outVec);
    }

    std::vector<unsigned short> indicies;
    for(int i=0; i < faces.size(); i++) {
        indicies.push_back(faces[i].index0);
        indicies.push_back(faces[i].index1);
        indicies.push_back(faces[i].index2);
    }

    //mesh->set(vertex, _normals, texcoord, NoLimitsRenderer::currentColor, indicies);

    int facenum = 0;
    for(int i=0; i < faces.size(); i++) {
        int index0 = faces[i].index0, index1 = faces[i].index1, index2 = faces[i].index2;

        if(facenum) {
            index0 = faces[i].index2;
            index2 = faces[i].index0;
            facenum = 0;
        } else facenum++;

        glm::vec3 p1 = glm::vec3(vertex[index0].x,
                                 vertex[index0].y,
                                 vertex[index0].z);
        glm::vec3 n1 = _normals[index0];

        glm::vec3 p2 = glm::vec3(vertex[index1].x,
                                 vertex[index1].y,
                                 vertex[index1].z);
        glm::vec3 n2 = _normals[index1];
        glm::vec3 p3 = glm::vec3(vertex[index2].x,
                                 vertex[index2].y,
                                 vertex[index2].z);
        glm::vec3 n3 = _normals[index2];



        if(pointColors.size()) {
            glm::vec4 faceColor = faces[i].color;
            mesh->addPoint(p1, n1, glm::vec2(0.0), faceColor);
            mesh->addPoint(p2, n2, glm::vec2(0.0), faceColor);
            mesh->addPoint(p3, n3, glm::vec2(0.0), faceColor);
        } else {
            mesh->addVertex(p1, n1, glm::vec2(0.0));
            mesh->addVertex(p2, n2, glm::vec2(0.0));
            mesh->addVertex(p3, n3, glm::vec2(0.0));
        }
    }
    capMesh->addVerts(_mesh);
    generated = true;

    return;
}

void extrude::render() {
    if(generated) {
        objectVBO->setData(mesh);
        generated = false;
    }
    objectVBO->drawVBO();
}
