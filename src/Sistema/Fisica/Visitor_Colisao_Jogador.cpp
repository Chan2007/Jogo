#include "Visitor_Colisao_Jogador.h"

#include <iostream>

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

VisitorColisaoJogador::VisitorColisaoJogador(Personagens::Jogador* J) : jogador(J) {}
VisitorColisaoJogador::~VisitorColisaoJogador() {}

void VisitorColisaoJogador::visitar(Personagens::Jogador* J)  {
    // Se o jogador colidido for um SEGUNDO jogador diferente do que disparou a colisão
    if (J && jogador && J != jogador) {
        jogador->setColisao(true);
        J->setColisao(true);
    }
}

void VisitorColisaoJogador::visitar(Personagens::Inimigo* I) {
    if (!I || !jogador) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        I->receberDano(jogador->getPoder());
}

void VisitorColisaoJogador::visitar(Obstaculos::Obstaculo* O) {
    if (!O || !jogador) return;

    sf::FloatRect hitboxJogador = jogador->getTamanho();
    sf::FloatRect hitboxObs = O->getTamanho();

    float peDoJogador = hitboxJogador.top + hitboxJogador.height;
    float topoPlataforma = hitboxObs.top;

    // Verifica se o jogador pousou em cima (margem de tolerância)
    if (peDoJogador <= topoPlataforma + 10.f) {
        Gerenciadores::Gerenciador_Gravidade& gerenciadorGravidade = jogador->getGerenciadorGravidade();
        gerenciadorGravidade.aoTocarChao(jogador, sf::Vector2f(0.f, -1.f));
    }
}

void VisitorColisaoJogador::visitar(Entidades::Projetil* p)  {
    if (!jogador || !p->getAtivo()) return;

    // Verifica se o tiro foi do jogador para causar dano
    if (!p->getDoJogador()) {
        if (!jogador->getInvulneravel()) {
            jogador->receberDano(p->getDano());
            std::cout << "Jogador foi atingido por um projétil." << std::endl;
        }
        p->setAtivo(false);
    }
}