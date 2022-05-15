#include "raytracer.h"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <omp.h>

#define ran() ( double( rand() % 32768 ) / 32768 )

const double SPEC_POWER = 20;
const int MAX_DREFL_DEP = 2;
const int MAX_RAYTRACING_DEP = 10;
const int HASH_FAC = 7;
const int HASH_MOD = 10000007;
const int NUM_RESAMPLE = 3;

Raytracer::Raytracer() {
    light_head = NULL;
    background_color_top = Color();
    background_color_bottom = Color();
    camera = new Camera;
}

Color Raytracer::CalnDiffusion(CollidePrimitive collide_primitive , int* hash )
{
    Primitive* primitive = collide_primitive.collide_primitive;
    Color color = primitive->GetMaterial()->color;

    // use texture if any
    if (primitive->GetMaterial()->texture != NULL) {
        color = color * collide_primitive.GetTexture();
    }

    Color ret;

    for ( Light* light = light_head ; light != NULL ; light = light->GetNext() ) {
        double shade = light->CalnShade(collide_primitive.C, scene.GetPrimitiveHead(), int(16 * camera->GetShadeQuality()));
        if (shade < EPS) {
            continue;
        }

        // now the light produces some shade on this diffuse point

        // R: light ray from diffuse point to the light
        Vector3 R = ( light->GetO() - collide_primitive.C ).GetUnitVector();
        double dot = R.Dot( collide_primitive.N );
        if (dot > EPS) {
            if (hash != NULL && light->IsPointLight()) {
                *hash = (*hash + light->GetSample()) & HASH_MOD;
            }

            // diffuse light
            if ( primitive->GetMaterial()->diff > EPS ) {
                double diff = primitive->GetMaterial()->diff * dot * shade;
                ret += color * light->GetColor() * diff;
            }
            // specular light
            if ( primitive->GetMaterial()->spec > EPS ) {
                double spec = primitive->GetMaterial()->spec * pow( dot , SPEC_POWER ) * shade;
                ret += color * light->GetColor() * spec;
            }
        }
    }

    return ret;
}

Color Raytracer::CalnReflection(CollidePrimitive collide_primitive , Vector3 ray_V , int dep , int* hash )
{
    ray_V = ray_V.Reflect( collide_primitive.N );
    Primitive* primitive = collide_primitive.collide_primitive;

    // only reflection
    if (primitive->GetMaterial()->drefl < EPS || dep > MAX_DREFL_DEP) {
        return RayTracing(collide_primitive.C, ray_V, dep + 1, hash) * primitive->GetMaterial()->color * primitive->GetMaterial()->refl;
    }
    // diffuse reflection (fuzzy reflection)
    else {
        // TODO: NEED TO IMPLEMENT
        
        // Unit *circle* perpendicular to ray_V.
        // This is different from sampling from a unit sphere -- when projecting the sphere
        // to this circle the points are not uniformly distributed.
        // However, considering the ExpBlur, this approximation may be justified.
        auto baseX = ray_V.GetAnVerticalVector();
        auto baseY = (ray_V * baseX).GetUnitVector();

        // scale the circle according to drefl (fuzzy) value
        baseX *= primitive->GetMaterial()->drefl;
        baseY *= primitive->GetMaterial()->drefl;

        // ADD BLUR
        auto xy = primitive->GetMaterial()->blur->GetXY();
        auto fuzzy_V = ray_V + baseX * xy.first + baseY * xy.second;

        return RayTracing(collide_primitive.C, fuzzy_V, dep + 1, hash) * primitive->GetMaterial()->color * primitive->GetMaterial()->refl;
    }
}

Color Raytracer::CalnRefraction(CollidePrimitive collide_primitive , Vector3 ray_V , int dep , int* hash )
{
    Primitive* primitive = collide_primitive.collide_primitive;
    double n = primitive->GetMaterial()->rindex;
    if (collide_primitive.front) {
        n = 1 / n;
    }

    ray_V = ray_V.Refract( collide_primitive.N , n );
    Color rcol = RayTracing( collide_primitive.C , ray_V , dep + 1 , hash );

    if (collide_primitive.front) {
        return rcol * primitive->GetMaterial()->refr;
    }

    Color absor = primitive->GetMaterial()->absor * -collide_primitive.dist;
    Color trans = Color( exp( absor.r ) , exp( absor.g ) , exp( absor.b ) );
    return rcol * trans * primitive->GetMaterial()->refr;
}

