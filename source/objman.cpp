#include "objman.h"

int ObjectManager::addObject(Gmobj gmobj)
{
    gmobj.mesh.loadMesh();
    loadedObjects.push_back(gmobj);
return 0;
}
