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

    void makeFace(Vector2f screenPos[4], float depth[4], Vector2f uv[4], Texture& texture) {
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
    };

    void prepare(
    Camera& camera,
    Vector3f points[4],
    Texture& texture,
    Vector2f uv[4]) {

        #define CLIP_AT 5
        Vector2f screenPos[4];
        float depth[4];
        int behind[4] = {};
        int behind_count = 0;
        for (int i=0; i<4; ++i) {
            depth[i] = camera.getDepth(points[i]);
            if (depth[i] <= CLIP_AT) {
                behind[i] = i+4;
                behind_count++;
            }
        }
        
        #define resetUv() for(int ii=0; ii<4; ++ii) drawUv[ii] = uv[ii];

        #define getscp(ps) for (int ii=0; ii<4; ++ii) {   \
            screenPos[ii] = camera.clipToScreen(ps[ii]); \
            depth[ii] = ps[ii].z;                        \
        }        
        
        for (int i=0; i<4; ++i)
            points[i] = camera.worldToClip(points[i]);
        
        #define domod(i, q) ((i)+q)%q
        #define setup_clipping() \
            i-=4; \
            int bef=domod(i-1,4); \
            int aft=domod(i+1,4);

        switch (behind_count) {
            case 1: {
                Vector3f other[4] = {};
                Vector2f uv1[4] = {};
                Vector2f uv2[4] = {};
                memcpy(other, points, sizeof(other));
                memcpy(uv1, uv, sizeof(uv1));
                memcpy(uv2, uv, sizeof(uv2));

                for (int i : behind) {
                    if (i) {
                        setup_clipping();

                        float tb=(CLIP_AT-points[bef].z) / (points[i].z - points[bef].z);
                        float ta=(CLIP_AT-points[aft].z) / (points[i].z - points[aft].z);

                        other[i] = points[bef] + (points[i] - points[bef])*tb;
                        points[i] = points[aft] + (points[i] - points[aft])*ta;
                        other[aft] = points[i];
                        
                        uv2[i] = uv1[bef] + (uv1[i] - uv2[bef])*tb;
                        uv1[i] = uv1[aft] + (uv1[i] - uv2[aft])*ta;
                        uv2[aft] = uv1[i];

                        getscp(points);
                        makeFace(screenPos, depth, uv1, texture);
                        getscp(other);
                        makeFace(screenPos, depth, uv2, texture);
                    }
                }
            } break;

            case 2: {
                Vector2f uv1[4] = {};
                memcpy(uv1, uv, sizeof(uv1));

                for (int i : behind) {
                    if (i) {
                        setup_clipping();
                        int j = *find_if(behind, behind+4, [&](int x) -> bool {
                            return x != i+4 and x; });

                        let take = (j-4==bef) ? aft : bef;

                        float t=(CLIP_AT-points[take].z) / (points[i].z - points[take].z);

                        points[i] = points[take] + (points[i] - points[take])*t;
                        uv1[i] = uv[take] + (uv[i] - uv[take])*t;
                    }
                }
                getscp(points);
                makeFace(screenPos, depth, uv1, texture);
            } break;

            case 3: {
                Vector2f uv1[4] = {};
                memcpy(uv1, uv, sizeof(uv1));

                for (let& i : behind) {
                    if (not i) {
                        int j = distance(behind, &i);

                        int bef=domod(j-1,4);
                        int aft=domod(j+1,4);
                        int opp=domod(j+2,4);

                        float tb = (CLIP_AT-points[j].z) / (points[bef].z - points[j].z);
                        float ta = (CLIP_AT-points[j].z) / (points[aft].z - points[j].z);

                        points[bef] = points[j] + (points[bef] - points[j])*tb;
                        points[aft] = points[j] + (points[aft] - points[j])*ta;
                        points[opp] = points[bef] + (points[opp] - points[j])*0.001f;

                        uv1[bef] = uv[j] + (uv[bef] - uv[j])*tb;
                        uv1[aft] = uv[j] + (uv[aft] - uv[j])*ta;
                        uv1[opp] = uv1[bef];
                    }
                }
                getscp(points);
                makeFace(screenPos, depth, uv1, texture);
            } break;

            case 0: {
                getscp(points);
                makeFace(screenPos, depth, uv, texture);
            } break;
        }
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
