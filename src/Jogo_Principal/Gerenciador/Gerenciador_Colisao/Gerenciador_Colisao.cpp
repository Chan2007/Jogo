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
    class Mediador_Colisao;
}
namespace Gerenciadores {
    Gerenciador_Colisao* Gerenciador_Colisao::gerenciador_colisao = NULL;

    Gerenciador_Colisao::Gerenciador_Colisao(): Mediador_Colisao() {}
    Gerenciador_Colisao& Gerenciador_Colisao::getInstancia() {
        if (!gerenciador_colisao) gerenciador_colisao = new Gerenciador_Colisao();
        return *gerenciador_colisao;
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
        if (!entidade || !movel) return false;
        const sf::Vector2f posE = entidade->getPosicao();
        const sf::FloatRect tamE = entidade->getTamanho();
        const sf::Vector2f posM = movel->getPosicao();
        const sf::FloatRect tamM = movel->getTamanho();

        return (posE.x < posM.x + tamM.width && posE.x + tamE.width > posM.x && posE.y < posM.y + tamM.height && posE.y + tamE.height > posM.y);
    }
    void Gerenciador_Colisao::calculaColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel) {
        if (!entidade || !movel) return;
        const sf::Vector2f posM = movel->getPosicao();
        const sf::FloatRect tamM = movel->getTamanho();
        const sf::Vector2f posE = entidade->getPosicao();
        const sf::FloatRect tamE = entidade->getTamanho();

        const float centroP_x = posM.x + (tamM.width / 2.0f);
        const float centroP_y = posM.y + (tamM.height / 2.0f);
        const float centroE_x = posE.x + (tamE.width / 2.0f);
        const float centroE_y = posE.y + (tamE.height / 2.0f);

        const float dx = centroP_x - centroE_x;
        const float dy = centroP_y - centroE_y;

        const float intersecX = (tamM.width / 2.0f + tamE.width / 2.0f) - static_cast<float>(fabs(dx));
        const float intersecY = (tamM.height / 2.0f + tamE.height / 2.0f) - static_cast<float>(fabs(dy));

        if (intersecX < intersecY) {
            if (dx > 0.0f)
                movel->setPosicao(sf::Vector2f(posM.x + intersecX, posM.y));
            else
                movel->setPosicao(sf::Vector2f(posM.x - intersecX, posM.y));
        }
        else {
            if (dy > 0.0f)
                movel->setPosicao(sf::Vector2f(posM.x, posM.y + intersecY));
            else
                movel->setPosicao(sf::Vector2f(posM.x, posM.y - intersecY));
        }
    }

    bool Gerenciador_Colisao::verificarLimitesJanela(Entidades::Entidade* entidade) {
        if (!entidade) return false;
        const sf::Vector2f posicaoAtual = entidade->getPosicao();
        const sf::FloatRect tamanho = entidade->getTamanho();
        const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        sf::Vector2f novaPosicao = posicaoAtual;
        bool colidiuBorda = false;
        if (novaPosicao.x < 0.0f) {
            novaPosicao.x = 0.0f;
            colidiuBorda = true;
        }
        else if (novaPosicao.x + tamanho.width > desktopMode.width) {
            novaPosicao.x = desktopMode.width - tamanho.width;
            colidiuBorda = true;
        }
        if (novaPosicao.y - tamanho.height < 0.0f) {
            novaPosicao.y = tamanho.height;
            colidiuBorda = true;
        }
        else if (novaPosicao.y  > desktopMode.height) {
            novaPosicao.y = desktopMode.height;
            colidiuBorda = true;
        }
        if (colidiuBorda) {
            entidade->setPosicao(novaPosicao);
            entidade->setColisao(true)










            ;
        }
        return colidiuBorda;
    }

    void Gerenciador_Colisao::verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel) {
        if (!entidade || !movel) return;
        movel->setColisao(false);
        if (colidiu(entidade, movel)) {
            calculaColisao(entidade, movel);
            movel->setColisao(true);
            movel->aoColidir(entidade);
            entidade->aoColidir(movel);
        }
    }
    void Gerenciador_Colisao::verificarObstaculo(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Lobstaculos, entidade);
    }

    void Gerenciador_Colisao::verificarProjetil(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Lprojetil, entidade);
    }

    void Gerenciador_Colisao::verificarInimigo(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Linimigos, entidade);
    }
    void Gerenciador_Colisao::verificarJogador(Entidades::Entidade* entidade) {
        colisao_Entidade_Classe(Ljogadores, entidade);
    }
    void Gerenciador_Colisao::executar(Entidades::Entidade* entidade) {
        verificarObstaculo(entidade);
        verificarProjetil(entidade);
        verificarInimigo(entidade);
        verificarJogador(entidade);
        verificarLimitesJanela(entidade);
    }
} // Gerenciador
