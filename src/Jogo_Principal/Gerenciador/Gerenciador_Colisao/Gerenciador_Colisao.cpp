//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Colisao.h"
#include "Jogo_Principal/Entidade/Entidade.h"
#include "Jogo_Principal/Entidade/Obstaculo/Obstaculo.h"
#include "Jogo_Principal/Entidade/Personagem/Jogador/Jogador.h"
#include "Jogo_Principal/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Jogo_Principal/Entidade/Projetil/Projetil.h"

namespace Gerenciadores {
    Gerenciador_Colisao::Gerenciador_Colisao() {}
    Gerenciador_Colisao& Gerenciador_Colisao::getInstancia() {
        static Gerenciador_Colisao instancia;
        return instancia;
    }
    Gerenciador_Colisao::~Gerenciador_Colisao() {limpar();}
    void Gerenciador_Colisao::limpar() {
        Linimigos.clear();
        Lobstaculos.clear();
        Lprojetil.clear();
        Ljogadores.clear();
    }
    void Gerenciador_Colisao::incluirEntidade(Entidades::Entidade* entidade) {
        if (!entidade) return;
        Personagens::Jogador* jogador = NULL;
        Obstaculos::Obstaculo* obstaculo = NULL;
        Personagens::Inimigo* inimigo = NULL;
        Entidades::Projetil* projetil = NULL;
        jogador = dynamic_cast<Personagens::Jogador*>(entidade);
        obstaculo = dynamic_cast<Obstaculos::Obstaculo*>(entidade);
        inimigo = dynamic_cast<Personagens::Inimigo*>(entidade);
        projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (obstaculo) incluirNaLista(Lobstaculos, obstaculo);
        else if (projetil) incluirNaLista(Lprojetil, projetil);
        else if (inimigo) incluirNaLista(Linimigos, inimigo);
        else if (jogador) incluirNaLista(Ljogadores, jogador);
    }

    void Gerenciador_Colisao::removerEntidade(Entidades::Entidade* entidade) {
        if (!entidade) return;
        Personagens::Jogador* jogador = NULL;
        Obstaculos::Obstaculo* obstaculo = NULL;
        Personagens::Inimigo* inimigo = NULL;
        Entidades::Projetil* projetil = NULL;
        jogador = dynamic_cast<Personagens::Jogador*>(entidade);
        obstaculo = dynamic_cast<Obstaculos::Obstaculo*>(entidade);
        inimigo = dynamic_cast<Personagens::Inimigo*>(entidade);
        projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (jogador) removerDaLista(Ljogadores, jogador);
        else if (obstaculo) removerDaLista(Lobstaculos, obstaculo);
        else if (inimigo) removerDaLista(Linimigos, inimigo);
        else if (projetil) removerDaLista(Lprojetil, projetil);
    }

    bool Gerenciador_Colisao::colidiu(const Entidades::Entidade* entidade, const Entidades::Entidade* movel) {
        if (!entidade || !movel) {cerr << "Ponteiro nulo!" << endl; return false;}
        const float x_m = movel->getPosicao().x;
        const float y_m = movel->getPosicao().y;
        const float x_e = entidade->getPosicao().x;
        const float y_e = entidade->getPosicao().y;

        const float hitbox_x_m = x_m + movel->getTamanho().x;
        const float hitbox_y_m = y_m + movel->getTamanho().y;
        const float hitbox_x_e = x_e + entidade->getTamanho().x;
        const float hitbox_y_e = y_e + entidade->getTamanho().y;

        return (x_m < hitbox_x_e && hitbox_x_m > x_e && y_m < hitbox_y_e && hitbox_y_m > y_e);
    }

    void Gerenciador_Colisao::calculaColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel) {
        if (!entidade || !movel) return;
        movel->setColisao(true);
        const sf::Vector2f posP = movel->getPosicao();
        const sf::Vector2f tamP = movel->getTamanho();
        const sf::Vector2f posE = entidade->getPosicao();
        const sf::Vector2f tamE = entidade->getTamanho();

        const float centroP_x = posP.x + (tamP.x / 2.0f);
        const float centroP_y = posP.y + (tamP.y / 2.0f);
        const float centroE_x = posE.x + (tamE.x / 2.0f);
        const float centroE_y = posE.y + (tamE.y / 2.0f);

        const float dx = centroP_x - centroE_x;
        const float dy = centroP_y - centroE_y;

        const float intersecX = (tamP.x / 2.0f + tamE.x / 2.0f) - static_cast<float>(fabs(dx));
        const float intersecY = (tamP.y / 2.0f + tamE.y / 2.0f) - static_cast<float>(fabs(dy));

        if (intersecX < intersecY) {
            if (dx > 0.0f)
                movel->setPosicao(sf::Vector2f(posP.x + intersecX, posP.y));
            else
                movel->setPosicao(sf::Vector2f(posP.x - intersecX, posP.y));
        }
        else {
            if (dy > 0.0f)
                movel->setPosicao(sf::Vector2f(posP.x, posP.y + intersecY));
            else
                movel->setPosicao(sf::Vector2f(posP.x, posP.y - intersecY));
        }
    }

    bool Gerenciador_Colisao::implementarColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel) {
        if (!entidade || !movel) return false;
        movel->setColisao(false);
        if (colidiu(entidade, movel)) {
            calculaColisao(entidade, movel);
            movel->setColisao(true);
            return true;
        }
        return false;
    }

    void Gerenciador_Colisao::verificaColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel) {
        implementarColisao(entidade, movel);
    }
    void Gerenciador_Colisao::verificaObstaculo(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Lobstaculos, entidade);
    }

    void Gerenciador_Colisao::verificaProjetil(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Lprojetil, entidade);
    }

    void Gerenciador_Colisao::verificaInimigo(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Linimigos, entidade);
    }
    void Gerenciador_Colisao::verificaJogador(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Ljogadores, entidade);
    }
    void Gerenciador_Colisao::executar(Entidades::Entidade* entidade) {
        verificaObstaculo(entidade);
        verificaProjetil(entidade);
        verificaInimigo(entidade);
    }
} // Gerenciador