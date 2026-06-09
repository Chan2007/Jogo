//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Ente/Entidade/Entidade.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

namespace Entidades {
    class Entidade;
}

namespace Personagens {
    enum EstadoCombate {
        ESTADO_OCIOSO = 0,
        ESTADO_MOVIMENTO = 1,
        ESTADO_COMBATE = 2,
        ESTADO_MORTO = 3
    };

    enum EstadoAnimacao {
        PARADO = 0,
        ANDANDO = 1,
        PULANDO = 2,
        CAINDO = 3,
        ATACANDO = 4
    };

    class Personagem : public Entidades::Entidade {
    private:
        sf::Vector2f velocidade;
        sf::Vector2f aceleracao;
    protected:
        std::string caminhoArquivoSprite;
        std::string caminhoArquivoSpritePulo;
        sf::Texture texturaPulo;
        sf::IntRect rectAtual;
        sf::Clock clockAnimacao;
        float tempoPorFrame;
        float frameAcumulado;
        int indexFrameAtual;
        int totalFramesAnimacao;
        int colunasSpritesheet;
        int frameWidth;
        int frameHeight;

        float velocidadeMax;
        int vida;
        int vidaMaxima;
        int poder;
        int alcanceAtaque;
        int chanceCritica;
        float regeneracaoVida;
        int estado;
    public:
        Personagem();
        virtual ~Personagem();

        sf::FloatRect getTamanho() const;
        sf::Vector2f getVelocidade() const { return velocidade; }
        sf::Vector2f getAceleracao() const { return aceleracao; }
        void setVelocidade(sf::Vector2f v) { velocidade.x = v.x; velocidade.y = v.y; }
        void setAceleracao(sf::Vector2f a) { aceleracao.x = a.x; aceleracao.y = a.y; }
        // void setVy(float vy) {velocidade.y = vy;}
        // void setVx(float vx) {velocidade.x = vx;}
        float getVidaPercentual() const;
        int getVida() const { return vida; }
        int getVidaMaxima() const { return vidaMaxima; }
        int getPoder() const { return poder; }
        int getAlcanceAtaque() const { return alcanceAtaque; }
        int getChanceCritica() const { return chanceCritica; }
        int getEstado() const { return estado; }
        bool estaVivo() const { return vida > 0; }
        void setVidaMaxima(int valor);
        void setVida(int valor);
        void setPoder(int valor) { if (valor >= 0) poder = valor; }
        void setAlcanceAtaque(int valor) { if (valor >= 0) alcanceAtaque = valor; }
        void setChanceCritica(int valor) { chanceCritica = (valor < 0) ? 0 : ((valor > 100) ? 100 : valor); }
        void setRegeneracaoVida(float valor) { regeneracaoVida = (valor < 0.0f) ? 0.0f : valor; }
        void setEstado(EstadoCombate novoEstado) { estado = static_cast<int>(novoEstado); }
        int receberDano(int dano);
        int causarDanoBasico() const;
        void curar(int valor);
        void regenerarVida(float deltaTempo);
        void moverHorizontal(float direcao);
        void resetarCombate();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H
