//
// Created by Henrique on 29/04/2026.
//

#ifndef JOGO_ELEMENTO_H
#define JOGO_ELEMENTO_H

template <class tipo>
class Elemento {
    private:
        Elemento <tipo>* pAnterior;
        Elemento <tipo>* pProximo;
        tipo* pInfo;

    public:
        Elemento();
        ~Elemento();
        void setInfo(tipo* E);
        void setProximo(Elemento <tipo>* E);
        Elemento <tipo>* getProximo() const;
        tipo* getInfo();
        void setAnterior(Elemento <tipo>* E);
        Elemento <tipo>* getAnterior();
        bool operator==(Elemento <tipo>* E);
};

template <class tipo>
Elemento <tipo>::Elemento() {
    pAnterior = NULL;
    pProximo = NULL;
    pInfo = NULL;
}
template <class tipo>
Elemento <tipo>::~Elemento() {
    pAnterior = NULL;
    pProximo = NULL;
    pInfo = NULL;
}

template<class tipo>
void Elemento<tipo>::setInfo(tipo *E) {
    if (!E) return;
    pInfo = E;
}
template<class tipo>
tipo* Elemento<tipo>::getInfo() {
    return pInfo;
}
template<class tipo>
Elemento <tipo>* Elemento <tipo>::getProximo() const {
    return pProximo;
}
template<class tipo>
Elemento <tipo>* Elemento <tipo>::getAnterior() {
    return pAnterior;
}
template<class tipo>
void Elemento <tipo>::setProximo(Elemento <tipo>* E) {
    if (!E) return;
    pProximo = E;
}
template<class tipo>
void Elemento <tipo>::setAnterior(Elemento <tipo>* E) {
    if (!E) return;
    pAnterior = E;
}
template<class tipo>
bool Elemento <tipo>::operator==(Elemento <tipo>* E) {
    if (!E) return false;
    return pInfo == E->getInfo() && pProximo == E->getProximo() && pAnterior == E->getAnterior();
}

#endif //JOGO_ELEMENTO_H