#define USE_NAMESPACES
#define INCLUDE_JSON

#include <iostream>
#include "giotool.hpp"

int main() {
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(
        VideoMode(1290, 720),
        "Sigma boy",
        Style::Default,
        settings );
        window.setFramerateLimit(FPS);
        window.setView(View(Vector2f{0,0}, Vector2f{1290, 720}));
    
    TextureArchive texs;
    Texture& texture = texs.createTextureFromFile("data/image.png");

    Camera camera;
    Clock clock;
    Scene3D scene(window, camera);

    scene.addEntity(Cube(Vector3f(100,10,100), texture));
    scene.addEntity(Cube(Vector3f(100,10,-100), texture));
    scene.addEntity(Cube(Vector3f(-100,10,100), texture));
    scene.addEntity(Cube(Vector3f(-100,10,-100), texture));

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        float delta = makeDelta(elapsed);
        
        handle_events_for(event in window) {
            if (event.type == Event::Closed)
                window.close();
        }
        
        camera.move(delta);
        
        window.clear(Color::Cyan);
        scene.draw();
        window.display();
    }
};
