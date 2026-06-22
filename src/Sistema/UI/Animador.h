#ifndef ANIMADOR_H
#define ANIMADOR_H

#include <SFML/Graphics.hpp>
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Proxy_Textura.h"
#include <string>
#include <vector>

class Animador {
    private:
        struct FrameData {
            std::string caminho_spSheet;
            sf::IntRect RectTextura;
            sf::Texture* textura;
            FrameData();
        };

        sf::Sprite SpriteAtual;
        sf::Sprite ProxSprite;
        Gerenciadores::Proxy_Textura proxy;
        mutable bool allowLoad;

        std::vector<FrameData> frames_data;
        Gerenciadores::Gerenciador_Textura* gerenciadorTextura;

        unsigned int FrameIndexAtual;
        sf::Clock clock;
        float frameAccumulator;
        float frameTime;
        sf::Vector2u frameSize;
        sf::Vector2u targetSize;
        sf::Vector2f position;
        bool loaded;

        void updateSpriteScale();
        void updateBlend();
        sf::Texture* findTexture(const std::string& path) const;
        void applyFrame(sf::Sprite& sprite, FrameData& frameData);

    public:
        explicit Animador(Gerenciadores::Gerenciador_Textura* gerenciadorTextura = NULL);
        ~Animador();

        bool loadFrames(const std::string& pathPrefix,
                        const std::string& name,
                        int numFrames, int frameStep,
                        unsigned int colunas, unsigned int linhas);
        void update();
        void draw(sf::RenderTarget& target) const;
        void setSheetPosition(const sf::Vector2f& pos);
        void setSheetTargetSize(const sf::Vector2u& size);

        void atualizarSpriteEntidade(
            sf::Sprite& sprite, sf::IntRect& rectAtual,
            int numFrames, float dt,
            float& tempoAcumulado, int& indexFrameAtual,
            unsigned int cols = 9, unsigned int rows = 1, float tempoPorFrame = 0.12f
        );
};

#endif