Color Raytracer::RayTracing( Vector3 ray_O , Vector3 ray_V , int dep , int* hash )
{
    if (dep > MAX_RAYTRACING_DEP) {
        return Color();
    }

    Color ret;
    CollidePrimitive collide_primitive = scene.FindNearestPrimitiveGetCollide( ray_O , ray_V );

    if (collide_primitive.isCollide) {
        if (hash != NULL) {
            *hash = (*hash + collide_primitive.collide_primitive->GetSample()) % HASH_MOD;
        }
        Primitive* primitive = collide_primitive.collide_primitive;
        if ( primitive->IsLightPrimitive() ) {
            ret += primitive->GetMaterial()->color;
            //std::cout<<primitive->GetMaterial()->color.r;
        }
        else {
            if (primitive->GetMaterial()->diff > EPS || primitive->GetMaterial()->spec > EPS) {
                ret += CalnDiffusion(collide_primitive, hash);
            }
            if (primitive->GetMaterial()->refl > EPS) {
                ret += CalnReflection(collide_primitive, ray_V, dep, hash);
            }
            if (primitive->GetMaterial()->refr > EPS) {
                ret += CalnRefraction(collide_primitive, ray_V, dep, hash);
            }
        }
    }
    // nothing there, just background
    else {
        auto direction = ray_V.GetUnitVector();
        double t = 0.5 * (direction.z + 1.0);
        ret = background_color_bottom * (1.0 - t) + background_color_top * t;
    }

    if (hash != NULL) {
        *hash = (*hash * HASH_FAC) % HASH_MOD;
    }
    ret.Confine();
    return ret;
}

Primitive* Raytracer::CreateAndLinkLightPrimitive(Primitive* primitive_head)
{
    Light* light_iter = light_head;
    while(light_iter != NULL)
    {
        Primitive* new_primitive = light_iter->CreateLightPrimitive();
        if ( new_primitive != NULL ) {
            new_primitive->SetNext( primitive_head );
            primitive_head = new_primitive;
        }
        light_iter = light_iter->GetNext();
    }
    return primitive_head;
}

void Raytracer::CreateAll()
{
    srand( 1995 - 05 - 12 );
    std::cout << input << std::endl;
    std::ifstream fin( input );

    std::string obj;
    Primitive* primitive_head = nullptr;
    while ( fin >> obj ) {
        if (obj[0] == '#') {
            continue;
        }

        // create a primitive if necessary
        Primitive* new_primitive = nullptr;
        Light* new_light = nullptr;
        if ( obj == "primitive" ) {
            std::string type; fin >> type;
            if ( type == "sphere" ) new_primitive = new Sphere;
            if ( type == "plane" ) new_primitive = new Plane;
            if ( type == "square" ) new_primitive = new Square;
            // TODO: IMPLEMENT HERE
            //if ( type == "cylinder" ) new_primitive = new Cylinder;
            //if ( type == "bezier" ) new_primitive = new Bezier;

            if ( new_primitive != nullptr ) {
                new_primitive->SetNext( primitive_head );
                primitive_head = new_primitive;
            }
        }
        else if ( obj == "light" ) {
            std::string type; fin >> type;
            if ( type == "point" ) new_light = new PointLight;
            if ( type == "square" ) new_light = new SquareLight;
            // TODO: IMPLEMENT HERE
            //if ( type == "sphere" ) new_light = new SphereLight;

            if ( new_light != nullptr ) {
                new_light->SetNext( light_head );
                light_head = new_light;
            }
        }
        else if ( obj != "background" && obj != "camera" ) continue;

        fin.ignore( 1024 , '\n' );

        // read config for this primitive
        std::string order;
        while ( getline( fin , order , '\n' ) ) {
            std::stringstream fin2( order );
            std::string var; fin2 >> var;

            if (var[0] == '#') {
                continue;
            }

            if (var == "end") {
                break;
            }

            // parse configs
            if (obj == "background") {
                if (var == "color=") {
                    background_color_top.Input(fin2);
                    background_color_bottom = background_color_top;
                } else if (var == "top=") {
                    background_color_top.Input(fin2);
                    background_color_bottom = Color(1.0, 1.0, 1.0);
                } else if (var == "bottom=") {
                    background_color_bottom.Input(fin2);
                }
            }
            if ( obj == "primitive" && new_primitive != nullptr) new_primitive->Input( var , fin2 );
            if ( obj == "light" && new_light != nullptr) new_light->Input( var , fin2 );
            if ( obj == "camera" ) camera->Input( var , fin2 );
        }
    }

    scene.CreateScene(CreateAndLinkLightPrimitive(primitive_head));
    camera->Initialize();
}

void Raytracer::Run()
{
    CreateAll();

    Vector3 ray_O = camera->GetO();
    int H = camera->GetH() , W = camera->GetW();

    int** sample = new int*[H];
    for ( int i = 0 ; i < H ; i++ ) {
        sample[i] = new int[W];
        for (int j = 0; j < W; j++) {
            sample[i][j] = 0;
        }
    }

//#ifdef _OPENMP
//#pragma omp parallel for
//#endif
//    //for ( int i = 0 ; i < H ; std::cout << "Sampling:   " << ++i << "/" << H << std::endl )
//    for (int i = 0; i < H; i++) {
//        for (int j = 0; j < W; j++) {
//            Vector3 ray_V = camera->Emit(i, j);
//            Color color = RayTracing(ray_O, ray_V, 1, &sample[i][j]);
//            camera->SetColor(i, j, color);
//        }
//    }

#ifdef _OPENMP
#pragma omp parallel for
#endif
    //for ( int i = 0 ; i < H ; std::cout << "Resampling: " << ++i << "/" << H << std::endl )
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            Color color;
            for (int r = -NUM_RESAMPLE; r <= NUM_RESAMPLE; r++) {
                for (int c = -NUM_RESAMPLE; c <= NUM_RESAMPLE; c++) {
                    Vector3 ray_V = camera->Emit(i + (double)r / (NUM_RESAMPLE * 2 + 1), j + (double)c / (NUM_RESAMPLE * 2 + 1));
                    color += RayTracing(ray_O, ray_V, 1, nullptr) / pow((NUM_RESAMPLE * 2 + 1), 2);
                }
            }
            camera->SetColor(i, j, color);
        }
    }

    for (int i = 0; i < H; i++) {
        delete[] sample[i];
    }
    delete[] sample;

    Bmp* bmp = new Bmp( H , W );
    camera->Output( bmp );
    bmp->Output( output );
    delete bmp;
}

