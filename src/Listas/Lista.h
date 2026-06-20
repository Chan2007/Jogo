//
// Created by Henrique on 29/04/2026.
//

#ifndef JOGO_LISTA_H
#define JOGO_LISTA_H

#include <string>
#include <iostream>

#include "Iterator.h"

template <class tipo>
class Lista {
    public:
        class Elemento {
            private:
                Elemento* pAnterior;
                Elemento* pProximo;
                tipo* pInfo;

            public:
                Elemento() : pAnterior(NULL), pProximo(NULL), pInfo(NULL) {}
                ~Elemento() {
                    pAnterior = NULL;
                    pProximo = NULL;
                    pInfo = NULL;
                }

                void setInfo(tipo* E) { if (E) pInfo = E; }
                tipo* getInfo() { return pInfo; }

                void setProximo(Elemento* E) { pProximo = E; }
                Elemento* getProximo() const { return pProximo; }

                void setAnterior(Elemento* E) { pAnterior = E; }
                Elemento* getAnterior() { return pAnterior; }

                bool operator==(Elemento* E) { return pInfo == E->getInfo(); }
        };
        class IteratorLista : public Iterator<tipo> {
            private:
                Elemento* pElemento;
            public:
                IteratorLista(Elemento* pPrimeiro) : pElemento(pPrimeiro) {}
                ~IteratorLista() {}

                bool hasProximo() const { return pElemento->getProximo() != NULL; }
                bool hasAnterior() const { return pElemento->getAnterior() != NULL; }
                tipo* proximo() {
                    if (!pElemento) return NULL;
                    tipo* info = pElemento->getInfo();
                    pElemento = pElemento->getProximo();
                    return info;
                }
                tipo* anterior() {
                    if (!pElemento) return NULL;
                    tipo* info = pElemento->getInfo();
                    pElemento = pElemento->getAnterior();
                    return info;
                }

                // Equivalente a fazer "*it" para pegar o objeto de dentro do nó.
                // Só se usa se o "tipo" for ponteiro!
                tipo& operator*() { return *pElemento->getInfo(); }

                // Permite chamar métodos do objeto direto pelo iterador
                tipo* operator->() { return pElemento->getInfo(); }

                // Avança para o próximo e retorna o iterador modificado
                IteratorLista& operator++() {
                    if (pElemento) pElemento = pElemento->getProximo();
                    return *this;
                }

                // Volta para o anterior e retorna o iterador modificado
                IteratorLista& operator--() {
                    if (pElemento) pElemento = pElemento->getAnterior();
                    return *this;
                }

                // Compara se dois iteradores estão em posições diferentes.
                // Bom para criar condições de parada em loops.
                bool operator!=(const Iterator<tipo>& it) const override {
                    const IteratorLista* tipoCertoIt = dynamic_cast<const IteratorLista*>(&it);
                    if (tipoCertoIt) return pElemento != tipoCertoIt->pElemento;

                    // Se forem de tipos de iteradores diferentes, obviamente são diferentes
                    return true;
                }
                bool operator==(const Iterator<tipo>& it) const override {
                    const IteratorLista* tipoCertoIt = dynamic_cast<const IteratorLista*>(&it);
                    if (tipoCertoIt) return pElemento == tipoCertoIt->pElemento;

                    return true;
                }
                explicit operator bool() const {
                    // Retorna true se o iterador apontar para um nó válido, e false se for nulo
                    return pElemento != NULL;
                }
        };
    private:
        Elemento* pPrimeiro;
        Elemento* pAtual;

    public:
        Lista();
        ~Lista();
        bool incluirElemento(Elemento* E);
        bool incluirInfo(tipo* i);
        void limpar();
        IteratorLista getPrimeiro() const { return IteratorLista(pPrimeiro); }
        IteratorLista getUltimo() const { return IteratorLista(pAtual); }

};

template<class tipo>
Lista<tipo>::Lista() : pPrimeiro(NULL), pAtual(NULL) {}

template<class tipo>
Lista<tipo>::~Lista() {
    limpar();
}

template<class tipo>
bool Lista<tipo>::incluirElemento(Elemento* E) {
    if (!E) return false;
    if (pPrimeiro == NULL) {
        pPrimeiro = E;
        pAtual = E;
        E->setAnterior(NULL);
    }
    else {
        pAtual->setProximo(E);
        E->setAnterior(pAtual);
        pAtual = E;
    }
    return true;
}

template<class tipo>
bool Lista<tipo>::incluirInfo(tipo* i) {
    if (!i) return false;
    Elemento* pElemento = new Elemento();
    pElemento->setInfo(i);
    incluirElemento(pElemento);
    return true;
}

template<class tipo>
void Lista<tipo>::limpar() {
    Elemento* pAux = pPrimeiro;
    while (pAux) {
        Elemento* temp = pAux;
        pAux = pAux->getProximo();
        delete temp;
    }
    pPrimeiro = NULL;
    pAtual = NULL;
}



#endif // JOGO_LISTA_H