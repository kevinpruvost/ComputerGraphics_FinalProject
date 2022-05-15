#ifndef RAYTRACER_H
#define RAYTRACER_H

#include<string>
#include<vector>

#include"scene.h"
#include"bmp.h"

extern const double SPEC_POWER;
extern const int MAX_DREFL_DEP;
extern const int MAX_RAYTRACING_DEP;
extern const int HASH_FAC;
extern const int HASH_MOD;

class Raytracer {
    std::string input , output;
    Scene scene;
    Light* light_head;
    Color background_color_top;
    Color background_color_bottom;
    Camera* camera;
    Color CalnDiffusion( CollidePrimitive collide_primitive , int* hash );
    Color CalnReflection( CollidePrimitive collide_primitive , Vector3 ray_V , int dep , int* hash );
    Color CalnRefraction( CollidePrimitive collide_primitive , Vector3 ray_V , int dep , int* hash );
    Color RayTracing( Vector3 ray_O , Vector3 ray_V , int dep , int* hash );

public:
    Raytracer();
    ~Raytracer() {}

    void SetInput( std::string file ) { input = file; }
    void SetOutput( std::string file ) { output = file; }
    void CreateAll();
    Primitive* CreateAndLinkLightPrimitive(Primitive* primitive_head);
    void Run();
    void DebugRun(int w1, int w2, int h1, int h2);
    void MultiThreadRun();
    void MultiThreadFuncCalColor(int i, int** sample);
    void MultiThreadFuncResampling(int i, int** sample);
    int GetH() const { return camera->GetH(); }
    int GetW() const { return camera->GetW(); }
};

#endif
