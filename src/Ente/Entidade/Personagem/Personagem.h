

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Ente/Entidade/Entidade.h"

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
        float vida;
        float vidaMaxima;
        float poder;
        int alcanceAtaque;
        float chanceCritica;
        float regeneracaoVida;
        int estado;
        sf::Clock clockDano;
        float tempoDano;
        bool invulneravel;
    public:
        Personagem();
        virtual ~Personagem();

        virtual sf::FloatRect getTamanho() const;
        sf::Vector2f getVelocidade() const { return velocidade; }
        sf::Vector2f getAceleracao() const { return aceleracao; }

        // void setVy(float vy) {velocidade.y = vy;}
        // void setVx(float vx) {velocidade.x = vx;}

        void setVelocidade(sf::Vector2f v) {
            velocidade.x = v.x;
            velocidade.y = v.y;
        }
        void setAceleracao(sf::Vector2f a) {
            aceleracao.x = a.x;
            aceleracao.y = a.y;
        }
        void setVelocidade(const float* vx = NULL, const float* vy = NULL) {
            if (vx != NULL) velocidade.x = *vx;
            if (vy != NULL) velocidade.y = *vy;
        }
        void setAceleracao(const float* ax = NULL, const float* ay = NULL) {
            if (ax != NULL) aceleracao.x = *ax;
            if (ay != NULL) aceleracao.y = *ay;
        }
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
        void setChanceCritica(int valor) { chanceCritica = gerar_num_norm(50, 15.0f, 0.0f, 100.0f); }
        void setRegeneracaoVida(float valor) { regeneracaoVida = (valor < 0.0f) ? 0.0f : valor; }
        void setEstado(EstadoCombate novoEstado) { estado = static_cast<int>(novoEstado); }
        int receberDano(int dano);
        int causarDanoBasico() const;
        void curar(float valor);
        void regenerarVida(float dt);
        void moverHorizontal(float direcao);
        void resetarCombate();
        void setInvulneravel(bool i) { invulneravel = i; }
        bool getInvulneravel() { return invulneravel; }
        void salvarPersonagem();
        virtual void executar() = 0;
        virtual void salvar() = 0;
        virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H
