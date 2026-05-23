//
// Created by Henrique on 10/05/2026.
//

#ifndef JOGO_GERENCIADOR_GRAVIDADE_H
#define JOGO_GERENCIADOR_GRAVIDADE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Sistema/Fisica/Mediador_Gravidade.h"


namespace Gerenciadores {
    class Gerenciador_Gravidade: public Mediador_Gravidade {
        private:
            float gravidade;
            float vyTerminal;
            float forcaPulo;
            float atrito;
            float restituicao;
            std::vector<Entidades::Entidade*> entidadesAfetadas;
            std::vector<Entidades::Entidade*> entidadesNoChao;

        public:
            Gerenciador_Gravidade();
            ~Gerenciador_Gravidade();
            virtual void atualizarGravidade(Entidades::Entidade* entidade, float dt);
            virtual void aplicarGravidade(Entidades::Entidade* entidade, bool afetada);
            virtual float getGravidade() const {return gravidade;}
            virtual void setGravidade(float g) {gravidade = g;}
            virtual void aoTocarChao(Entidades::Entidade* entidade, const sf::Vector2f& normal);
            bool pular(Entidades::Entidade* entidade, float f = 0.0f);
            bool estaNoChao(const Entidades::Entidade* entidade) const;
            void aplicarAtrito(Entidades::Entidade* entidade, float dt) const;
            static float getVy(Entidades::Entidade* entidade) ;
            static void setVy(Entidades::Entidade* entidade, float velocidade);
            void setVelocidadeTerminal(float velocidade) {vyTerminal = velocidade;}
            void setForcaPulo(float forca) {forcaPulo = forca;}
            void setAtrito(float a) {atrito = a;}
            float getVelocidadeTerminal() const {return vyTerminal;}
            float getForcaPulo() const {return forcaPulo;}
            float getAtrito() const {return atrito;}
            float getRestituicao() const {return restituicao;}
            void limpar();
            void executar(float dt);

    };
} // Gerenciadores

#endif //JOGO_GERENCIADOR_GRAVIDADE_H