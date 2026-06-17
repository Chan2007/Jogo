//
// Created by Henrique on 06/05/2026.
//

#include "Obstaculo.h"
#include "Ente/Entidade/Entidade.h"
#include "Sistema/Fisica/Visitor_Colisao.h"

namespace Obstaculos {
    Obstaculo::Obstaculo() :
        Entidade("Obstaculo"),
        perigoso(false)
    {
    }

    Obstaculo::~Obstaculo() {
    }

    void Obstaculo::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
} // Obstaculo