//
// Created by Henrique on 06/05/2026.
//

#include "Obstaculo.h"
#include "Ente/Entidade/Entidade.h"
namespace Obstaculos {
    Obstaculo::Obstaculo() :
        Entidade("Obstaculo", 0),
        perigoso(false)
    {
    }

    Obstaculo::~Obstaculo() {
    }

    void Obstaculo::salvarDataBuffer() {
    }

    void Obstaculo::interagir_Colisao(Personagens::Inimigo* I) {
    }

    void Obstaculo::interagir_Colisao(Obstaculo* O) {
    }

    void Obstaculo::interagir_Colisao(Entidades::Projetil* P) {
    }

    void Obstaculo::interagir_Colisao(Personagens::Jogador* J) {
        setPlataforma(J);
    }
} // Obstaculo