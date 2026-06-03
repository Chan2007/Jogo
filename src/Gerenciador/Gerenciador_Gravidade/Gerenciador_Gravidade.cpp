//
// Created by Henrique on 10/05/2026.
//

#include "Gerenciador_Gravidade.h"
#include <cmath>
#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Personagem.h"
#include "Ente/Entidade/Projetil/Projetil.h"

namespace Gerenciadores {
    Gerenciador_Gravidade* Gerenciador_Gravidade::gerenciador_gravidade = NULL;
    Gerenciador_Gravidade::Gerenciador_Gravidade():
        gravidade(500.0f),
        vyTerminal(800.0f),
        forcaPulo(400.0f),
        atrito(0.9f),
        restituicao(0.9f)
    {}

    Gerenciador_Gravidade::~Gerenciador_Gravidade() {
        limpar();
    }
    Gerenciador_Gravidade& Gerenciador_Gravidade::getInstancia() {
        if (!gerenciador_gravidade) gerenciador_gravidade = new Gerenciador_Gravidade();
        return *gerenciador_gravidade;
    }
    void Gerenciador_Gravidade::limpar() {
        entidadesAfetadas.clear();
        entidadesNoChao.clear();
    }
    void Gerenciador_Gravidade::aplicarGravidade(Entidades::Entidade* entidade, bool afetada) {
        if (!entidade) return;
        if (afetada) {
            if (find(entidadesAfetadas.begin(), entidadesAfetadas.end(), entidade) == entidadesAfetadas.end())
                entidadesAfetadas.push_back(entidade);
        }
        else {
            entidadesAfetadas.erase(
            std::remove(entidadesAfetadas.begin(), entidadesAfetadas.end(), entidade),
            entidadesAfetadas.end()
            );
            entidadesNoChao.erase(
            std::remove(entidadesNoChao.begin(), entidadesNoChao.end(), entidade),
            entidadesNoChao.end()
            );
        }
    }
    void Gerenciador_Gravidade::atualizarGravidade(Entidades::Entidade* entidade, float dt) {
        if (!entidade) return;
        /*/if (estaNoChao(entidade)) {
            setVy(entidade, 0.0f);
            return;
        }*/
        float vy = getVy(entidade);
        vy += gravidade * dt;
        if (vy > vyTerminal) vy = vyTerminal;
        sf::Vector2f posicao = entidade->getPosicao();
        posicao.y += vy * dt;
        entidade->setPosicao(posicao);
        setVy(entidade, vy);
    }
    void Gerenciador_Gravidade::executar(float dt) {
        entidadesNoChao.clear();
        for (int i = 0; i < static_cast<int>(entidadesAfetadas.size()); ++i)
            atualizarGravidade(entidadesAfetadas[i], dt);
    }
    void Gerenciador_Gravidade::aoTocarChao(Entidades::Entidade* entidade, const sf::Vector2f& normal) {
        if (!entidade) return;

        // - 0,5 é o limite da inclinação, menor disso significa que a entidade tocou em um "chão"
        if (normal.y < -0.5f && getVy(entidade) >= 0.0f) {
            if (find(entidadesNoChao.begin(), entidadesNoChao.end(), entidade) == entidadesNoChao.end())
                entidadesNoChao.push_back(entidade);
            setVy(entidade, 0.0f);
        }
    }
    bool Gerenciador_Gravidade::pular(Entidades::Entidade* entidade, float f) {
        if (!entidade) return false;
        if (!estaNoChao(entidade)) return false;
        float forca = (f > 0.0f) ? f : forcaPulo;

        // Negativo = Subindo
        setVy(entidade, -forca);
        entidadesNoChao.erase(remove(entidadesNoChao.begin(), entidadesNoChao.end(), entidade),entidadesNoChao.end());
        return true;
    }
    bool Gerenciador_Gravidade::estaNoChao(const Entidades::Entidade* entidade) const {
        if (!entidade) return false;
        return find(entidadesNoChao.begin(), entidadesNoChao.end(), entidade) != entidadesNoChao.end();
    }
    float Gerenciador_Gravidade::getVy(Entidades::Entidade* entidade) {
        if (!entidade) return -1.0f;
        Personagens::Personagem* Personagem = dynamic_cast<Personagens::Personagem*>(entidade);
        Entidades::Projetil* Projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (Personagem) return Personagem->getVelocidade().y;
        if (Projetil) return Projetil->getVelocidade().y;
        return 0.0f;
    }
    void Gerenciador_Gravidade::setVy(Entidades::Entidade* entidade, float velocidade) {
        if (!entidade) return;
        Personagens::Personagem* Personagem = dynamic_cast<Personagens::Personagem*>(entidade);
        Entidades::Projetil* Projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (Personagem) {
            sf::Vector2f vel = Personagem->getVelocidade();
            vel.y = velocidade;
            Personagem->setVelocidade(vel);
        }
        if (Projetil) {
            sf::Vector2f vel = Projetil->getVelocidade();
            vel.y = velocidade;
            Projetil->setVelocidade(vel);
        }
    }
    void Gerenciador_Gravidade::aplicarAtrito(Entidades::Entidade* entidade, float dt) const {
        if (!entidade) return;
        if (!estaNoChao(entidade)) return;
        Personagens::Personagem* Personagem = dynamic_cast<Personagens::Personagem*>(entidade);
        Entidades::Projetil* Projetil = dynamic_cast<Entidades::Projetil*>(entidade);
        if (Personagem) {
            sf::Vector2f vel = Personagem->getVelocidade();
            vel.x *= atrito;
            // Se a velocidade em módulo por pequena demais, zera ela
            if (std::fabs(vel.x) < 1.0f) vel.x = 0.0f;
            Personagem->setVelocidade(vel);
        }
        if (Projetil) {
            sf::Vector2f vel = Projetil->getVelocidade();
            vel.x *= atrito;
            if (fabs(vel.x) < 1.0f) vel.x = 0.0f;
            Projetil->setVelocidade(vel);
        }
    }
} // Gerenciadores