#pragma once

class Face : Entity3D {
public:

    static bool shouldCull(Vector2f a, Vector2f b, Vector2f c) {
        return
            (b.x - a.x) * (c.y - a.y) -
            (c.x - a.x) * (b.y - a.y) >= 0;
    };

    static void drawFace(RenderWindow& window, Camera& camera, Vector3i points[4], Texture& texture) {
        Vector2f screenPos[4];
        float depth[4];
        for (int i=0; i<4; ++i) {
            depth[i] = camera.getDepth(points[i]);
            screenPos[i] = camera.worldToScreen(points[i]);
        }
        
        if (shouldCull(screenPos[0], screenPos[1], screenPos[2]))
            return;
        
        // FROM HERE STORE INFORMATION INTO FaceRenderer class or
        // make Face store shit and then draw at the end (rename Face to FaceRenderer
        // below is FaceRenderer::draw
        // above is FaceRenderer::prepare

        sf::VertexArray va(sf::Quads, 4);
        
        Vector2f uvCoords[4] = {
            {0.f, 1.f},  // Bottom-left
            {1.f, 1.f},  // Bottom-right
            {1.f, 0.f},  // Top-right
            {0.f, 0.f},  // Top-left
        };
        
        for (int i = 0; i < 4; ++i) {
            float w = depth[i];
            float invW = 1.f / w;
            
            va[i].position = screenPos[i];
            
            va[i].texCoords.x = uvCoords[i].x * invW;
            va[i].texCoords.y = uvCoords[i].y * invW;
            
            uint32_t encoded = (uint32_t)(invW * 1000000.f);
            uint32_t *colorint = (uint32_t*)&va[i].color;
            *colorint = encoded;
        }
        
        Shader* shader = perspectiveShader.get();
        shader->setUniform("tex", texture);
        
        window.draw(va, shader);
    };
};
