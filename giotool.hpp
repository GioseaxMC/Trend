#pragma once

#include <SFML/graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <memory>
#include <math.h>

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
#define elapsedMax (1.0f / FPS*2)
#define elapsedMin (1.0f / FPS*0.5)
#define makeDelta(elapsed) (min(elapsedMax, elapsed) / dtExp)

#define handle_events_for(...) handle_events_exp(handle_events_for_impl, (__VA_ARGS__))
#define handle_events_exp(X, A) X A
#define handle_events_for_impl(X, B) sf::Event X; while (B.pollEvent(X))

#define setScaleXY(xx) setScale(xx, xx)

#define $notnull(statement)  if ( statement)
#define  $isnull(statement)  if (!statement)

#include "giotool/PerspectiveShader.cpp"
#include "giotool/Camera.cpp"
#include "giotool/Entity3D.cpp"
#include "giotool/Face.cpp"
#include "giotool/Cube.cpp"
#include "giotool/Scene3D.cpp"
