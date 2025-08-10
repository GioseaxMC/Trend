#define USE_NAMESPACES

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
    
    Texture texture;
    texture.loadFromFile("image.png");

    Camera camera;
    Clock clock;
    Scene3D scene(window, camera);
    scene.addEntity(Cube(Vector3i(100,10,100), texture));
    scene.addEntity(Cube(Vector3i(100,10,-100), texture));
    scene.addEntity(Cube(Vector3i(-100,10,100), texture));
    scene.addEntity(Cube(Vector3i(-100,10,-100), texture));

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        float delta = makeDelta(elapsed);
        
        handle_events_for(event in window) {
            if (event.type == Event::Closed)
                window.close();
        }
        

        camera.move(delta);
        cout << "Direction: X: " << camera.direction.x << " Y: " << camera.direction.y << "\n";
        
        window.clear(Color::White);
        scene.draw();
        window.display();
    }
};
