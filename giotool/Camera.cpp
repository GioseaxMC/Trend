#pragma once

class Camera {
public:
    Vector3i position;
    Vector2f direction;
    // RenderWindow& window;
    float nearPlane;
    float sin_x = sin(direction.x);
    float sin_y = sin(direction.y);

    float cos_x = cos(direction.x);
    float cos_y = cos(direction.y);

    Camera():
        position  (Vector3i(1,0,1)),
        direction (Vector2f(0,0)),
        // window (theWindow),
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

        int speed = 10;
        float dirSpeed = 0.1;

        sin_x = sin(direction.x);
        sin_y = sin(direction.y);
        cos_x = cos(direction.x);
        cos_y = cos(direction.y);
        
        float movementX = inputX*speed*dt;
        float movementY = inputY*speed*dt;
        position += Vector3i(
            (movementX*cos_y+
            +movementY*sin_y),
            0,
            (movementY*cos_y+
            -movementX*sin_y)
        );
        direction += Vector2f(dirSpeed*dirX*dt, dirSpeed*dirY*dt);
    };

    Vector2f worldToScreen(Vector3i coords) {
        coords -= position;
        
        Vector3i oc = coords;

        coords.x = cos_y * oc.x - sin_y * oc.z;
        coords.z = cos_y * oc.z + sin_y * oc.x;

        float depth = coords.z;
        if (depth >= 0) return Vector2f(
            (coords.x) * nearPlane / depth,
            (coords.y) * nearPlane / depth
        );
        else return Vector2f(
            (coords.x) * nearPlane * abs(depth),
            (coords.y) * nearPlane * abs(depth)
        );
    };

    float getDepth(Vector3i coords) {
        coords -= position;
        // float depth = coords.z;
        float depth = cos_y * coords.z + sin_y * coords.x;
        if (depth >= 0) return depth;
        else return abs(1/depth);
    };
};
