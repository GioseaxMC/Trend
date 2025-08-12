#pragma once

class TextureArchive {
public:
    unordered_map<string, unique_ptr<Texture>> container;

    TextureArchive() {};

    sf::Texture& createTextureFromFile(const string& fname) {

        if (container.find(fname) != container.end()) {
            return *container[fname];
        }

        container[fname] = make_unique<Texture>();

        sf::Texture& tex = *container[fname];

        if (!tex.loadFromFile(fname+".png")) {
            if (!tex.loadFromFile("data/default.png")) {
                exit(-1);
            }
        }

        return tex;
    };
};
