#ifndef ANIMADOR_FUNDO_H
#define ANIMADOR_FUNDO_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class BackgroundAnimator {
private:
    sf::Sprite* currentSprite;
    sf::Sprite* nextSprite;
    std::vector<sf::Texture> frames;
    std::vector<std::string> framePaths;
    size_t currentFrame;
    sf::Clock clock;
    float frameAccumulator;
    float frameTime;
    sf::Vector2u frameSize;

    void updateSpriteScale();
    void updateBlend(float blend);

    BackgroundAnimator(const BackgroundAnimator&);
    BackgroundAnimator& operator=(const BackgroundAnimator&);

public:
    BackgroundAnimator();
    ~BackgroundAnimator();

    bool loadFrames(const std::string& pathPrefix, int numFrames, int startIndex = 0, int frameStep = 1);
    void update();
    void draw(sf::RenderWindow& window);
    void setPosition(const sf::Vector2f& pos);
};

#endif
