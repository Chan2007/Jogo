//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_COLISAO_H
#define JOGO_GERENCIADOR_COLISAO_H
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>

using namespace sf;
using namespace std;

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
    class Gerenciador_Colisao {
        private:
            vector <Obstaculos::Obstaculo*> Lobstaculos;
            vector <Personagens::Inimigo*> Linimigos;
            vector <Entidades::Projetil*> Lprojetil;
            vector <Personagens::Jogador*> Ljogadores;

            Gerenciador_Colisao();

            // Proibir o uso de construtora de cópia e atribuição à cópias
            Gerenciador_Colisao(const Gerenciador_Colisao&);
            Gerenciador_Colisao& operator=(const Gerenciador_Colisao&);

            template <class tipo>
            void incluirNaLista(vector <tipo*>& lista, tipo* entidade) {
                if (!entidade) return;
                if (find(lista.begin(), lista.end(), entidade) == lista.end())
                    lista.push_back(entidade);
            }
            template <class tipo>
            void removerDaLista(vector <tipo*>& lista, tipo* entidade) {
                lista.erase(remove(lista.begin(), lista.end(), entidade), lista.end());
            }
            template <class tipo>
            void colisao_Entidade_Classe(vector <tipo*>& lista, Entidades::Entidade* entidade) {
                for (size_t i = 0; i < lista.size(); i++)
                    implementarColisao(lista[i], entidade);
            }

            void limpar();
            static bool colidiu(const Entidades::Entidade *entidade, const Entidades::Entidade *movel);
            static void calculaColisao(const Entidades::Entidade *entidade, Entidades::Entidade *movel);
            static bool implementarColisao(const Entidades::Entidade *entidade, Entidades::Entidade *movel);

        public:
            static Gerenciador_Colisao& getInstancia();
            ~Gerenciador_Colisao();

            void incluirEntidade(Entidades::Entidade* entidade);
            void removerEntidade(Entidades::Entidade* entidade);
            static void verificaColisao(const Entidades::Entidade* entidade, Entidades::Entidade* movel);
            void verificaObstaculo(Entidades::Entidade* entidade);
            void verificaProjetil(Entidades::Entidade* entidade);
            void verificaInimigo(Entidades::Entidade* entidade);
            void verificaJogador(Entidades::Entidade* entidade);
            void executar(Entidades::Entidade* entidade);
    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_COLISAO_H