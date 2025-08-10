class Entity3D {
public:
    Vector3f position;

    Entity3D() {};

    virtual void draw(RenderWindow& window, Camera& camera) {};
};
