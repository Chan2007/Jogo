#ifndef ANIMADOR_FUNDO_H
#define ANIMADOR_FUNDO_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Animador_Fundo {
    private:
        struct FrameData {
            std::string spSheetPath;
            sf::IntRect textureRect;
        };
        sf::Sprite currentSprite;
        sf::Sprite nextSprite;
        sf::Texture currentTexture;
        sf::Texture nextTexture;
        std::vector<FrameData> frames;
        std::string currentspSheetPath;
        std::string nextspSheetPath;
        size_t currentFrameIndex;
        sf::Clock clock;
        float frameAccumulator;
        float frameTime;
        sf::Vector2u frameSize;
        sf::Vector2u targetSize;
        bool loaded;

        void updateSpriteScale();
        void updateBlend(float blend);

        static bool loadTextureFromPath(sf::Texture& texture, std::string& loadedPath, const std::string& path);
        bool applyFrame(sf::Sprite& sprite, sf::Texture& texture, std::string& loadedPath, const FrameData& frame);

        Animador_Fundo(const Animador_Fundo&);
        Animador_Fundo &operator=(const Animador_Fundo&);

    public:
        Animador_Fundo();
        ~Animador_Fundo();

        bool loadFrames(const std::string& pathPrefix, int numFrames = 376, int startIndex = 1, int frameStep = 2);
        void update();

        void draw(sf::RenderWindow &window) const;

        void setPosicao(const sf::Vector2f& pos);
        void setTargetSize(const sf::Vector2u& size);
};

#endif
