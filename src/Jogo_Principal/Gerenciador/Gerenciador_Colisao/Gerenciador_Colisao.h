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

class Entidade;
namespace Entidades {
    class Projetil;
    class Obstaculo;
    class Jogador;
    class Inimigo;
}

namespace Gerenciadores {
    class Gerenciador_Colisao {
        private:
            vector <Entidades::Obstaculo*> Lobstaculos;
            vector <Entidades::Inimigo*> Linimigos;
            vector <Entidades::Projetil*> Lprojetil;
            vector <Entidades::Jogador*> Ljogadores;

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
            void mediarComJogadores(Entidade* entidade);
            void colisao_Obstaculo_Jogador();
            void colisao_Projetil_Jogador();
            void colisao_Inimigo_Jogador();
            void limpar();
        public:
            static Gerenciador_Colisao& getInstancia();
            ~Gerenciador_Colisao();

            void incluirEntidade(Entidade* entidade);
            void removerEntidade(Entidade* entidade);

            bool colidiu(Entidade* entidade, Entidade* jogador);
            void calculaColisao(Entidade* entidade, Entidade* jogador);
            bool testarColisao(Entidade* entidade, Entidade* jogador);

            void verificaColisao(Entidade* entidade, Entidade* jogador);
            void verificaObstaculo();
            void verificaProjetil();
            void verificaInimigo();
            void executar();
    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_COLISAO_H