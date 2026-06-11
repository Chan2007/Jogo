//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Colisao.h"

#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Gerenciadores {
    class Observer_Colisao;
}
namespace Gerenciadores {
    Gerenciador_Colisao& Gerenciador_Colisao::getGerenciador() {
        static Gerenciador_Colisao instancia;
        return instancia;
    }
    Gerenciador_Colisao::Gerenciador_Colisao(): Observer_Colisao() {}

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

    bool Gerenciador_Colisao::verificarPosicaoLivre(const sf::FloatRect& hitboxProvisoria) {

        for (auto it = Lobstaculos.begin(); it != Lobstaculos.end(); ++it) {
            Entidades::Entidade* obstaculo = *it;
            if (obstaculo) {
                if (hitboxProvisoria.intersects(obstaculo->getTamanho())) {
                    return false;
                }
            }
        }
        return true;
    }

    bool Gerenciador_Colisao::colidiu(const Entidades::Entidade* entidade, const Entidades::Entidade* movel) {
        if (!entidade || !movel) return false;

        return entidade->getTamanho().intersects(movel->getTamanho());
    }

    void Gerenciador_Colisao::calculaColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel) {
        if (!entidade || !movel) return;

        const sf::FloatRect tamE = entidade->getTamanho();
        const sf::FloatRect tamM = movel->getTamanho();

        float centroEx = tamE.left + tamE.width / 2.0f;
        float centroEy = tamE.top + tamE.height / 2.0f;

        float centroMx = tamM.left + tamM.width / 2.0f;
        float centroMy = tamM.top + tamM.height / 2.0f;

        const float dx = centroMx - centroEx;
        const float dy = centroMy - centroEy;

        const float intersecX = tamM.width / 2.0f + tamE.width / 2.0f - std::fabs(dx);
        const float intersecY = tamM.height / 2.0f + tamE.height / 2.0f - std::fabs(dy);

        if (intersecX < intersecY) {
            sf::Vector2f posAtual = movel->getPosicao();
            if (dx > 0.0f)
                movel->setPosicao(sf::Vector2f(posAtual.x + intersecX, posAtual.y));
            else
                movel->setPosicao(sf::Vector2f(posAtual.x - intersecX, posAtual.y));

        }
        else {
            sf::Vector2f posAtual = movel->getPosicao();
            if (dy > 0.0f)
                movel->setPosicao(sf::Vector2f(posAtual.x, posAtual.y + intersecY));
            else {
                movel->setPosicao(sf::Vector2f(posAtual.x, posAtual.y - intersecY));
                Gerenciadores::Gerenciador_Gravidade::getGerenciador().aoTocarChao(movel, sf::Vector2f(0.f, -1.f));
            }
            Gerenciadores::Gerenciador_Gravidade::getGerenciador().setVy(movel, 0.0f);
        }
    }

    bool Gerenciador_Colisao::verificarLimitesJanela(Entidades::Entidade* entidade, const sf::Vector2u& tamanhoJanela, Gerenciador_Gravidade* pGravidade) {
        if (!entidade) return false;

        const sf::Vector2f posicaoAtual = entidade->getPosicao();
        const sf::FloatRect tamanho = entidade->getTamanho();

        float limiteLargura = static_cast<float>(tamanhoJanela.x);
        float limiteAltura = static_cast<float>(tamanhoJanela.y);

        sf::Vector2f novaPosicao = posicaoAtual;
        bool colidiuBorda = false;

        float metadeLargura = tamanho.width / 2.0f;
        float metadeAltura = tamanho.height / 2.0f;

        if (novaPosicao.x - metadeLargura < 0.0f) {
            novaPosicao.x = metadeLargura;
            colidiuBorda = true;
        }

        else if (novaPosicao.x + metadeLargura > limiteLargura) {
            novaPosicao.x = limiteLargura - metadeLargura;
            colidiuBorda = true;
        }

        if (novaPosicao.y - metadeAltura < 0.0f) {
            novaPosicao.y = metadeAltura;
            colidiuBorda = true;
        }

        else if (novaPosicao.y + metadeAltura > limiteAltura) {
            novaPosicao.y = limiteAltura - metadeAltura;
            colidiuBorda = true;

            if (pGravidade) pGravidade->aoTocarChao(entidade, sf::Vector2f(0.0f, -1.0f));
        }

        if (colidiuBorda) {
            entidade->setPosicao(novaPosicao);
            entidade->setColisao(true);
        }
        return colidiuBorda;
    }

    void Gerenciador_Colisao::verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel) {
        if (!entidade || !movel || entidade == movel) return;

        if (colidiu(entidade, movel)) {
        calculaColisao(entidade, movel);
        movel->setColisao(true);

        movel->aoColidir(entidade);
        entidade->aoColidir(movel);
        }
    }
    void Gerenciador_Colisao::verificarObstaculo(Entidades::Entidade* entidade) const {
        colisao_Entidade_Classe(Lobstaculos, entidade);
    }

    void Gerenciador_Colisao::verificarProjetil(Entidades::Entidade* entidade) const {
        colisao_Entidade_Classe(Lprojetil, entidade);
    }

    void Gerenciador_Colisao::verificarInimigo(Entidades::Entidade* entidade) const {
        colisao_Entidade_Classe(Linimigos, entidade);
    }
    void Gerenciador_Colisao::verificarJogador(Entidades::Entidade* entidade) const {
        colisao_Entidade_Classe(Ljogadores, entidade);
    }
    void Gerenciador_Colisao::executar(const sf::Vector2u& tamanhoJanela, Gerenciador_Gravidade* pGravidade) {
        std::vector<Personagens::Jogador*>::iterator itJog;
        std::vector<Obstaculos::Obstaculo*>::iterator itObs;
        std::list<Personagens::Inimigo*>::iterator itInim;
        std::set<Entidades::Projetil*>::iterator itProj;

        // Colisão entre jogadores e obstáculos
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (jogador == NULL) continue;

            for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
                Obstaculos::Obstaculo* obstaculo = *itObs;
                if (obstaculo != NULL)
                    verificarColisao(obstaculo, jogador);

                // Sair do loop deste jogador, mas NÃO parar o gerenciador
                if (!jogador->estaVivo()) break;
            }
        }

        // Colisão entre jogadores e inimigos
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (jogador == NULL) continue;

            for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
                Personagens::Inimigo* inimigo = *itInim;
                if (inimigo == NULL) continue;

                verificarColisao(inimigo, jogador);

                if (!jogador->estaVivo() || Linimigos.empty())
                    break;
            }
        }

        // olisão entre jogadores e projéteis
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (jogador == NULL) continue;

            for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
                Entidades::Projetil* projetil = *itProj;
                if (projetil == NULL) continue;

                verificarColisao(projetil, jogador);

                if (!jogador->estaVivo()) break;
            }
        }

        // Colisão entre inimigos e obstáculos
        for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
            Personagens::Inimigo* inimigo = *itInim;
            if (inimigo == NULL) continue;

            for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
                Obstaculos::Obstaculo* obstaculo = *itObs;
                if (obstaculo != NULL)
                    verificarColisao(obstaculo, inimigo);
            }
        }

        // Colisão entre inimigos e projéteis
        for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
            Personagens::Inimigo* inimigo = *itInim;
            if (inimigo == NULL) continue;

            for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
                Entidades::Projetil* projetil = *itProj;
                if (projetil != NULL)
                    verificarColisao(projetil, inimigo);
            }
        }

        // Colisão entre obstáculos e projéteis
        for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
            Obstaculos::Obstaculo* obstaculo = *itObs;
            if (obstaculo == NULL) continue;

            for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
                Entidades::Projetil* projetil = *itProj;
                if (projetil != NULL)
                    verificarColisao(obstaculo, projetil);
            }
        }

        // Checagem de colisão nas bordas da janela
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            if (*itJog != NULL) verificarLimitesJanela(*itJog, tamanhoJanela, pGravidade);
        }

        for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
            if (*itInim != NULL) verificarLimitesJanela(*itInim, tamanhoJanela, pGravidade);
        }

        for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
            if (*itProj != NULL) verificarLimitesJanela(*itProj, tamanhoJanela, pGravidade);
        }
    }
} // Gerenciador
