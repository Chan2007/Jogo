//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Colisao.h"
#include "Jogo_Principal/Entidade/Entidade.h"


namespace Gerenciadores {
    Gerenciador_Colisao::Gerenciador_Colisao() {}

    Gerenciador_Colisao& Gerenciador_Colisao::getInstancia() {
        static Gerenciador_Colisao instancia;
        return instancia;
    }

    Gerenciador_Colisao::~Gerenciador_Colisao() {
        limpar();
    }

    void Gerenciador_Colisao::limpar() {
        Linimigos.clear();
        Lobstaculos.clear();
        Lprojetil.clear();
        Ljogadores.clear();
    }


    void Gerenciador_Colisao::incluirEntidade(Entidade* entidade) {
        if (!entidade) return;
        Entidades::Jogador* jogador = NULL;
        Entidades::Obstaculo* obstaculo = NULL;
        Entidades::Inimigo* inimigo = NULL;
        Entidades::Projetil* projetil = NULL;
        jogador = dynamic_cast<Entidades::Jogador*>(entidade);
        inimigo = dynamic_cast<Entidades::Inimigo*>(entidade);
        obstaculo = dynamic_cast<Entidades::Obstaculo*>(entidade);
        projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (obstaculo) incluirNaLista(Lobstaculos, obstaculo);
        else if (projetil) incluirNaLista(Lprojetil, projetil);
        else if (inimigo) incluirNaLista(Linimigos, inimigo);
        else if (jogador) incluirNaLista(Ljogadores, jogador);
    }

    void Gerenciador_Colisao::removerEntidade(Entidade* entidade) {
        if (!entidade) return;
        Entidades::Jogador* jogador = NULL;
        Entidades::Obstaculo* obstaculo = NULL;
        Entidades::Inimigo* inimigo = NULL;
        Entidades::Projetil* projetil = NULL;
        jogador = dynamic_cast<Entidades::Jogador*>(entidade);
        obstaculo = dynamic_cast<Entidades::Obstaculo*>(entidade);
        inimigo = dynamic_cast<Entidades::Inimigo*>(entidade);
        projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (jogador) removerDaLista(Ljogadores, jogador);
        else if (obstaculo) removerDaLista(Lobstaculos, obstaculo);
        else if (inimigo) removerDaLista(Linimigos, inimigo);
        else if (projetil) removerDaLista(Lprojetil, projetil);
    }

    bool Gerenciador_Colisao::colidiu(Entidade* entidade, Entidade* jogador) {
        const float x_j = jogador->getPosicao().x;
        const float y_j = jogador->getPosicao().y;
        const float x_e = entidade->getPosicao().x;
        const float y_e = entidade->getPosicao().y;

        const float hbox_x_j = x_j + jogador->getTamanho().x;
        const float hbox_y_j = y_j + jogador->getTamanho().y;
        const float hbox_x_e = x_e + entidade->getTamanho().x;
        const float hbox_y_e = y_e + entidade->getTamanho().y;

        return (x_j < hbox_x_e && hbox_x_j > x_e && y_j < hbox_y_e && hbox_y_j > y_e);
    }

    void Gerenciador_Colisao::calculaColisao(Entidade* entidade, Entidade* jogador) {
        const sf::Vector2f posJ = jogador->getPosicao();
        const sf::Vector2f tamJ = jogador->getTamanho();
        const sf::Vector2f posE = entidade->getPosicao();
        const sf::Vector2f tamE = entidade->getTamanho();

        const float centroJ_x = posJ.x + (tamJ.x / 2.0f);
        const float centroJ_y = posJ.y + (tamJ.y / 2.0f);
        const float centroE_x = posE.x + (tamE.x / 2.0f);
        const float centroE_y = posE.y + (tamE.y / 2.0f);

        const float dx = centroJ_x - centroE_x;
        const float dy = centroJ_y - centroE_y;

        const float intersecX = (tamJ.x / 2.0f + tamE.x / 2.0f) - static_cast<float>(fabs(dx));
        const float intersecY = (tamJ.y / 2.0f + tamE.y / 2.0f) - static_cast<float>(fabs(dy));

        if (intersecX < intersecY) {
            if (dx > 0.0f)
                jogador->setPosicao(sf::Vector2f(posJ.x + intersecX, posJ.y));
            else
                jogador->setPosicao(sf::Vector2f(posJ.x - intersecX, posJ.y));
        }
        else {
            if (dy > 0.0f)
                jogador->setPosicao(sf::Vector2f(posJ.x, posJ.y + intersecY));
            else
                jogador->setPosicao(sf::Vector2f(posJ.x, posJ.y - intersecY));
        }

        jogador->setColisao(true);
    }

    bool Gerenciador_Colisao::testarColisao(Entidade* entidade, Jogador* jogador) {
        if (!entidade || !jogador)
            return false;

        if (colidiu(entidade, jogador)) {
            calculaColisao(entidade, jogador);
            return true;
        }

        jogador->setColisao(false);
        return false;
    }

    void Gerenciador_Colisao::verificaColisao(Entidade* entidade, Entidade* jogador) {
        testarColisao(entidade, jogador);
    }

    void Gerenciador_Colisao::mediarComJogadores(Entidade* entidade) {
        for (size_t i = 0; i < Ljogadores.size(); i++)
            testarColisao(entidade, Ljogadores[i]);
    }

    void Gerenciador_Colisao::colisao_Obstaculo_Jogador() {
        for (size_t i = 0; i < Lobstaculos.size(); i++)
            mediarComJogadores(Lobstaculos[i]);
    }

    void Gerenciador_Colisao::colisao_Projetil_Jogador() {
        for (size_t i = 0; i < Lprojetil.size(); i++)
            mediarComJogadores(Lprojetil[i]);
    }

    void Gerenciador_Colisao::colisao_Inimigo_Jogador() {
        for (size_t i = 0; i < Linimigos.size(); i++)
            mediarComJogadores(Linimigos[i]);
    }

    void Gerenciador_Colisao::verificaObstaculo() {
        colisao_Obstaculo_Jogador();
    }

    void Gerenciador_Colisao::verificaProjetil() {
        colisao_Projetil_Jogador();
    }

    void Gerenciador_Colisao::verificaInimigo() {
        colisao_Inimigo_Jogador();
    }

    void Gerenciador_Colisao::executar() {
        verificaObstaculo();
        verificaProjetil();
        verificaInimigo();
    }
} // Gerenciador