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

    /*
    void Obstaculo::aceitar(VisitorColisao* visitor) {
        if (visitor) visitor->colidir(this);
    }
    */

    void Obstaculo::salvarObstaculo() {
        if (buffer) {
        }
    }

    Memento* Obstaculo::salvarMemento() const {
        return new ObstaculoMemento(*this);
    }

    void Obstaculo::restaurarMemento(const Memento *memento) {
        Entidade::restaurarMemento(memento);
        const ObstaculoMemento* pMemento = dynamic_cast<const ObstaculoMemento*>(memento);
        if (pMemento) {
            perigoso = pMemento->perigoso;
        }
    }

} // Obstaculo