#ifndef MESH_H
#define MESH_H

#include <common/global.h>

#include <vector>

typedef struct {
    int index0, index1, index2, normal;
    glm::vec2 index0uv, index1uv, index2uv;
    glm::vec4 color;
} MeshFace;

class Mesh
{
public:
   Mesh(void);
  ~Mesh(void);

  void addVertex(glm::vec3 _vertex);
  void addNormal(glm::vec3 _normal);
  void addTexCoord(glm::vec2 _texcoord);
  void addColor(glm::vec4 _color);
  void addFaceIndicies(short int index0, short int index1, short int index2);
  void addPoint(glm::vec3 p, glm::vec3 n, glm::vec2 t, glm::vec4 c);

  void setColor(glm::vec4 c);

  void addVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t);
  void addVerts(Mesh* m);
  void applyMatrix(glm::mat4 _m);
  void applyMatrix(glm::mat3 _m);
  void clear();

  void set(
          std::vector<glm::vec3> p,
          std::vector<glm::vec3> n,
          std::vector<glm::vec2> t,
          std::vector<glm::vec4> c,
          std::vector<unsigned short> i
          );

  void set(
          std::vector<glm::vec3> p,
          std::vector<glm::vec3> n,
          std::vector<glm::vec2> t,
          glm::vec4 c,
          std::vector<unsigned short> i
          );


  std::vector<float> verticies;
  std::vector<unsigned short> indicies;
  std::vector<float> normals;
  std::vector<float> texcoord;
  std::vector<float> secondTexcoord;
  std::vector<float> colors;
};

#endif
