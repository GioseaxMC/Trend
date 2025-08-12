#pragma once

class FaceRenderer {
public:
    
    struct FaceInfo {
        Texture* tex;
        Vector2f pos[4];
        float depths[4];
        Vector2f uv[4];
    };
    
    vector<FaceInfo> faces;
    
    FaceRenderer() {};

    static bool shouldCull(Vector2f a, Vector2f b, Vector2f c) {
        return
            (b.x - a.x) * (c.y - a.y) -
            (c.x - a.x) * (b.y - a.y) >= 0;
    };

    void prepare(
    Camera& camera,
    Vector3f points[4],
    Texture& texture,
    Vector2f uv[4]) {
        Vector2f screenPos[4];
        float depth[4];
        for (int i=0; i<4; ++i) {
            depth[i] = camera.getDepth(points[i]);
            screenPos[i] = camera.worldToScreen(points[i]);
        }
        
        if (shouldCull(screenPos[0], screenPos[1], screenPos[2]))
            return;
        
        FaceInfo faceInfo = {
            &texture
        };

        for (int i=0; i<4; ++i) {
            faceInfo.pos[i]    = screenPos[i];
            faceInfo.depths[i] = depth[i];
            faceInfo.uv[i] = uv[i];
        }

        faces.emplace_back(faceInfo);

        // FROM HERE STORE INFORMATION INTO FaceRenderer class or
        // make Face store shit and then draw at the and
        // rename Face to FaceRenderer
        // below is FaceRenderer::draw
        // above is FaceRenderer::prepare
    };

    void draw(RenderWindow& window) {
        sort(faces.begin(), faces.end(),
            [&](FaceInfo& x, FaceInfo& y) -> bool {
                return 
                *min_element(x.depths, x.depths+4) -
                *min_element(y.depths, y.depths+4) > 0;
            }
        );
        for (auto& [texture, screenPos, depths, uvCoords] : faces) {
            sf::VertexArray va(sf::Quads, 4);
            
            for (int i = 0; i < 4; ++i) {
                float w = depths[i];
                float invW = 1.f / w;
                
                va[i].position = screenPos[i];
                
                va[i].texCoords.x = uvCoords[i].x * invW;
                va[i].texCoords.y = uvCoords[i].y * invW;
                
                uint32_t encoded = (uint32_t)(invW * 1000000.f);
                uint32_t *colorint = (uint32_t*)&va[i].color;
                *colorint = encoded;
            }
            
            Shader* shader = perspectiveShader.get();
            shader->setUniform("tex", *texture);
            window.draw(va, shader);
        }
        faces.clear();
    };
};
