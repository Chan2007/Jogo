#ifndef ANIMADOR_FUNDO_H
#define ANIMADOR_FUNDO_H

#include <SFML/Graphics.hpp>
#include "Jogo_Principal/Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include <string>
#include <vector>

class Animador_Fundo {
private:
    struct FrameData {
        std::string caminho_spSheet;
        sf::IntRect RectTextura;
        sf::Texture* textura;
        FrameData();
    };

    sf::Sprite SpriteAtual;
    sf::Sprite ProxSprite;

    std::vector<FrameData> frames_data;

    Gerenciador_Textura* gerenciadorTextura;

    unsigned int FrameIndexAtual;
    sf::Clock clock;
    float frameAccumulator;
    float frameTime;
    sf::Vector2u frameSize;
    sf::Vector2u targetSize;
    sf::Vector2f posicaoBase;
    bool loaded;

    void updateSpriteScale();
    void updateBlend();
    sf::Texture* buscarTextura(const std::string& path) const;
    bool applyFrame(sf::Sprite& sprite, FrameData& frameData);

    Animador_Fundo(const Animador_Fundo&);
    Animador_Fundo& operator=(const Animador_Fundo&);

public:
    explicit Animador_Fundo(Gerenciador_Textura* gerenciadorTextura);
    ~Animador_Fundo();

    bool loadFrames(const std::string& pathPrefix, int numFrames = 376, int startIndex = 1,
                    int frameStep = 2, int colunas = 4, int linhas = 3);
    void update();
    void draw(sf::RenderWindow& window) const;
    void setPosicao(const sf::Vector2f& pos);
    void setTargetSize(const sf::Vector2u& size);
};

#endif