//
// Created by Henrique on 29/04/2026.
//

#include "ListaEntidades.h"
#include "Elemento.h"
#include "Lista.h"

ListaEntidades::ListaEntidades() {
    LEntidade = new Lista <Entidade>();
}

ListaEntidades::~ListaEntidades() {
    LEntidade->limpar();
    delete LEntidade;
    LEntidade = NULL;
}

bool ListaEntidades::incluirEntidade(Entidade* E) {
    return LEntidade->incluirInfo(E);
}

void ListaEntidades::limparLista() {
    LEntidade->limpar();
}

Elemento<Entidade>* ListaEntidades::localizarEntidade(Elemento <Entidade>* E) {
    return LEntidade->localizarElemento(E);
}

Entidade* ListaEntidades::localizarInfo(Entidade* E) {
    return LEntidade->localizarInfo(E);
}

Entidade* ListaEntidades::localizarNome(std::string nome) {
    return LEntidade->localizarNome(nome);
}
