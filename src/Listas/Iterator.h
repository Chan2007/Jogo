//
// Created by Henrique on 17/06/2026.
//

#ifndef JOGO_ITERATOR_H
#define JOGO_ITERATOR_H

template <class tipo>
class Iterator {
    public:
        virtual bool hasProximo() const = 0;
        virtual bool hasAnterior() const = 0;

        virtual tipo* proximo() = 0;
        virtual tipo* anterior() = 0;

        virtual tipo& operator*() = 0;
        virtual tipo* operator->() = 0;
        virtual Iterator& operator++() = 0;
        virtual Iterator& operator--() = 0;
        virtual bool operator!=(const Iterator& it) const = 0;
        virtual bool operator==(const Iterator& it) const = 0;
        virtual explicit operator bool() const = 0;  // Praticamente quase igual para qualquer tipo de iterator
        virtual ~Iterator() {}
};

#endif //JOGO_ITERATOR_H