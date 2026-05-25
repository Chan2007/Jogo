//
// Created by Henrique on 29/04/2026.
//

#include "ListaEntidades.h"
#include "Elemento.h"
#include "Lista.h"
#include "Entidade/Entidade.h"
namespace Listas {
    ListaEntidades::ListaEntidades() {
        LEntidade = new Lista <Entidades::Entidade>();
    }

    ListaEntidades::~ListaEntidades() {
        LEntidade->limpar();
        delete LEntidade;
        LEntidade = NULL;
    }

    bool ListaEntidades::incluirEntidade(Entidades::Entidade *E)  {
        return LEntidade->incluirInfo(E);
    }

    void ListaEntidades::limparLista() {
        LEntidade->limpar();
    }

    Elemento<Entidades::Entidade>* ListaEntidades::localizarEntidade(Elemento <Entidades::Entidade>* E) {
        return LEntidade->localizarElemento(E);
    }

    Entidades::Entidade* ListaEntidades::localizarInfo(Entidades::Entidade* E) {
        return LEntidade->localizarInfo(E);
    }

    Entidades::Entidade* ListaEntidades::localizarNome(const std::string& nome) {
        return LEntidade->localizarNome(nome);
    }
}
