//
// Created by Henrique on 29/04/2026.
//

#include "ListaEntidades.h"
#include "Lista.h"
#include "Ente/Entidade/Entidade.h"

namespace Listas {
    ListaEntidades::ListaEntidades() {
        LEntidade = new Lista <Entidades::Entidade>();
    }

    ListaEntidades::~ListaEntidades() {
        LEntidade->limpar();
        delete LEntidade;
        LEntidade = NULL;
    }

    bool ListaEntidades::incluirEntidade(Entidades::Entidade *E) const {
        if (LEntidade) {
            return LEntidade->incluirInfo(E);
        }
        return false;
    }

    void ListaEntidades::limparLista() const {
        if (LEntidade) LEntidade->limpar();
    }

    void ListaEntidades::desenharTodas() const {
        if (!LEntidade) return;

        Lista<Entidades::Entidade>::IteratorLista it = LEntidade->getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);

        while (it != fim) {
            if (it->getVigente())
                it->desenhar();
            ++it;
        }
    }

    void ListaEntidades::percorrer() const {
        if (!LEntidade) return;

        Lista<Entidades::Entidade>::IteratorLista it = LEntidade->getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);

        while (it != fim) {
            if (it->getVigente())
                it->executar();
            ++it;
        }
    }
    void ListaEntidades::salvarTodas(std::ostream& arq) const {
        if (!LEntidade) return;

        Lista<Entidades::Entidade>::IteratorLista it = LEntidade->getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);

        while (it != fim) {
            if (&(*it)) it->salvarEm(arq);
            ++it;
        }
    }

    Entidades::Entidade* ListaEntidades::localizarInfo(Entidades::Entidade* E) const {
        if (!LEntidade || !E) return NULL;

        Lista<Entidades::Entidade>::IteratorLista it = LEntidade->getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);

        while (it != fim) {
            if (it && &(*it) == E) return &(*it);
            ++it;
        }
        return NULL;
    }

    Entidades::Entidade* ListaEntidades::localizarNome(const std::string& nome) const {
        if (!LEntidade) return NULL;

        Lista<Entidades::Entidade>::IteratorLista it = LEntidade->getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);

        while (it != fim) {
            Entidades::Entidade* pEntidade = it.proximo();
            if (pEntidade && pEntidade->getNome() == nome) return pEntidade;
        }
        return NULL;
    }
}
