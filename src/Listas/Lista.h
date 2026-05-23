//
// Created by Henrique on 29/04/2026.
//

#ifndef JOGO_LISTA_H
#define JOGO_LISTA_H
#include "Elemento.h"
#include <string>

template <class tipo>
class Lista {
    private:
        Elemento <tipo>* pPrimeiro;
        Elemento <tipo>* pAtual;
    public:
        Lista();
        ~Lista();
        bool incluirElemento(Elemento<tipo>* E);
        bool incluirInfo(tipo* i);
        void limpar();
        Elemento <tipo>* localizarElemento(Elemento <tipo>* E);
        tipo* localizarInfo(tipo* i);
        tipo* localizarNome(std::string nome);
};

template<class tipo>
Lista<tipo>::Lista() {
    pPrimeiro = NULL;
    pAtual = NULL;
}
template<class tipo>
Lista<tipo>::~Lista() {
    limpar();
}
template<class tipo>
bool Lista <tipo>::incluirElemento(Elemento <tipo>* E) {
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
    E->setProximo(NULL);
    return true;
}
template<class tipo>
bool Lista <tipo>::incluirInfo(tipo* i) {
    if (!i) return false;
    Elemento <tipo>* pElemento = NULL;
    pElemento = new Elemento <tipo>();
    pElemento->setInfo(i);
    incluirElemento(pElemento);
    return true;
}
template<class tipo>
void Lista <tipo>::limpar() {
    Elemento <tipo>* pAux = pPrimeiro;
    while (pAux) {
        Elemento <tipo>* temp = pAux;
        pAux = pAux->getProximo();
        delete temp;
    }
    pPrimeiro = NULL;
    pAtual = NULL;
}
template<class tipo>
Elemento <tipo>* Lista <tipo>::localizarElemento(Elemento <tipo>* E){
    if (!E) return NULL;
    Elemento <tipo>* pAux = pPrimeiro;
    while (pAux) {
        if (pAux == E) return pAux;
        pAux = pAux->getProximo();
    }
    return NULL;
}
template<class tipo>
tipo* Lista <tipo>::localizarInfo(tipo* i){
    if (!i) return NULL;
    Elemento <tipo>* pAux = pPrimeiro;
    while (pAux) {
        if (pAux->getInfo() == i) return pAux->getInfo();
        pAux = pAux->getProximo();
    }
    return NULL;
}
template <class tipo>
tipo* Lista <tipo>::localizarNome(std::string nome){
    if (nome.empty()) return NULL;
    Elemento <tipo>* pAux = pPrimeiro;
    while (pAux) {
        if (pAux->getInfo()->getNome() == nome) return pAux->getInfo();
        pAux = pAux->getProximo();
    }
    return NULL;
}
#endif //JOGO_LISTA_H