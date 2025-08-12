#pragma once

class TextureArchive {
public:
    vector<unique_ptr<Texture>> container;

    TextureArchive() {};

    sf::Texture& createTextureFromFile(const string& fname) {
        container.emplace_back(make_unique<Texture>());

        sf::Texture& tex = *container.back();

        if (!tex.loadFromFile(fname)) {
            if (!tex.loadFromFile("data/default.png")) {
                exit(-1);
            }
        }

        return tex;
    };
};
