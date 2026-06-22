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
            bool incluirEntidade(Entidades::Entidade *E) const;
            bool removerEntidade(Entidades::Entidade *E) const;
            void limparLista() const;
            void desenharTodas() const;
            void percorrer() const;

            Lista<Entidades::Entidade>::IteratorLista getPrimeiro() const { return LEntidade->getPrimeiro(); }
            Entidades::Entidade* localizarInfo(Entidades::Entidade* E) const;
            Entidades::Entidade* localizarNome(const std::string &nome) const;
            void salvarTodas(std::ostream& arq) const;
    };
}


#endif //JOGO_LISTAENTIDADES_H