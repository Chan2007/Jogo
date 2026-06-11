//
// Created by Henrique on 06/05/2026.
//

#ifndef JOGO_GERENCIADOR_COLISAO_H
#define JOGO_GERENCIADOR_COLISAO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

#include "Sistema/Fisica/Observer_Colisao.h"

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

    class Gerenciador_Gravidade;

    class Gerenciador_Colisao: public Observer_Colisao {
        private:
            std::vector <Obstaculos::Obstaculo*> Lobstaculos;
            std::list <Personagens::Inimigo*> Linimigos;
            std::set <Entidades::Projetil*> Lprojetil;
            std::vector <Personagens::Jogador*> Ljogadores;

            template <class tipo>
            void incluirNaLista(std::vector<tipo*>& lista, tipo* entidade) {
                if (!entidade) return;
                if (std::find(lista.begin(), lista.end(), entidade) == lista.end())
                    lista.push_back(entidade);
            }
            template <class tipo>
            void incluirNaLista(std::list<tipo*>& lista, tipo* entidade) {
                if (!entidade) return;
                if (std::find(lista.begin(), lista.end(), entidade) == lista.end())
                    lista.push_back(entidade);
            }
            template <class tipo>
            static void incluirNaLista(std::set<tipo*>& lista, tipo* entidade) {
                if (!entidade) return;
                lista.insert(entidade);
            }

            template <class tipo>
            void removerDaLista(std::vector<tipo*>& lista, tipo* entidade) {
                lista.erase(std::remove(lista.begin(), lista.end(), entidade), lista.end());
            }
            template <class tipo>
            static void removerDaLista(std::list<tipo*>& lista, tipo* entidade) {
                lista.remove(entidade);
            }
            template <class tipo>
            static void removerDaLista(std::set<tipo*>& lista, tipo* entidade) {
                lista.erase(entidade);
            }
            template <class Container>
            static void colisao_Entidade_Classe(const Container& lista, Entidades::Entidade* entidade) {
                typename Container::const_iterator it;
                for (it = lista.begin(); it != lista.end(); ++it)
                    verificarColisao(*it, entidade);
            }
            static bool colidiu(const Entidades::Entidade *entidade, const Entidades::Entidade *movel);
            static void calculaColisao(const Entidades::Entidade *entidade, Entidades::Entidade *movel);
            static bool verificarLimitesJanela(Entidades::Entidade* entidade, const sf::Vector2u& tamanhoJanela,
                                               Gerenciador_Gravidade* pGravidade = NULL);
            Gerenciador_Colisao();
            Gerenciador_Colisao(const Gerenciador_Colisao&);
            Gerenciador_Colisao& operator=(const Gerenciador_Colisao&);
        public:
            static Gerenciador_Colisao& getGerenciador();
            ~Gerenciador_Colisao();
            void limpar();
            void incluirEntidade(Entidades::Entidade* entidade);
            void removerEntidade(Entidades::Entidade* entidade);
            bool verificarPosicaoLivre(const sf::FloatRect& hitboxProvisoria);
            static void verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel);
            void verificarObstaculo(Entidades::Entidade* entidade) const;
            void verificarProjetil(Entidades::Entidade* entidade) const;
            void verificarInimigo(Entidades::Entidade* entidade) const;
            void verificarJogador(Entidades::Entidade* entidade) const;
            void executar(const sf::Vector2u &tamanhoJanela, Gerenciador_Gravidade *pGravidade = NULL);
    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_COLISAO_H
