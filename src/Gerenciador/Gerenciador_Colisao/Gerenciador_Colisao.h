//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_COLISAO_H
#define JOGO_GERENCIADOR_COLISAO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#include "Sistema/Fisica/Mediador_Colisao.h"

namespace Entidades {
    class Entidade;
    class Projetil;
}

namespace Personagens {
    class Personagem;
    class Jogador;
    class Inimigo;
}

namespace Obstaculos {
    class Obstaculo;
}

namespace Gerenciadores {
    class Gerenciador_Colisao: public Mediador_Colisao {
        private:
            std::vector <Obstaculos::Obstaculo*> Lobstaculos;
            std::vector <Personagens::Inimigo*> Linimigos;
            std::vector <Entidades::Projetil*> Lprojetil;
            std::vector <Personagens::Jogador*> Ljogadores;

            Gerenciador_Colisao();
            static Gerenciador_Colisao* gerenciador_colisao;

            // Proibir o uso de construtora de cópia e atribuição à cópias
            Gerenciador_Colisao(const Gerenciador_Colisao&);
            Gerenciador_Colisao& operator=(const Gerenciador_Colisao&);

            template <class tipo>
            void incluirNaLista(std::vector <tipo*>& lista, tipo* entidade) {
                if (!entidade) return;
                if (find(lista.begin(), lista.end(), entidade) == lista.end())
                    lista.push_back(entidade);
            }
            template <class tipo>
            void removerDaLista(std::vector <tipo*>& lista, tipo* entidade) {
                lista.erase(remove(lista.begin(), lista.end(), entidade), lista.end());
            }
            template <class tipo>
            static void colisao_Entidade_Classe(std::vector <tipo*>& lista, Entidades::Entidade* entidade) {
                for (size_t i = 0; i < lista.size(); i++)
                    verificarColisao(lista[i], entidade);
            }

            void limpar();
            static bool colidiu(const Entidades::Entidade *entidade, const Entidades::Entidade *movel);
            static void calculaColisao(const Entidades::Entidade *entidade, Entidades::Entidade *movel);
            static bool verificarLimitesJanela(Entidades::Entidade* entidade);
        public:
            static Gerenciador_Colisao& getInstancia();
            ~Gerenciador_Colisao();
            void incluirEntidade(Entidades::Entidade* entidade);
            void removerEntidade(Entidades::Entidade* entidade);
            static void verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel);
            virtual void aoColidir(Entidades::Entidade* E1, Entidades::Entidade* E2){};
            void verificarObstaculo(Entidades::Entidade* entidade);
            void verificarProjetil(Entidades::Entidade* entidade);
            void verificarInimigo(Entidades::Entidade* entidade);
            void verificarJogador(Entidades::Entidade* entidade);
            void executar(Entidades::Entidade* entidade);
    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_COLISAO_H
