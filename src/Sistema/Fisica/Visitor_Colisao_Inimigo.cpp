//
// Created by Henrique on 12/06/2026.
//

#include "Visitor_Colisao_Inimigo.h"

#include "Visitor_Colisao_Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Obstaculo/Obstaculo.h"
#include "Ente/Entidade/Projetil/Projetil.h"

VisitorColisaoInimigo::VisitorColisaoInimigo(Personagens::Inimigo* i) : inimigo(i) {}
VisitorColisaoInimigo::~VisitorColisaoInimigo() {}

void VisitorColisaoInimigo::visitar(Personagens::Inimigo* I) {
    if (I && inimigo && I != inimigo) {
        inimigo->setColisao(true);
        I->setColisao(true);
    }
}

void VisitorColisaoInimigo::visitar(Obstaculos::Obstaculo* O) {
    if (!O || !inimigo) return;
    inimigo->setColisao(true);

    sf::FloatRect hitboxInimigo = inimigo->getTamanho();
    sf::FloatRect hitboxObs = O->getTamanho();

    float centroYInimigo = hitboxInimigo.top + (hitboxInimigo.height / 2.f);
    float centroXInimigo = hitboxInimigo.left + (hitboxInimigo.width / 2.f);

    bool bateuNaParede = (centroYInimigo > hitboxObs.top) && (centroYInimigo < hitboxObs.top + hitboxObs.height);

    if (bateuNaParede)
        inimigo->inverterPatrulha();
    else {
        float margem = 5.0f;
        if (inimigo->getDirecaoPatrulha() > 0.0f) {
            if (centroXInimigo >= (hitboxObs.left + hitboxObs.width) - margem)
                inimigo->inverterPatrulha();
        }
        else if (inimigo->getDirecaoPatrulha() <= 0.0f) {
            if (centroXInimigo <= hitboxObs.left + margem)
                inimigo->inverterPatrulha();
        }
    }
}

void VisitorColisaoInimigo::visitar(Entidades::Projetil* P) {
    if (!inimigo || !P->getAtivo()) return;
    if (P->getDoJogador()) {
        inimigo->receberDano(P->getDano());
        P->setAtivo(false);
    }
}