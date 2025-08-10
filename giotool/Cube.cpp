class Cube : public Entity3D {
public:
    Vector3i position;
    Texture& texture;

    Cube(Vector3i thePosition, Texture& theTexture):
        position(thePosition),
        texture(theTexture)
    {}; // TODO: support multiple textures
    
    void draw(RenderWindow& window, Camera& camera) {
        Vector3i points[8] = {
            position + Vector3i(000,100,000),
            position + Vector3i(100,100,000),
            position + Vector3i(100,000,000),
            position + Vector3i(000,000,000),

            position + Vector3i(000,100,100),
            position + Vector3i(100,100,100),
            position + Vector3i(100,000,100),
            position + Vector3i(000,000,100),
        };

        int faces[][4] = {
            {4, 5, 1, 0},
            {3, 2, 6, 7},
            {4, 0, 3, 7},
            {5, 4, 7, 6},
            {1, 5, 6, 2},
            {0, 1, 2, 3},
        };
        
        for (auto& face : faces) {
            Vector3i fpoints[4] = {
                points[face[0]],
                points[face[1]],
                points[face[2]],
                points[face[3]],
            };
            Face::drawFace(window, camera, fpoints, texture);
        }
    };
};
