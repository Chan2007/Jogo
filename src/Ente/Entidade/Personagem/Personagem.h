

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Ente/Entidade/Entidade.h"

namespace Entidades {
    class Entidade;
}

namespace Personagens {
    enum Estado {
        ESTADO_OCIOSO = 0,
        ESTADO_MOVIMENTO = 1,
        ESTADO_AR = 2,
        ESTADO_COMBATE = 3,
        ESTADO_MORTO = 4
    };

    class Personagem : public Entidades::Entidade {
        protected:
            sf::Vector2f velocidade;
            sf::Vector2f aceleracao;
            std::string caminhoArquivoSpriteOcioso;
            std::string caminhoArquivoSpriteMovimento;
            std::string caminhoArquivoSpritePulo;
            std::string caminhoArquivoSpriteAtaque;
            sf::Texture* texturaOcioso;
            sf::Texture* texturaMovimento;
            sf::Texture* texturaPulo;
            sf::Texture* texturaAtaque;
            sf::IntRect rectAtual;
            sf::Clock clockAnimacao;
            float tempoPorFrame;
            float frameAcumulado;
            int indexFrameAtual;
            int totalFramesAnimacao;
            int colunasSpritesheet;
            int linhasSpritesheet;
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
            class PersonagemMemento : public EntidadeMemento {
                private:
                    sf::Vector2f velocidadeMemento;
                    sf::Vector2f aceleracaoMemento;
                    std::string caminhoArquivoSpriteOciosoMemento;
                    std::string caminhoArquivoSpriteMovimentoMemento;
                    std::string caminhoArquivoSpritePuloMemento;
                    std::string caminhoArquivoSpriteAtaqueMemento;
                    sf::Texture* texturaOciosoMemento;
                    sf::Texture* texturaMovimentoMemento;
                    sf::Texture* texturaPuloMemento;
                    sf::Texture* texturaAtaqueMemento;
                    sf::IntRect rectAtualMemento;
                    sf::Clock clockAnimacaoMemento;
                    float tempoPorFrameMemento;
                    float frameAcumuladoMemento;
                    int indexFrameAtualMemento;
                    int totalFramesAnimacaoMemento;
                    int colunasSpritesheetMemento;
                    int linhasSpritesheetMemento;
                    int frameWidthMemento;
                    int frameHeightMemento;
                    float velocidadeMaxMemento;
                    float vidaMemento;
                    float vidaMaximaMemento;
                    float poderMemento;
                    int alcanceAtaqueMemento;
                    float chanceCriticaMemento;
                    float regeneracaoVidaMemento;
                    int estadoMemento;
                    sf::Clock clockDanoMemento;
                    float tempoDanoMemento;
                    bool invulneravelMemento;

                public:
                    // O construtor recebe o Personagem inteiro e repassa para a Entidade base
                    PersonagemMemento(const Personagem& p) : EntidadeMemento(p),
                    velocidadeMemento(p.velocidade), aceleracaoMemento(p.aceleracao),
                    rectAtualMemento(p.rectAtual), tempoPorFrameMemento(p.tempoPorFrame),
                    frameAcumuladoMemento(p.frameAcumulado), indexFrameAtualMemento(p.indexFrameAtual),
                    totalFramesAnimacaoMemento(p.totalFramesAnimacao), colunasSpritesheetMemento(p.colunasSpritesheet),
                    linhasSpritesheetMemento(p.linhasSpritesheet), frameWidthMemento(p.frameWidth),
                    frameHeightMemento(p.frameHeight), velocidadeMaxMemento(p.velocidadeMax),
                    vidaMemento(p.vida), vidaMaximaMemento(p.vidaMaxima), poderMemento(p.poder),
                    alcanceAtaqueMemento(p.alcanceAtaque), chanceCriticaMemento(p.chanceCritica),
                    regeneracaoVidaMemento(p.regeneracaoVida), estadoMemento(p.estado), tempoDanoMemento(p.tempoDano),
                    invulneravelMemento(p.invulneravel), caminhoArquivoSpriteOciosoMemento(p.caminhoArquivoSpriteOcioso),
                    caminhoArquivoSpriteMovimentoMemento(p.caminhoArquivoSpriteMovimento),
                    caminhoArquivoSpritePuloMemento(p.caminhoArquivoSpritePulo),
                    caminhoArquivoSpriteAtaqueMemento(p.caminhoArquivoSpriteAtaque), texturaAtaqueMemento(p.texturaAtaque),
                    texturaMovimentoMemento(p.texturaMovimento), texturaOciosoMemento(p.texturaOcioso),
                    texturaPuloMemento(p.texturaPulo) {}

                    virtual ~PersonagemMemento() {}

                    friend class Personagem; // Permite ao Personagem ler os dados privados ao restaurar
                };
        public:
            Personagem();
            virtual ~Personagem();

            virtual Gerenciadores::Memento* salvarMemento() const;
            virtual void restaurarMemento(const Gerenciadores::Memento* memento);

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
            float getVida() const { return vida; }
            float getPoder() const { return poder; }
            bool estaVivo() const { return vida > 0; }

            void setVidaMaxima(float valor);
            void setVida(float valor);
            void setPoder(int valor) { if (valor >= 0) poder = valor; }
            void setEstado(Estado novoEstado) { estado = static_cast<int>(novoEstado); }

            void setInvulneravel(bool i) { invulneravel = i; }
            bool getInvulneravel() { return invulneravel; }

            float receberDano(float dano);
            float causarDanoNormal() const;
            void curar(float valor);
            void regenerarVida(float dt);
            void moverHorizontal(float direcao);
            void resetarCombate();

            void salvarPersonagem();
            virtual void executar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H
