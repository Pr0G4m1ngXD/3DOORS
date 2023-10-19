#pragma once

#include <vector>
#include "gmobj.h"
#include "renderman.h"

/// @brief Manage multiple objects at once, allow for object hierachry, and manage graphics objects for those objects
class ObjectManager
{
friend class Gmobj;
friend class Gmobj::Mesh;
friend class RenderManager;
protected:
    std::vector<Gmobj> loadedObjects;
public:
    int addObject(Gmobj gmobj);
    // render moving to renderman
    ObjectManager() {};
    ~ObjectManager() {};
};