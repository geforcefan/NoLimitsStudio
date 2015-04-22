#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <QRegExp>

#include <QFile>

#include "objloader.hpp"

#include <QTextStream>
#include <common/Mesh.h>

Mesh *loadOBJ(
    const char * path
) {
    return loadOBJ(path, "");
}

Mesh *loadOBJ(
    const char * path,
    std::string objectName
){
    Mesh *out = new Mesh();
    qDebug("Loading OBJ file %s", path);

    std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

    QFile file(path);
    if (!file.open (QIODevice::ReadOnly)) qDebug("File %s not found", path);
    QTextStream stream ( &file );
    char *line;
    char currentObjectName[128] = "";

    bool foundObjectName = !objectName.size()?true:false;
    bool progressObject = !objectName.size()?true:false;

    while( !stream.atEnd() ) {
        line = NoLimitsRenderer::toChar(stream.readLine());
        char lineHeader[128];
        char tmps[128];
        sscanf(line, "%s", &lineHeader);

        if ( objectName.size() && strcmp( lineHeader, "o" ) == 0 ){
            sscanf(line, "%s %s\n", &tmps, &currentObjectName);


            QRegExp rx(objectName.c_str());
            if (rx.indexIn(std::string(currentObjectName).c_str()) < 0) progressObject = false;
            else progressObject = foundObjectName = true;
            continue;
        }

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            sscanf(line, "%s %f %f %f\n", &tmps, &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            sscanf(line, "%s %f %f\n", &tmps, &uv.x, &uv.y );
            //uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            sscanf(line, "%s %f %f %f\n", &tmps, &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }

        if(!progressObject) continue;

        if ( strcmp( lineHeader, "f" ) == 0 ){
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d\n", &tmps, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            if (matches != 10){
                qDebug("File can't be read by our simple parser :-( Try exporting with other options\n");
                return out;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    file.close(); // when your done.

    if(!foundObjectName) return out;

	// For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

        out->addVertex(vertex, normal, uv);
    }

    return out;
}
