//
// Created by Henrique on 06/05/2026.
//

#include "Gerenciador_Colisao.h"

#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Sistema/Fisica/Visitor_Colisao_Inimigo.h"
#include "Sistema/Fisica/Visitor_Colisao_Jogador.h"
#include <iostream>

namespace Gerenciadores {
    class Observer_Colisao;
}
namespace Gerenciadores {
    Gerenciador_Colisao& Gerenciador_Colisao::getGerenciador() {
        static Gerenciador_Colisao instancia;
        return instancia;
    }
    Gerenciador_Colisao::Gerenciador_Colisao() {}

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

        for (std::vector<Obstaculos::Obstaculo*>::const_iterator it = Lobstaculos.begin(); it != Lobstaculos.end(); ++it) {
            Entidades::Entidade* obstaculo = *it;
            if (obstaculo) {
                if (hitboxProvisoria.intersects(obstaculo->getTamanho())) {
                    return false;
                }
            }
        }
        return true;
    }

    bool Gerenciador_Colisao::verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel) {

        if (!entidade || !movel || !entidade->getVigente() || !movel->getVigente()) return false;
        if (entidade == movel) return false;
        if (!entidade->getTamanho().intersects(movel->getTamanho())) return false;

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
                Gerenciador_Gravidade::getGerenciador().aoTocarChao(movel, sf::Vector2f(0.f, -1.f));
            }
            Gerenciador_Gravidade::getGerenciador().setVy(movel, 0.0f);
        }
        return true;
    }

    void Gerenciador_Colisao::tratarColisoesJogsObstacs() const {
        std::vector<Personagens::Jogador *>::const_iterator itJog;
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (!jogador || !jogador->getVigente()) continue;

            // Jogador x Obstáculos
            std::vector<Obstaculos::Obstaculo *>::const_iterator itObs;
            for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
                Obstaculos::Obstaculo* obstaculo = *itObs;
                if (obstaculo && verificarColisao(obstaculo, jogador)) {
                    /*VisitorColisaoJogador visitor(jogador);
                    obstaculo->aceitar(&visitor);*/
                    obstaculo->obstaculizar(jogador);
                }
            }
        }
    }

    void Gerenciador_Colisao::tratarColisoesJogsProjeteis() const {
        std::vector<Personagens::Jogador *>::const_iterator itJog;
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (!jogador || !jogador->getVigente()) continue;

            // Jogador x Projétil
            std::set<Entidades::Projetil *>::const_iterator itProj;
            for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
                Entidades::Projetil* projetil = *itProj;
                if (projetil && verificarColisao(jogador, projetil) && projetil->getVigente()) {
                    /*VisitorColisaoJogador visitor(jogador);
                    projetil->aceitar(&visitor);*/
                    if (!projetil->getDoJogador()) {
                        if (!jogador->getInvulneravel()) {
                            jogador->receberDano(projetil->getDano());
                            std::cout << "Jogador foi atingido por um projetil." << std::endl;
                        }
                        projetil->setAtivo(false);
                    }
                }
            }
        }
    }

    void Gerenciador_Colisao::tratarColisoesJogsInimgs() const {
        std::vector<Personagens::Jogador *>::const_iterator itJog;
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (!jogador || !jogador->getVigente()) continue;

            // Jogador x Inimigos
            std::list<Personagens::Inimigo *>::const_iterator itInim;
            for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
                Personagens::Inimigo* inimigo = *itInim;
                if (inimigo && verificarColisao(inimigo, jogador) && inimigo->getVigente()) {
                    /*VisitorColisaoJogador visitor(jogador);
                    inimigo->aceitar(&visitor);*/
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                        inimigo->receberDano(jogador->getPoder());
                        if (inimigo->getVida() <= 0) {
                            float pontoAnt = jogador->getPontos();
                            jogador->registrarAbate();
                            std::cout << "Inimigo abatido! Pontos ganhos: " << jogador->getPontos() - pontoAnt << " Pontuacao atual: " << jogador->getPontos() << std::endl;
                            inimigo->setVigente(false);
                        }
                    }
                }
            }
        }
    }
    void Gerenciador_Colisao::tratarColisoesJogs() const {
        std::vector<Personagens::Jogador *>::const_iterator itJog;
        for (itJog = Ljogadores.begin(); itJog != Ljogadores.end(); ++itJog) {
            Personagens::Jogador* jogador = *itJog;
            if (!jogador || !jogador->getVigente()) continue;

            // Jogador x Jogador
            std::vector<Personagens::Jogador *>::const_iterator itJog2;
            for (itJog2 = Ljogadores.begin(); itJog2 != Ljogadores.end(); ++itJog2) {
                Personagens::Jogador* jogador2 = *itJog2;
                if (jogador2 && verificarColisao(jogador, jogador2) && jogador2->getVigente()) {
                    /*VisitorColisaoJogador visitor(jogador);
                    jogador2->aceitar(&visitor);*/
                    if (jogador2 != jogador) {
                        jogador->setColisao(true);
                        jogador2->setColisao(true);
                    }
                }
            }
        }
    }
    void Gerenciador_Colisao::tratarColisoesInim() const {
        std::vector<Obstaculos::Obstaculo *>::const_iterator itObs;
        std::set<Entidades::Projetil *>::const_iterator itProj;
        std::list<Personagens::Inimigo *>::const_iterator itInim;
        for (itInim = Linimigos.begin(); itInim != Linimigos.end(); ++itInim) {
            Personagens::Inimigo* inimigo1 = *itInim;
            if (!inimigo1 || !inimigo1->getVigente()) continue;
            
            // Inimigo x Obstáculos
            for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
                Obstaculos::Obstaculo* obstaculo = *itObs;
                Obstaculos::Plataforma* p = dynamic_cast<Obstaculos::Plataforma*>(obstaculo);
                if (p) {
                    if (obstaculo && verificarColisao(obstaculo, inimigo1)) {
                        /*VisitorColisaoInimigo visitor(inimigo1);
                        obstaculo->aceitar(&visitor);*/
                        inimigo1->setColisao(true);

                        sf::FloatRect hitboxInimigo = inimigo1->getTamanho();
                        sf::FloatRect hitboxObs = obstaculo->getTamanho();

                        float centroYInimigo = hitboxInimigo.top + (hitboxInimigo.height / 2.f);
                        float centroXInimigo = hitboxInimigo.left + (hitboxInimigo.width / 2.f);

                        bool bateuNaParede = (centroYInimigo > hitboxObs.top) && (centroYInimigo < hitboxObs.top + hitboxObs.height);

                        if (bateuNaParede)
                            inimigo1->inverterPatrulha();
                        else {
                            Obstaculos::Plataforma* p = dynamic_cast<Obstaculos::Plataforma*>(obstaculo);
                            if (p) {
                                float margem = 5.0f;
                                if (inimigo1->getDirecaoPatrulha() > 0.0f) {
                                    if (centroXInimigo >= (hitboxObs.left + hitboxObs.width) - margem)
                                        inimigo1->inverterPatrulha();
                                }
                                else if (inimigo1->getDirecaoPatrulha() <= 0.0f) {
                                    if (centroXInimigo <= hitboxObs.left + margem)
                                        inimigo1->inverterPatrulha();
                                }
                            }
                        }
                    }
                }
            }
            // Inimigo x Projétil
            for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
                Entidades::Projetil* projetil= *itProj;
                if (projetil && verificarColisao(projetil, inimigo1)) {
                    /*VisitorColisaoInimigo visitor(inimigo1);
                    projetil->aceitar(&visitor);*/
                }
            }

            // Inimigo x Inimigo
            std::list<Personagens::Inimigo*>::const_iterator itInim2;
            for (itInim2 = Linimigos.begin(); itInim2 != Linimigos.end(); ++itInim2) {
                Personagens::Inimigo* inimigo2 = *itInim2;
                if (inimigo2 && inimigo1 != inimigo2 && verificarColisao(inimigo1, inimigo2)) {
                    /*VisitorColisaoInimigo visitor(inimigo1);
                    inimigo2->aceitar(&visitor);*/
                    inimigo1->setColisao(true);
                    inimigo2->setColisao(true);
                }
            }

        }
    }
    void Gerenciador_Colisao::tratarColisoesProj() const{
        std::vector<Obstaculos::Obstaculo *>::const_iterator itObs;
        std::set<Entidades::Projetil *>::const_iterator itProj;
        
        for (itProj = Lprojetil.begin(); itProj != Lprojetil.end(); ++itProj) {
            Entidades::Projetil* projetil1 = *itProj;
            if (!projetil1 || !projetil1->getVigente()) continue;

            // Projétil x Obstáculos
            for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
                Obstaculos::Obstaculo* obstaculo = *itObs;
                if (obstaculo && verificarColisao(obstaculo, projetil1)) {
                    projetil1->setVigente(false);
                }
            }
            // Projétil x Projétil
            std::set<Entidades::Projetil*>::const_iterator itProj2;
            for (itProj2 = Lprojetil.begin(); itProj2 != Lprojetil.end(); ++itProj2) {
                Entidades::Projetil* projetil2 = *itProj2;
                if (projetil2 && projetil1 != projetil2 && verificarColisao(projetil1, projetil2)) {
                    projetil1->setVigente(false);
                    projetil2->setVigente(false);
                }
            }
        }
    }
    void Gerenciador_Colisao::tratarColisoesObst() const {
        std::vector<Obstaculos::Obstaculo*>::const_iterator itObs;
        for (itObs = Lobstaculos.begin(); itObs != Lobstaculos.end(); ++itObs) {
            Obstaculos::Obstaculo* obstaculo1 = *itObs;
            if (!obstaculo1) continue;

            std::vector<Obstaculos::Obstaculo*>::const_iterator itObs2;
            for (itObs2 = Lobstaculos.begin(); itObs2 != Lobstaculos.end(); ++itObs2) {
                Obstaculos::Obstaculo* obstaculo2 = *itObs2;
                if (obstaculo2)
                    verificarColisao(obstaculo1, obstaculo2);
            }
        }

    }
    void Gerenciador_Colisao::executar() {

        // Colisão entre jogadores
        tratarColisoesJogs();

        // Colisão entre jogadores e obstáculos
        tratarColisoesJogsObstacs();

        // Colisão entre jogadores e inimigos
        tratarColisoesJogsInimgs();

        // Colisão entre jogadores e projéteis
        tratarColisoesJogsProjeteis();

        // Colisão entre inimigos e outros
        tratarColisoesInim();

        // Colisão entre projéteis e outros
        tratarColisoesProj();

        // Colisão entre obstáculos
        tratarColisoesObst();
    }
} // Gerenciador
