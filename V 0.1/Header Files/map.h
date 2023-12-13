#include "physics.h"

class map {
public:
    staticObject staticObjects[66];
    std::vector <dynamicObject> barrels;
    std::vector <bas::animatedtexture> barrelsTextures;

    void setupColliders(physicsWorld* world);
};
