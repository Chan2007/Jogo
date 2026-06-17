#include "Visitor_Colisao_Jogador.h"

#include <iostream>

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

VisitorColisaoJogador::VisitorColisaoJogador(Personagens::Jogador* J) : jogador(J) {}
VisitorColisaoJogador::~VisitorColisaoJogador() {}

void VisitorColisaoJogador::colidir(Personagens::Jogador* J)  {
    // Se o jogador colidido for um SEGUNDO jogador diferente do que disparou a colisão
    if (J && jogador && J != jogador) {
        jogador->setColisao(true);
        J->setColisao(true);
    }
}

void VisitorColisaoJogador::colidir(Personagens::Inimigo* I) {
    if (!I || !jogador) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        I->receberDano(jogador->getPoder());
        if (I->getVida() <= 0) {
            float pontoAnt = jogador->getPontos();
            jogador->registrarAbate();
            std::cout << "Inimigo abatido! Pontos ganhos: " << jogador->getPontos() - pontoAnt << std::endl;
            I->setVigente(false);
        }
    }
}

void VisitorColisaoJogador::colidir(Obstaculos::Obstaculo* O) {
    if (!O || !jogador) return;
    O->obstaculizar(jogador);
}

void VisitorColisaoJogador::colidir(Entidades::Projetil* p)  {
    if (!jogador || !p->getVigente()) return;

    // Verifica se o tiro foi do jogador para causar dano
    if (!p->getDoJogador()) {
        if (!jogador->getInvulneravel()) {
            jogador->receberDano(p->getDano());
            std::cout << "Jogador foi atingido por um projetil." << std::endl;
        }
        p->setAtivo(false);
    }
}