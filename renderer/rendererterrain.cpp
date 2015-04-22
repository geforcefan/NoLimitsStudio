#include <maingl/glrenderer.h>
#include <maingl/glwidget.h>

#include <common/objloaderbuffer.h>
#include <common/textures.h>
#include <common/extrude.h>
#include <common/settings.h>

#include "rendererterrain.h"

rendererTerrain::rendererTerrain(GLWidget& _glw) : glw(_glw) {
    renderer();

    terrainObject = new VertexBufferObject();
    waterObject = new VertexBufferObject();
    terrainMesh = new Mesh();
    waterMesh = new Mesh();
}

void rendererTerrain::init() {
    allowRender = true;
}

void rendererTerrain::render() {
    if(changes) {
        terrainTexture = NoLimitsRenderer::Textures->loadTexture("rendererTerrain:terrainTexture",
                                                                 (char*)NoLimitsRenderer::getNLFilePath(NoLimitsRenderer::Track.groundTextureFilename)
                                                                 );
        terrainDetailTexture = NoLimitsRenderer::Textures->loadTexture("rendererTerrain:terrainDetailTexture",
                                                                 (char*)NoLimitsRenderer::getNLFilePath(NoLimitsRenderer::Track.detailTextureFilename)
                                                                 );
        waterTexture = NoLimitsRenderer::Textures->loadTexture("rendererTerrain:waterTexture",
                                                                 (char*)NoLimitsRenderer::getNLFilePath(NoLimitsRenderer::Track.waterTextureFilename)
                                                                 );

        terrainObject = new VertexBufferObject();
        waterObject = new VertexBufferObject();
        if(NoLimitsRenderer::Track.sizeX) {
            terrainObject->setData(terrainMesh);
            waterObject->setData(waterMesh);
        }
        changes = false;
    }
    if(allowRender && NoLimitsRenderer::Settings->rendererTerrain) {
        NoLimitsRenderer::Textures->bindTexture(terrainTexture);
        NoLimitsRenderer::Textures->bindSecondTexture(terrainDetailTexture);

        terrainObject->drawVBO();
        NoLimitsRenderer::Textures->releaseSecondTexture();

        NoLimitsRenderer::Textures->bindTexture(waterTexture);
        waterObject->drawVBO();
        NoLimitsRenderer::Textures->releaseTexture();
    }
}


