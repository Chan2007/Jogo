//
// Created by Henrique on 05/05/2026.
//

#include "Entidade.h"

namespace Entidades {
    Listas::ListaEntidades* Entidade::listaEntidades = 0;
    Gerenciadores::Mediador_Colisao* Entidade::mediador_colisao = 0;

    Entidade::Entidade(std::string nam, Gerenciadores::Mediador_Colisao* mediador):
        corpo(),
        textura(),
        colisao(false),
        nome(nam),
        ativo(true),
        tipo(ENTIDADE_GENERICA),
        nivel(1),
        prioridadeRenderizacao(0),
        escalaColisao(1.0f) {
        if (!nam.empty())
            nome = nam;
        mediador_colisao = mediador;
    }

    Entidade::~Entidade() {}

    void Entidade::salvarDataBuffer() {}
}
