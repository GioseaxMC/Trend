class Scene3D {
public:
    Camera& camera;
    vector<unique_ptr<Entity3D>> entities;
    RenderWindow& window;
    FaceRenderer fr;

    Scene3D(RenderWindow& theWindow, Camera& theCamera):
        camera(theCamera),
        window(theWindow)
    {};
    
    template<typename T>
    void addEntity(T theEntity) {
        entities.push_back(make_unique<T>(theEntity));
    };

    void draw() {
        for(auto& entity : entities) {
            entity->draw(camera, fr);
        }
        fr.draw(window);
    };
};
