#pragma once

#include <string>
#include <citro3d.h>
#include <vector>
#include "types.h"

class Gmobj {
    protected:
        /// @brief destroy the object and clean up memory
        void destroy();
    public:
        class Mesh {
            friend class Gmobj;
            public:
                std::string objPath;
                std::vector<vertex> vertexList;
                /// @brief Call the Set buffer function
                void renderBuffer();
                int uLoc_projection, uLoc_modelView;
                DVLB_s* vshader_dvlb;
                shaderProgram_s program;
                C3D_BufInfo bufferInf;
                C3D_AttrInfo attrInf;
                C3D_FVec position;
                C3D_FQuat rotation;
                C3D_Mtx modelMatrix;
                
                void* vboData;
                void AddToBuffer(void* vboData, size_t dataSize);
                /// @brief Load mesh data into memory
                bool loadMesh();
                /// @brief Add mesh to buffer
                void loadRender();
                /// @brief Remove mesh from buffer
                void uloadRender();
        };

        /// @brief remove the object from scene
        void uinit();

        // Transform functions
        void setPosition(vec3 position);
        void setRotation(quat rotation);
        Mesh mesh;
        Gmobj() {}
        ~Gmobj() {
            destroy();
        }
};
