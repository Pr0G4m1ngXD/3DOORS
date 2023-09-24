#include <vector>
#include "gmobj.h"

/// @brief Manage multiple objects at once, allow for object hierachry
class ObjectManager
{
private:
    std::vector<Gmobj> loadedObjects;
public:
    int addObject(Gmobj gmobj);
    void renderAll(C3D_Mtx *mtxProj, C3D_Mtx *mtxMod);
    ObjectManager() {};
    ~ObjectManager() {};
};