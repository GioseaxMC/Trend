#pragma once

class Camera {
public:
    Vector3f position;
    Vector2f direction;
    // RenderWindow& window;
    float nearPlane;
    float sin_x = sin(direction.x);
    float sin_y = sin(direction.y);

    float cos_x = cos(direction.x);
    float cos_y = cos(direction.y);

    Camera():
        position  (Vector3f(1,0,1)),
        direction (Vector2f(0,0)),
        nearPlane (200)
    {
        sin_x = sin(direction.x);
        sin_y = sin(direction.y);
        cos_x = cos(direction.x);
        cos_y = cos(direction.y);
    };
    
    void move(float dt) {
        float inputX = Keyboard::isKeyPressed(Keyboard::D) - 
            Keyboard::isKeyPressed(Keyboard::A);
        float inputY = Keyboard::isKeyPressed(Keyboard::W) - 
            Keyboard::isKeyPressed(Keyboard::S);
        
        float dirX = Keyboard::isKeyPressed(Keyboard::Up) - 
            Keyboard::isKeyPressed(Keyboard::Down);
        float dirY = Keyboard::isKeyPressed(Keyboard::Right) - 
            Keyboard::isKeyPressed(Keyboard::Left);

        float speed = 10;
        float dirSpeed = 0.1;
        
        direction += Vector2f(dirSpeed*dirX*dt, dirSpeed*dirY*dt);
        direction.x = min(3.14f/2, max(-3.14f/2, direction.x));

        if (dirX or dirY) {
            sin_x = sin(direction.x);
            sin_y = sin(direction.y);
            cos_x = cos(direction.x);
            cos_y = cos(direction.y);
        }
        
        float movementX = inputX*speed*dt;
        float movementY = inputY*speed*dt;
        position += Vector3f(
            (movementX*cos_y+
            +movementY*sin_y),
            0,
            (movementY*cos_y+
            -movementX*sin_y)
        );
    };
    
    Vector3f worldToClip(Vector3f coords) {
        coords.z = -coords.z;
        coords.y = -coords.y;
        coords -= position;

        Vector3f oc = coords;

        coords.x = cos_y * oc.x - sin_y * oc.z;
        coords.z = cos_y * oc.z + sin_y * oc.x;

        oc = coords;

        coords.y = cos_x * oc.y + sin_x * oc.z;
        coords.z = cos_x * oc.z - sin_x * oc.y;
        return coords;
    }

    Vector2f clipToScreen(Vector3f coords) {
        float depth = coords.z;
        return Vector2f(
            (coords.x) * nearPlane / depth,
            (coords.y) * nearPlane / depth
        );
    };

    float getDepth(Vector3f coords) {
        coords.z = -coords.z;
        coords.y = -coords.y;
        coords -= position;
        float depth = cos_y * coords.z + sin_y * coords.x;
        depth = cos_x * depth - sin_x * coords.y;
        return depth; 
    };
};
