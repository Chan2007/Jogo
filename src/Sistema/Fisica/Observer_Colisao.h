//
// Created by Henrique on 08/05/2026.
//

#ifndef JOGO_MEDIADOR_COLISAO_H
#define JOGO_MEDIADOR_COLISAO_H

namespace Entidades {
    class Entidade;
}
namespace Gerenciadores {
    class Observer_Colisao {
        private:
            Observer_Colisao(const Observer_Colisao&);
            Observer_Colisao& operator=(const Observer_Colisao&);
        protected:
            Observer_Colisao(){};
        public:
            virtual ~Observer_Colisao() {}
            virtual void aoColidir(Entidades::Entidade* E1, Entidades::Entidade* E2){};
        };

} // Gerenciador

#endif //JOGO_MEDIADOR_COLISAO_H