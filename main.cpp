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
    ModelArchive models = texs;

    Camera camera;
    Clock clock;
    Scene3D scene(window, camera);
    
    const string model = "models/block.json";

    scene.addEntity(
        Model(
            Vector3f(100,-120,100),
            models.createModel(model)
        )
    );

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        float delta = makeDelta(elapsed);
        
        handle_events_for(event in window) {
            if (event.type == Event::Closed)
                window.close();
        }
        
        camera.move(delta);
        
        window.clear(Color::White);
        scene.draw();
        window.display();
    }
};
