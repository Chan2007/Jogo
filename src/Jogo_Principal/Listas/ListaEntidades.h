//
// Created by Henrique on 29/04/2026.
//

#ifndef JOGO_LISTAENTIDADES_H
#define JOGO_LISTAENTIDADES_H
#include "Lista.h"

class Entidade;
class ListaEntidades {
    private:
        Lista <Entidade>* LEntidade;
    public:
        ListaEntidades();
        ~ListaEntidades();
        bool incluirEntidade(Entidade* E);
        void limparLista();
        Elemento <Entidade>* localizarEntidade(Elemento <Entidade>* E);
        Entidade* localizarInfo(Entidade* E);
        Entidade* localizarNome(std::string nome);
};


#endif //JOGO_LISTAENTIDADES_H