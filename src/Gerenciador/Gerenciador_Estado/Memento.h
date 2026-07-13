//
// Created by Henrique on 20/06/2026.
//

#ifndef JOGO_MEMENTO_H
#define JOGO_MEMENTO_H
#include <fstream>
#include <string>
namespace Gerenciadores {
    class Memento {
        public:
            virtual ~Memento() {};
            virtual void gravarNoArquivo(std::ofstream& arquivo, int fase) const {}

            virtual std::string getTipo() const { return ""; }
    };
}

#endif //JOGO_MEMENTO_H