void rendererTerrain::update() {
    QMutex mutex;
    allowRender = false;
    //delete terrainObject, terrainMesh, waterMesh;
    terrainMesh = new Mesh();
    waterMesh = new Mesh();

    if(NoLimitsRenderer::Track.sizeX > 1) {
        std::vector<glm::vec3> verticies;
        std::vector<glm::vec2> uvGround;
        std::vector<glm::vec2> uvDetail;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> _normals;
        std::vector<MeshFace> faces;
        std::map<int, std::vector<int> > vertex2faces;

        // the uv coordinates being calculated for the ground texture map
        float uvGroundStepX = ((float)NoLimitsRenderer::Track.numGroundTextureRepeats) / (NoLimitsRenderer::Track.sizeX - 1);
        float uvGroundStepZ = ((float)NoLimitsRenderer::Track.numGroundTextureRepeats) / (NoLimitsRenderer::Track.sizeZ - 1);
        float uvGroundX = 0.0f;
        float uvGroundZ = 0.0f;

        // the uv coordinates being calculated for the detail texture map
        float uvDetailStepX = ((float)NoLimitsRenderer::Track.numDetailTextureRepeats) / (NoLimitsRenderer::Track.sizeX - 1);
        float uvDetailStepZ = ((float)NoLimitsRenderer::Track.numDetailTextureRepeats) / (NoLimitsRenderer::Track.sizeZ - 1);
        float uvDetailX = 0.0f;
        float uvDetailZ = 0.0f;

        for (int z = 0; z < NoLimitsRenderer::Track.sizeZ; z++)
        {
            uvGroundX = uvDetailX = 0.0f;
            for (int x = 0; x < NoLimitsRenderer::Track.sizeX; x++)
            {
                verticies.push_back(glm::vec3(
                            float(z)*float(NoLimitsRenderer::Track.scaleZ) - (float(NoLimitsRenderer::Track.sizeZ-1)*float(NoLimitsRenderer::Track.scaleZ)) / 2,
                            (float)NoLimitsRenderer::Track.terrainHeight[x][z],
                            float(x)*float(NoLimitsRenderer::Track.scaleX) - (float(NoLimitsRenderer::Track.sizeX-1)*float(NoLimitsRenderer::Track.scaleX)) / 2
                            ));
                uvGround.push_back(glm::vec2(uvGroundZ, uvGroundX));
                uvDetail.push_back(glm::vec2(uvDetailZ, uvDetailX));
                uvGroundX += uvGroundStepX;
                uvDetailX += uvDetailStepX;
            }
            uvGroundZ += uvGroundStepZ;
            uvDetailZ += uvDetailStepZ;
        }

        int currentX=0;
        for(int i = 0; i < verticies.size()-NoLimitsRenderer::Track.sizeZ; i++) {

            currentX++;
            if(currentX == NoLimitsRenderer::Track.sizeX) {
                currentX = 0;
                continue;
            }

            int currentBaseIndex = i;
            int nextBaseIndex = i + NoLimitsRenderer::Track.sizeX;

            MeshFace face0;
            face0.index2 = currentBaseIndex + 0;
            face0.index1 = nextBaseIndex + 1;
            face0.index0 = currentBaseIndex + 1;

            MeshFace face1;
            face1.index0 = nextBaseIndex + 1;
            face1.index1 = currentBaseIndex + 0;
            face1.index2 = nextBaseIndex;

            vertex2faces[face0.index2].push_back(faces.size());
            vertex2faces[face0.index1].push_back(faces.size());
            vertex2faces[face0.index0].push_back(faces.size());

            vertex2faces[face1.index0].push_back(faces.size()+1);
            vertex2faces[face1.index1].push_back(faces.size()+1);
            vertex2faces[face1.index2].push_back(faces.size()+1);

            faces.push_back(face0);
            faces.push_back(face1);
        }

        int num = 1;
        glm::vec3 normal;
        for(int i=0; i < faces.size(); i++) {
            glm::vec3 p1 = glm::vec3(verticies[faces[i].index0].x,
                                     verticies[faces[i].index0].y,
                                     verticies[faces[i].index0].z);

            glm::vec3 p2 = glm::vec3(verticies[faces[i].index1].x,
                                     verticies[faces[i].index1].y,
                                     verticies[faces[i].index1].z);

            glm::vec3 p3 = glm::vec3(verticies[faces[i].index2].x,
                                     verticies[faces[i].index2].y,
                                     verticies[faces[i].index2].z);

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
        for(int i=0; i < verticies.size(); i++) {
            glm::vec3 outVec;
            int num=0;
            for(int j=0; j < vertex2faces[i].size(); j++) outVec += normals[faces[vertex2faces[i][j]].normal];
            if(num) outVec /= (float)vertex2faces[i].size();
            float d = outVec.length();
            if(d) outVec /= (float)d;

           _normals.push_back(outVec);
        }

        int facenum = 0;
        for(int i=0; i < faces.size(); i++) {
            int index0 = faces[i].index0, index1 = faces[i].index1, index2 = faces[i].index2;
            glm::vec2 index0uv = faces[i].index0uv, index1uv = faces[i].index1uv, index2uv = faces[i].index2uv;

            if(facenum) {
                index0 = faces[i].index2;
                index2 = faces[i].index0;

                index0uv = faces[i].index2uv;
                index2uv = faces[i].index0uv;
                facenum = 0;
            } else facenum++;

            glm::vec3 p1 = glm::vec3(verticies[index0].x,
                                     verticies[index0].y,
                                     verticies[index0].z);
            glm::vec3 n1 = _normals[index0];
            glm::vec2 uvGround1 = uvGround[index0];
            glm::vec2 uvDetail1 = uvDetail[index0];

            glm::vec3 p2 = glm::vec3(verticies[index1].x,
                                     verticies[index1].y,
                                     verticies[index1].z);
            glm::vec3 n2 = _normals[index1];
            glm::vec2 uvGround2 = uvGround[index1];
            glm::vec2 uvDetail2 = uvDetail[index1];

            glm::vec3 p3 = glm::vec3(verticies[index2].x,
                                     verticies[index2].y,
                                     verticies[index2].z);
            glm::vec3 n3 = _normals[index2];
            glm::vec2 uvGround3 = uvGround[index2];
            glm::vec2 uvDetail3 = uvDetail[index2];

            glm::vec4 terrainColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            terrainMesh->addPoint(p1, n1, uvGround1, terrainColor);
            terrainMesh->addPoint(p2, n2, uvGround2, terrainColor);
            terrainMesh->addPoint(p3, n3, uvGround3, terrainColor);

            // TODO: Umstellung auf floats
            /*terrainMesh->secondTexcoord.push_back(uvDetail1);
            terrainMesh->secondTexcoord.push_back(uvDetail2);
            terrainMesh->secondTexcoord.push_back(uvDetail3);*/
        }

        // Water
        glm::vec4 color = glm::vec4(1.0,1.0,1.0,1.0);
        glm::vec2 waterSize = glm::vec2(
                (float(NoLimitsRenderer::Track.sizeZ-1)*float(NoLimitsRenderer::Track.scaleZ)) / 2.0f,
                (float(NoLimitsRenderer::Track.sizeX-1)*float(NoLimitsRenderer::Track.scaleX)) / 2.0f);

        /*0*/ waterMesh->addPoint(glm::vec3(-waterSize.x, NoLimitsRenderer::Track.seaLevel,  waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, (float)NoLimitsRenderer::Track.numWaterTextureRepeats), color);
        /*1*/ waterMesh->addPoint(glm::vec3( waterSize.x, NoLimitsRenderer::Track.seaLevel,  waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2((float)NoLimitsRenderer::Track.numWaterTextureRepeats, (float)NoLimitsRenderer::Track.numWaterTextureRepeats), color);
        /*2*/ waterMesh->addPoint(glm::vec3( waterSize.x, NoLimitsRenderer::Track.seaLevel, -waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2((float)NoLimitsRenderer::Track.numWaterTextureRepeats, 0), color);

        /*2*/ waterMesh->addPoint(glm::vec3( waterSize.x, NoLimitsRenderer::Track.seaLevel, -waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2((float)NoLimitsRenderer::Track.numWaterTextureRepeats, 0), color);
        /*3*/ waterMesh->addPoint(glm::vec3(-waterSize.x, NoLimitsRenderer::Track.seaLevel, -waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, 0), color);
        /*0*/ waterMesh->addPoint(glm::vec3(-waterSize.x, NoLimitsRenderer::Track.seaLevel,  waterSize.y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, (float)NoLimitsRenderer::Track.numWaterTextureRepeats), color);
    }

    changes = true;
    allowRender = true;
}
