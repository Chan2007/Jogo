//
// Created by Henrique on 08/05/2026.
//

#ifndef JOGO_MEDIADOR_COLISAO_H
#define JOGO_MEDIADOR_COLISAO_H

namespace Entidades {
    class Entidade;
}
namespace Gerenciadores {
    class Mediador_Colisao {
        private:
            Mediador_Colisao(const Mediador_Colisao&){}
            Mediador_Colisao& operator=(const Mediador_Colisao&){}
        protected:
            Mediador_Colisao(){};
        public:
            virtual ~Mediador_Colisao() {}
        };

} // Gerenciador

#endif //JOGO_MEDIADOR_COLISAO_H