//
// Created by Henrique on 12/06/2026.
//

#include "Visitor_Colisao_Inimigo.h"

#include "Visitor_Colisao_Jogador.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Ente/Entidade/Projetil/Projetil.h"

VisitorColisaoInimigo::VisitorColisaoInimigo(Personagens::Inimigo* i) : inimigo(i) {}
VisitorColisaoInimigo::~VisitorColisaoInimigo() {}

void VisitorColisaoInimigo::colidir(Personagens::Inimigo* I) {
    if (I && inimigo && I != inimigo) {
        inimigo->setColisao(true);
        inimigo->inverterPatrulha();
        I->inverterPatrulha();
    }
}

void VisitorColisaoInimigo::colidir(Obstaculos::Obstaculo* O) {
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
        Obstaculos::Plataforma* p = dynamic_cast<Obstaculos::Plataforma*>(O);
        if (p) {
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
}

void VisitorColisaoInimigo::colidir(Entidades::Projetil* P) {
    if (!inimigo || !P->getVigente()) return;
    if (P->getDoJogador()) {
        inimigo->receberDano(P->getDano());
        P->setVigente(false);
    }
}