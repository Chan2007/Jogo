//
// Created by Henrique on 09/05/2026.
//

#include "Primeira_Fase.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"

namespace Fases {
    void Primeira_Fase::criarCenario() {
        float dt = relogio.restart().asSeconds();

        if (dt > 0.1f) { dt = 0.1f; }

        animadorFase1.update();
        janela.clear(sf::Color::Black);
        animadorFase1.draw(janela);

        LEntidades.desenharTodas(janela);
    }
    void Primeira_Fase::criarObstaculos() {
        criarObstFaceis();
    }
    void Primeira_Fase::criarObstFaceis() {
        Obstaculos::Plataforma* chao = new Obstaculos::Plataforma(Obstaculos::Plataforma::CHAO);
        if (chao) {
            Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(chao);
            chao->getCorpo().setPosition(sf::Vector2f(desktop.width * 0.5, desktop.height));
            LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(chao));
        }
        else {std::cerr << "Falha ao criar plataforma" << std::endl;}

        Obstaculos::Plataforma* novaPlat;
        Ente::sementear();
        for (int i = 0; i < (rand()%8)+3; i++) {
            if ((rand()%10)<5)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL1);
            else if ((rand()%10)<4)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL2);
            else
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL3);

            if (!novaPlat) {std::cerr << "Falha ao criar nova plataforma." << std::endl;}

            bool posicaoValida = false;
            int tentativas = 0;

            while (!posicaoValida && tentativas < 500) {
                int sizex = ((janela.getSize().x) - novaPlat->getTamanho().width);
                int sizey = ((janela.getSize().y) - novaPlat->getTamanho().height - (chao->getAltura()) / 2);
                novaPlat->getCorpo().setPosition((rand() % sizex) + (novaPlat->getTamanho().width) / 2, (rand() % sizey) + (novaPlat->getTamanho().height) / 2);

                sf::FloatRect hitboxExpandida = novaPlat->getCorpo().getGlobalBounds();
                hitboxExpandida.left -= 20.f;
                hitboxExpandida.top -= 20.f;
                hitboxExpandida.width += 40.f;
                hitboxExpandida.height += 40.f;
                if (Gerenciadores::Gerenciador_Colisao::getInstancia().verificarPosicaoLivre(hitboxExpandida))
                    posicaoValida = true;

                tentativas++;
            }
            if (posicaoValida) {
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(novaPlat));
                Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(novaPlat);
            }
            else delete novaPlat;
        }
        novaPlat = NULL;
    }
} // Fases