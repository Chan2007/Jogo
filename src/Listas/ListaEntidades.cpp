//
// Created by Henrique on 29/04/2026.
//

#include "ListaEntidades.h"
#include "Elemento.h"
#include "Lista.h"
#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Listas {
    ListaEntidades::ListaEntidades() {
        LEntidade = new Lista <Entidades::Entidade>();
    }

    ListaEntidades::~ListaEntidades() {
        LEntidade->limpar();
        if (LEntidade) delete LEntidade;
        LEntidade = NULL;
    }

    bool ListaEntidades::incluirEntidade(Entidades::Entidade *E) const {
        if (LEntidade) {
            return LEntidade->incluirInfo(E);
        }
        return false;
    }

    void ListaEntidades::limparLista() const {

        if (LEntidade) {
            LEntidade->limpar();
        }
    }

    void ListaEntidades::desenharTodas(sf::RenderWindow& janela) const {

        if (!LEntidade) return;

        Elemento<Entidades::Entidade>* pAux = LEntidade->getPrimeiro();

        while (pAux != NULL) {
            Entidades::Entidade* pEntidade = pAux->getInfo();
            if (pEntidade) {
                if (pEntidade->getVigente()) {
                    Entidades::Projetil* proj = dynamic_cast<Entidades::Projetil*>(pEntidade);
                    if (proj) {
                        if (proj->getVigente()) {}
                            proj->desenhar();
                    }
                    else pEntidade->desenhar();
                }
            }
            pAux = pAux->getProximo();
        }
    }

    void ListaEntidades::percorrer() const {
        if (!LEntidade) return;

        Elemento<Entidades::Entidade>* pAux = LEntidade->getPrimeiro();

        while (pAux != NULL) {
            Entidades::Entidade* pEntidade = pAux->getInfo();
            if (pEntidade) {
                if (pEntidade->getVigente()) {
                    pEntidade->executar();
                }
            }
            pAux = pAux->getProximo();
        }
    }

    void ListaEntidades::salvarTodas(std::ostream& arq) const {
        if (!LEntidade) return;

        Elemento<Entidades::Entidade>* pAux = LEntidade->getPrimeiro();

        while (pAux != NULL) {
            Entidades::Entidade* pEntidade = pAux->getInfo();
            if (pEntidade) {
                pEntidade->salvarEm(arq);
            }
            pAux = pAux->getProximo();
        }
    }

    Elemento<Entidades::Entidade>* ListaEntidades::localizarEntidade(Elemento <Entidades::Entidade>* E) const {
        if (!LEntidade) return NULL;
        return LEntidade->localizarElemento(E);
    }

    Entidades::Entidade* ListaEntidades::localizarInfo(Entidades::Entidade* E) const {
        if (!LEntidade) return NULL;
        return LEntidade->localizarInfo(E);
    }

    Entidades::Entidade* ListaEntidades::localizarNome(const std::string& nome) const {
        if (!LEntidade) return NULL;
        return LEntidade->localizarNome(nome);
    }
}
