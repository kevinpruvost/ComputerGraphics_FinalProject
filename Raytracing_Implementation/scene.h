#ifndef SCENE_H
#define SCENE_H

#include<string>
#include<fstream>
#include<sstream>

#include"color.h"
#include"vector3.h"
#include"primitive.h"
#include"light.h"
#include"camera.h"

class Scene {
    Primitive* primitive_head;

public:
    Scene();
    ~Scene();

    Primitive* GetPrimitiveHead() { return primitive_head; }

    void CreateScene(Primitive* primitive_head_p);
    CollidePrimitive FindNearestPrimitiveGetCollide( Vector3 ray_O , Vector3 ray_V );
};

#endif
