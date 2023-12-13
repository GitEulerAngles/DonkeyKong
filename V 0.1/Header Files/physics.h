#include "basic.h"
#include <vector>

struct aabb {
    bas::vector2 <float> min;
    bas::vector2 <float> max;

    void draw(SDL_Renderer* r);
};

struct collider {
    bas::vector2 <float> center;
    bas::vector2 <float> dimension;
    aabb aabb;
    void setAABB();
};

struct rigidBody {
    bas::vector2 <float> position;
    bas::vector2 <float> velocity;
    bas::vector2 <int> slope;
};

struct dynamicObject {
    rigidBody rigidBody;
    collider collider;
    Uint16 id;
};

struct staticObject {
    collider collider;
    Uint16 id;
};

struct collision {
    collider* col1;
    collider* col2;

    bas::vector2 <int> normal ={ 0,0 };
    float depth;

    collision(collider* col1I, collider* col2I);
    void getData();
};

class physicsWorld {
public:
    void addStaticObject(staticObject* obj);
    void addDynamicObject(dynamicObject* obj);
    void getCollisions();
    void resolveCollisions();
    std::vector <dynamicObject*> dynamicObjects;

private:
    std::vector <collision>      collidedObjects;
    std::vector <staticObject*>  staticObjects;
};
