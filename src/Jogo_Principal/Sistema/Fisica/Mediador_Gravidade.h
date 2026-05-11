//
// Created by Henrique on 10/05/2026.
//

#ifndef JOGO_MEDIADOR_GRAVIDADE_H
#define JOGO_MEDIADOR_GRAVIDADE_H
#include "SFML/System.hpp"

namespace Entidades {
    class Entidade;
}
namespace Gerenciadores {
    class Mediador_Gravidade {
        private:
            Mediador_Gravidade(const Mediador_Gravidade&){}
            Mediador_Gravidade& operator=(const Mediador_Gravidade&){}
        protected:
            Mediador_Gravidade(){};
        public:
            virtual ~Mediador_Gravidade() {}
            // Exemplos
            virtual void atualizarGravidade(Entidades::Entidade* entidade, float dt) = 0;
            virtual void aplicarGravidade(Entidades::Entidade* entidade, bool afetada) = 0;
            virtual float getGravidade() const = 0;
            virtual void setGravidade(float gravidade) = 0;
            virtual void aoTocarChao(Entidades::Entidade* entidade, const sf::Vector2f& normal) = 0;

    };
} // Sistemas

#endif //JOGO_MEDIADOR_GRAVIDADE_H