#ifndef ANIMADOR_FUNDO_H
#define ANIMADOR_FUNDO_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Animador_Fundo {
    private:
        sf::Sprite currentSprite;
        sf::Sprite nextSprite;
        std::vector<sf::Texture> frames;
        std::vector<std::string> framePaths;
        size_t currentFrameIndex;
        sf::Clock clock;
        float frameAccumulator;
        float frameTime;
        sf::Vector2u frameSize;
        sf::Vector2u targetSize;
        bool loaded;

        void updateSpriteScale();
        void updateBlend(float blend);

        Animador_Fundo(const Animador_Fundo&);
        Animador_Fundo &operator=(const Animador_Fundo&);

    public:
        Animador_Fundo();
        ~Animador_Fundo();

        bool loadFrames(const std::string& pathPrefix, int numFrames = 376, int startIndex = 1, int frameStep = 2);
        void update();
        void draw(sf::RenderWindow& window);
        void setPosition(const sf::Vector2f& pos);
        void setTargetSize(const sf::Vector2u& size);
};

#endif
