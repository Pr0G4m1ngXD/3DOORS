#include "gmobj.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <citro3d.h>
#include "vshader_shbin.h"
#include "types.h"
#include "define.h"
#include <3ds.h>

    //position = C3D_FVec{0.0f, 0.0f, 0.0f, 1.0f};
    //rotation = C3D_FVec{0.0f, 0.0f, 0.0f, 1.0f};



void Gmobj::uinit() {
    // Implement unloading of resources here
    // Delete VAOs, VBOs, IBOs, etc.
    mesh.uloadRender();
}

void Gmobj::setPosition(vec3 position) {
    this->mesh.position.x = position.x;
    this->mesh.position.y = position.y;
    this->mesh.position.z = position.z;
    Mtx_Identity(&mesh.modelMatrix);
    Mtx_Translate(&mesh.modelMatrix, position.x, position.y, position.z, false); // Set the last argument to false
}


void Gmobj::setRotation(quat rotation) {
    this->mesh.rotation.r = rotation.r;
    this->mesh.rotation.k = rotation.k;
    this->mesh.rotation.j = rotation.j;
    this->mesh.rotation.i = rotation.i;

    // Implement rotation using Citro3D functions
    // For example: C3D_MtxRotateX, C3D_MtxRotateY, C3D_MtxRotateZ
}

void Gmobj::Mesh::AddToBuffer(void* vboData, size_t dataSize)
{
    // Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);

	// Get the location of the uniforms
	uLoc_projection   = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView    = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");

    AttrInfo_Init(&attrInf);
    AttrInfo_AddLoader(&attrInf, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(&attrInf, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(&attrInf, 2, GPU_FLOAT, 3); // v2=normal

    size_t bufSize = vertexList.size() * dataSize;
    vboData = linearAlloc(bufSize);
    memcpy(vboData, vertexList.data(), bufSize);
	BufInfo_Init(&bufferInf);
    BufInfo_Add(&bufferInf, vboData, dataSize, 3, 0x210);
}

bool Gmobj::Mesh::loadMesh()
{
    FILE* file = fopen(objPath.c_str(), "r");
    if (!file) {
        printf("Failed to open: %s", objPath.c_str());
        return false;
    }
    vertexList.clear();

    // Current Line
    char line[256];

    // We can index these later
    std::vector<array3> poslist;
    std::vector<array2> uvlist;
    std::vector<array3> normlist;

    // Loop through the lines of the file
    while(fgets(line, sizeof(line), file)) {
        if (line[0] == 'v') {
            // Position Vector
            if (line[1] == ' ') {
                array3 pos;
                sscanf(line + 2, "%f %f %f", &pos.vect[0], &pos.vect[1], &pos.vect[2]);
                poslist.push_back(pos);
            // Texture Vector
            } else if (line[1] == 't') {
                array2 uv;
                sscanf(line + 3, "%f %f", &uv.vect[0], &uv.vect[1]);
                uvlist.push_back(uv);
            // Normal Vector
            } else if (line[1] == 'n') {
                array3 norm;
                sscanf(line + 3, "%f %f %f", &norm.vect[0], &norm.vect[1], &norm.vect[2]);
                normlist.push_back(norm);
            }
        // If we have a face, start looking up data and add to temp vertecies
        } else if (line[0] == 'f') {
            int vIndex[3], tIndex[3], nIndex[3];
            // Save the indexes into the variables above, returning the amount of indexes given
            int availableIndex = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                             &vIndex[0], &tIndex[0], &nIndex[0],
                             &vIndex[1], &tIndex[1], &nIndex[1],
                             &vIndex[2], &tIndex[2], &nIndex[2]);
            
            if (availableIndex != 9) {
                printf("Indexes could not be assigned correctly (each face needs to have 3 full vertexes with pos uv norm)");
                fclose(file);
                return false;
            }
            for (int i = 0; i < 3; i++) {
                vertex final;

                // Use the index to form a vertex
                final.position[0] = poslist[vIndex[i] - 1].vect[0];
                final.position[1] = poslist[vIndex[i] - 1].vect[1];
                final.position[2] = poslist[vIndex[i] - 1].vect[2];

                final.texcoord[0] = uvlist[tIndex[i] - 1].vect[0];
                final.texcoord[1] = uvlist[tIndex[i] - 1].vect[1];

                final.normal[0] = normlist[nIndex[i] - 1].vect[0];
                final.normal[1] = normlist[nIndex[i] - 1].vect[1];
                final.normal[2] = normlist[nIndex[i] - 1].vect[2];

                vertexList.push_back(final);
            }

        }
    };
    
    fclose(file);

    #ifdef OBJDEBUG
    for (size_t i = 0; i < vertexList.size(); i++) {
        printf("v%zu %.1f/%.1f/%.1f %.1f/%.1f %.1f/%.1f/%.1f\n",
               i,
               vertexList[i].position[0], vertexList[i].position[1], vertexList[i].position[2],
               vertexList[i].texcoord[0], vertexList[i].texcoord[1],
               vertexList[i].normal[0], vertexList[i].normal[1], vertexList[i].normal[2]);
    }
    #endif
    
    // this shit actually fucking works holy hell fuck nintendo 8}
    return true;
}

void Gmobj::Mesh::loadRender()
{   
    size_t vertexSize = sizeof(vertex);
    AddToBuffer(vboData, vertexSize);
}

void Gmobj::Mesh::renderBuffer()
{
    C3D_BindProgram(&program); // not an error 
	C3D_SetAttrInfo(&attrInf);
	//C3D_SetBufInfo(&bufferInf);
	//C3D_LightEnvBind(&lightEnv);
	C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
	C3D_CullFace(GPU_CULL_BACK_CCW);

    C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    C3D_TexEnvInit(C3D_GetTexEnv(1));
	C3D_TexEnvInit(C3D_GetTexEnv(2));
	C3D_TexEnvInit(C3D_GetTexEnv(3));
	C3D_TexEnvInit(C3D_GetTexEnv(4));
	C3D_TexEnvInit(C3D_GetTexEnv(5));
}

void Gmobj::Mesh::uloadRender()
{
    linearFree(vboData);

    shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

void Gmobj::destroy()
{
    mesh.uloadRender();
}