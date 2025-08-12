#pragma once

struct ElementInfo {
    Vector3f points[8];
    Vector2f uvs[6][4];
    int textureID[6];
};

struct ModelDescriptor {
    vector<Texture*> textures;
    vector<ElementInfo> elements;
};

class ModelArchive {
public:
    unordered_map<string, ModelDescriptor> container;
    TextureArchive& texs;

    ModelArchive(TextureArchive& t):
        texs(t)
    {};

    ModelDescriptor loadFromString(const string& content) {

        ModelDescriptor md;

        string err;
        let json = Json::parse(content, err);
        
        vector<int> texture_log;
        

        for (const let& [key, value]:
        json["textures"].object_items()) {
            if (key == "particle")
                continue;

            int id = stoi(key);
            texture_log.push_back(id);
            Texture& tex = texs.createTextureFromFile(
                value.string_value()
            );
            md.textures.push_back(&tex);
        }
        
        int elementId = 0;
        for (const let& item:
        json["elements"].array_items()) {

            float p[2][3];

            int xxx = 0;
            for (let& fi : item["from"].array_items())
                p[0][xxx++] = fi.number_value()*100/16;
            for (let& ti : item["to"].array_items())
                p[0][xxx++] = ti.number_value()*100/16;

            Vector3f tpoints[8] = {
                Vector3f(p[0][0], p[1][1], p[0][2]),
                Vector3f(p[1][0], p[1][1], p[0][2]),
                Vector3f(p[1][0], p[0][1], p[0][2]),
                Vector3f(p[0][0], p[0][1], p[0][2]),

                Vector3f(p[0][0], p[1][1], p[1][2]),
                Vector3f(p[1][0], p[1][1], p[1][2]),
                Vector3f(p[1][0], p[0][1], p[1][2]),
                Vector3f(p[0][0], p[0][1], p[1][2])
            };
            
            ElementInfo ei;

            for (int i=0; i<8; ++i) {
                ei.points[i] = tpoints[i];
            }

            // ROTATE HERE
            
            int i = 0;
            for (let& [key, face]:
            item["faces"].object_items()) {
                string strid = face["texture"].string_value();
                ei.textureID[i] = distance(
                    texture_log.begin(),
                    find(
                        texture_log.begin(),
                        texture_log.end(),
                        stoi(strid.substr(1))
                    )
                );
                
                let uv = face["uv"].array_items();
                #define ff .number_value()/16.f
                Vector2f tuvs[] = {
                    Vector2f(uv[0]ff, uv[3]ff),
                    Vector2f(uv[2]ff, uv[3]ff),
                    Vector2f(uv[2]ff, uv[1]ff),
                    Vector2f(uv[0]ff, uv[1]ff),
                };
                memcpy(ei.uvs[i], tuvs, sizeof(tuvs));
                #undef ff
                i++;
            }

            md.elements.push_back(ei);

            ++elementId;
        }
        return md;
    };

    ModelDescriptor loadFromFile(const string& filename) {
        string content = readFile(filename);
        return loadFromString(content);
    };

    ModelDescriptor createModel(const string& filename) {
        if (container.find(filename) == container.end()) {
            container[filename] = loadFromFile(filename);
        }
        return container[filename];
    };
};
