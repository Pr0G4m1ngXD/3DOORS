#include "objman.h"
#include <citro3d.h>
#include <3ds.h>
#include "define.h"

int ObjectManager::addObject(Gmobj gmobj)
{
    gmobj.init();
    loadedObjects.push_back(gmobj);
return 0;
}

void ObjectManager::renderAll(const C3D_Mtx *mtxProj, const C3D_Mtx *mtxMod)
{
    int offset = 0, objcount = 0;
    for (size_t i = 0; i < loadedObjects.size(); i++) { // for loop through vector
    	int objVcount = loadedObjects[i].mesh.vertexList.size();
    	if (objVcount > 0) {
			loadedObjects[i].mesh.renderBuffer();

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
}
