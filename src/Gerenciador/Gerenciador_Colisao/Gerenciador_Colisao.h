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

namespace Fases {
    class Fase;
}

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

    class Gerenciador_Colisao {
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
            static bool verificarColisao(Entidades::Entidade *entidade, Entidades::Entidade *movel);
            void tratarColisoesJogsObstacs() const;
            void tratarColisoesJogsProjeteis() const;
            void tratarColisoesJogsInimgs() const;
            void tratarColisoesJogs() const;

            void tratarColisoesInim() const;
            void tratarColisoesProj() const;

            void executar();
    };
} // Gerenciador

#endif //JOGO_GERENCIADOR_COLISAO_H