void Raytracer::DebugRun(int w1, int w2, int h1, int h2)
{
    CreateAll();

    Vector3 ray_O = camera->GetO();
    int H = camera->GetH() , W = camera->GetW();
    int** sample = new int*[H];
    for ( int i = 0 ; i < H ; i++ ) {
        sample[i] = new int[W];
        for ( int j = 0 ; j < W ; j++ )
            sample[i][j] = 0;
    }
    h1 = H - h1;
    h2 = H - h2;
    //for ( int i = 0 ; i < H ; std::cout << "Sampling:   " << ++i << "/" << H << std::endl )
    for(int i=h2;i<h1;i++)
        for ( int j = w1 ; j < w2 ; j++ ) {
            Vector3 ray_V = camera->Emit( i , j );
            Color color = RayTracing( ray_O , ray_V , 1 , &sample[i][j] );
            camera->SetColor( i , j , color );
        }

    for ( int i = 0 ; i < H ; i++ )
        delete[] sample[i];
    delete[] sample;

    Bmp* bmp = new Bmp( H , W );
    camera->Output( bmp );
    bmp->Output( output );
    delete bmp;
}

void Raytracer::MultiThreadFuncCalColor(int i, int** sample)
{
    srand(i);
    Vector3 ray_O = camera->GetO();
    int W = camera->GetW();
    for (int j = 0 ; j < W ; j++) {
        Vector3 ray_V = camera->Emit( i , j );
        Color color = RayTracing( ray_O , ray_V , 1 , &sample[i][j] );
        camera->SetColor( i , j , color );
    }
}

void Raytracer::MultiThreadFuncResampling(int i, int** sample)
{
    Vector3 ray_O = camera->GetO();
    int H = camera->GetH() , W = camera->GetW();
    for ( int j = 0 ; j < W ; j++ ) {
        if ( ( i == 0 || sample[i][j] == sample[i - 1][j] ) && ( i == H - 1 || sample[i][j] == sample[i + 1][j] ) &&
                ( j == 0 || sample[i][j] == sample[i][j - 1] ) && ( j == W - 1 || sample[i][j] == sample[i][j + 1] ) ) continue;

        Color color;
        for (int r = -NUM_RESAMPLE; r <= NUM_RESAMPLE; r++) {
            for (int c = -NUM_RESAMPLE; c <= NUM_RESAMPLE; c++) {
                Vector3 ray_V = camera->Emit(i + (double)r / (NUM_RESAMPLE * 2 + 1), j + (double)c / (NUM_RESAMPLE * 2 + 1));
                color += RayTracing(ray_O, ray_V, 1, nullptr) / pow((NUM_RESAMPLE * 2 + 1), 2);
            }
        }
        camera->SetColor( i , j , color );
    }
}


void Raytracer::MultiThreadRun()
{
    CreateAll();

    Vector3 ray_O = camera->GetO();
    int H = camera->GetH() , W = camera->GetW();
    int** sample = new int*[H];
    for ( int i = 0 ; i < H ; i++ ) {
        sample[i] = new int[W];
        for ( int j = 0 ; j < W ; j++ )
            sample[i][j] = 0;
    }

    std::vector<std::thread> threads(H);
    //for ( int i = 0 ; i < H ; std::cout << "Sampling:   " << ++i << "/" << H << std::endl )
    for (int i = 0; i < H; i++) {
        threads[i] = std::thread(&Raytracer::MultiThreadFuncCalColor, this, i, sample);
    }
    for (int i = 0; i < H; i++) {
        threads[i].join();
    }

    //for ( int i = 0 ; i < H ; std::cout << "Resampling: " << ++i << "/" << H << std::endl )
    for (int i = 0; i < H; i++) {
        threads[i] = std::thread(&Raytracer::MultiThreadFuncResampling, this, i, sample);
    }
    for (int i = 0; i < H; i++) {
        threads[i].join();
    }

    for (int i = 0; i < H; i++) {
        delete[] sample[i];
    }
    delete[] sample;

    Bmp* bmp = new Bmp( H , W );
    camera->Output( bmp );
    bmp->Output( output );
    delete bmp;
}
