#include "physics.h"

void collider::setAABB() {
    aabb.min ={ center.x - dimension.x, center.y - dimension.y };
    aabb.max ={ center.x + dimension.x, center.y + dimension.y };
}

void aabb::draw(SDL_Renderer* r) {
    SDL_Rect rect ={ min.x, min.y, max.x-min.x, max.y-min.y };
    SDL_RenderDrawRect(r, &rect);
}

void physicsWorld::addDynamicObject(dynamicObject* obj) {
    obj->id = dynamicObjects.size();
    dynamicObjects.push_back(obj);
}

void physicsWorld::addStaticObject(staticObject* obj) {
    obj->id = staticObjects.size();
    staticObjects.push_back(obj);
}

void physicsWorld::resolveCollisions() {
    for (int col = collidedObjects.size()-1; col >= 0; col--) {
        collision* p_col = &collidedObjects[col];
        if (p_col->normal.x == 1 || p_col->normal.x == -1) {
            p_col->normal.y = 1;
            p_col->normal.x = 0;
        }
        bas::vector2 <float> n = { p_col->normal.x * p_col->depth / 2, p_col->normal.y * p_col->depth / 2 };
        collidedObjects[col].col1->center = bas::subVectors2(collidedObjects[col].col1->center, n);
        collidedObjects.erase(collidedObjects.begin()+col);
    }
}

bool AABBvsAABB(aabb a, aabb b) {
    // No overlap
    if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

    // Overlap
    return true;
}

void physicsWorld::getCollisions() {
    for (int obj1 = 0; obj1 < dynamicObjects.size(); obj1++)
    for (int obj2 = 0; obj2 < staticObjects.size(); obj2++) {
        dynamicObject* p_obj1 = dynamicObjects[obj1];
        staticObject* p_obj2 = staticObjects[obj2];
        p_obj1->collider.setAABB();
        p_obj2->collider.setAABB();

        if (AABBvsAABB(p_obj1->collider.aabb, p_obj2->collider.aabb)) {
            collision c(&p_obj1->collider, &p_obj2->collider);
            c.getData();
            collidedObjects.push_back(c);
        }
    }
}

collision::collision(collider* col1I, collider* col2I) {
    col1 = col1I;
    col2 = col2I;
}

void collision::getData() {
    bas::vector2 <float> n = bas::subVectors2 <float> (col2->center, col1->center);
    float a_extent = (col1->aabb.max.x - col1->aabb.min.x) / 2;
    float b_extent = (col2->aabb.max.x - col2->aabb.min.x) / 2;

    float x_overlap = a_extent + b_extent - abs(n.x);

    a_extent = (col1->aabb.max.y - col1->aabb.min.y) / 2;
    b_extent = (col2->aabb.max.y - col2->aabb.min.y) / 2;

    float y_overlap = a_extent + b_extent - abs(n.y);

    if (x_overlap < y_overlap) {
        if (n.x < 0)
            normal.x =-1;
        else
            normal.x = 1;
        depth = x_overlap;
    }
    else {
        if (n.y < 0)
            normal.y =-1;
        else
            normal.y = 1;
        depth = y_overlap;
    }
}
