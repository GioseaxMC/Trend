#pragma once

#include <SFML/graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <math.h>
#include <unordered_map>
#include <fstream>
#include <sstream>

#ifdef INCLUDE_JSON
// #include "giotool/thirdparty/json.cpp"
// using json = nlohmann::json;
#include "giotool/thirdparty/json11.cpp"
using namespace json11;
#endif

using namespace sf;

#ifdef USE_NAMESPACES
using namespace std;
#endif //USE_NAMESPACES

#ifndef FPS
    #define FPS 240
#endif //FPS

#define in ,
#define dtFps 60
#define dtExp (1.0f / dtFps)
#define elapsedMax (1.0f / FPS*3)
#define elapsedMin (1.0f / FPS*0.5)
#define makeDelta(elapsed) (min(elapsedMax, elapsed) / dtExp)

#define handle_events_for(...) \
    handle_events_exp(handle_events_for_impl, (__VA_ARGS__))
#define handle_events_exp(X, A) X A
#define handle_events_for_impl(X, B) \
    sf::Event X; while (B.pollEvent(X))

#define setScaleXY(xx) setScale(xx, xx)

#define $notnull(statement)  if ( statement)
#define  $isnull(statement)  if (!statement)

#define $   auto
#define let auto
#define var auto

static inline string readFile(const string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

#include "giotool/PerspectiveShader.cpp"
#include "giotool/TextureArchive.cpp"
#include "giotool/Camera.cpp"
#include "giotool/Face.cpp"
#include "giotool/Entity3D.cpp"
#include "giotool/ModelArchive.cpp"
#include "giotool/Cube.cpp"
#include "giotool/Scene3D.cpp"
