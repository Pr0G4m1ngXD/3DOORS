#include "objman.h"
#include <citro3d.h>
#include <3ds.h>
#include "define.h"
#include "types.h"
#include "vshader_shbin.h"

int ObjectManager::addObject(Gmobj gmobj)
{
    gmobj.mesh.loadMesh();
	size_t vertexSize = sizeof(vertex);

    size_t bufSize = gmobj.mesh.vertexList.size() * vertexSize;
    gmobj.mesh.vboData = linearAlloc(bufSize);
    memcpy(gmobj.mesh.vboData, gmobj.mesh.vertexList.data(), bufSize);
    BufInfo_Add(&bufferInf, gmobj.mesh.vboData, vertexSize, 3, 0x210);

    loadedObjects.push_back(gmobj);
return 0;
}

// see how i laugh at you
void ObjectManager::renderAll(C3D_Mtx *mtxProj, C3D_Mtx *mtxMod)
{
    int offset = 0, objcount = 0;
    for (size_t i = 0; i < loadedObjects.size(); i++) { // for loop through vector
    	int objVcount = loadedObjects[i].mesh.vertexList.size();
    	if (objVcount > 0) {
			// no youll never understand
			loadedObjects[i].mesh.renderBuffer();

			// no youll never understand
			// Calculate the modelView matrix
			Mtx_Identity(mtxMod);
			//Mtx_Translate(&modelView, player.position.x, -player.visualHeight - player.position.y ,player.position.z - 2.0f, true); position is currently not important

			// no youll never ever ever understand
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, loadedObjects[i].mesh.uLoc_projection, mtxProj);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, loadedObjects[i].mesh.uLoc_modelView,  mtxMod);
			
        	C3D_DrawArrays(GPU_TRIANGLES, offset, objVcount);
        	offset += objVcount;
    	}
		objcount++;
	}
    #ifdef VLISTDEBUG
	printf("vlistcount: %i \n", offset); // \x1b[10;1H
	printf("objeccount: %i \n", objcount); // \x1b[10;1H
	#endif
} // hawaii part 2 ;_;
