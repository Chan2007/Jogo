//
// Created by Henrique on 29/04/2026.
//

#ifndef JOGO_LISTAENTIDADES_H
#define JOGO_LISTAENTIDADES_H
#include <SFML/Graphics.hpp>
#include "Lista.h"

namespace Entidades {
    class Entidade;
}
namespace Listas {
    class ListaEntidades {
    private:
        Lista <Entidades::Entidade>* LEntidade;
    public:
        ListaEntidades();
        ~ListaEntidades();
        bool incluirEntidade(Entidades::Entidade *E);
        void limparLista();
        void desenharTodas(sf::RenderWindow& janela);
        Elemento <Entidades::Entidade>* localizarEntidade(Elemento <Entidades::Entidade>* E);
        Entidades::Entidade* localizarInfo(Entidades::Entidade* E);
        Entidades::Entidade* localizarNome(const std::string &nome);
    };
}


#endif //JOGO_LISTAENTIDADES_H