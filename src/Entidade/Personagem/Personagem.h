//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_PERSONAGEM_H
#define JOGO_PERSONAGEM_H

#include "Entidade/Entidade.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

namespace Entidades {
    class Entidade;
}

namespace Personagens {
    enum EstadoCombate {
        ESTADO_OCIOSO = 0,
        ESTADO_MOVIMENTO,
        ESTADO_COMBATE,
        ESTADO_CASTANDO,
        ESTADO_MORTO
    };

    enum EstadoAnimacao {
        PARADO,
        ANDANDO,
        PULANDO,
        CAINDO,
        ATACANDO
    };
    EstadoAnimacao estadoAnimacao;

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

            int vida;
            int vidaMaxima;
            int mana;
            int manaMaxima;
            int forca;
            int armadura;
            int resistenciaMagica;
            int alcanceAtaque;
            float velocidadeMovimento;
            int chanceCritica;
            float vampirismo;
            float regeneracaoVida;
            float regeneracaoMana;
            int estado;
        public:
            Personagem();
            virtual ~Personagem();

            sf::Vector2f getVelocidade() const {return velocidade;}
            sf::Vector2f getAceleracao() const {return aceleracao;}
            void setVelocidade(sf::Vector2f v) {velocidade.x = v.x; velocidade.y = v.y;}
            void setAceleracao(sf::Vector2f a) {aceleracao.x = a.x; aceleracao.y = a.y;}
            void setVy(float vy) {velocidade.y = vy;}
            void setVx(float vx) {velocidade.x = vx;}
            float getVidaPercentual() const;
            int getVida() const {return vida;}
            int getVidaMaxima() const {return vidaMaxima;}
            int getMana() const {return mana;}
            int getManaMaxima() const {return manaMaxima;}
            int getForca() const {return forca;}
            int getArmadura() const {return armadura;}
            int getResistenciaMagica() const {return resistenciaMagica;}
            int getAlcanceAtaque() const {return alcanceAtaque;}
            int getVelocidadeMovimento() const {return velocidadeMovimento;}
            int getChanceCritica() const {return chanceCritica;}
            float getVampirismo() const {return vampirismo;}
            int getEstado() const {return estado;}
            bool estaVivo() const {return vida > 0;}
            bool estaSemMana() const {return mana <= 0;}
            void setVidaMaxima(int valor);
            void setManaMaxima(int valor);
            void setVida(int valor);
            void setMana(int valor);
            void setForca(int valor) {if (valor >= 0) forca = valor;}
            void setArmadura(int valor) {if (valor >= 0) armadura = valor;}
            void setResistenciaMagica(int valor) {if (valor >= 0) resistenciaMagica = valor;}
            void setAlcanceAtaque(int valor) {if (valor >= 0) alcanceAtaque = valor;}
            void setVelocidadeMovimento(int valor) {if (valor >= 0) velocidadeMovimento = valor;}
            void setChanceCritica(int valor) {chanceCritica = (valor < 0) ? 0 : ((valor > 100) ? 100 : valor);}
            void setVampirismo(float valor) {vampirismo = (valor < 0.0f) ? 0.0f : valor;}
            void setRegeneracaoVida(float valor) {regeneracaoVida = (valor < 0.0f) ? 0.0f : valor;}
            void setRegeneracaoMana(float valor) {regeneracaoMana = (valor < 0.0f) ? 0.0f : valor;}
            void setEstado(EstadoCombate novoEstado) {estado = static_cast<int>(novoEstado);}
            int receberDanoFisico(int dano);
            int receberDanoMagico(int dano);
            int causarDanoBasico() const;
            void curar(int valor);
            bool gastarMana(int custo);
            void restaurarMana(int valor);
            void regenerarAtributos(float deltaTempo);
            void moverHorizontal(float direcao);
            void resetarCombate();
            virtual void desenhar(sf::RenderWindow& window);
            virtual void salvarDataBuffer();
            virtual void atualizar() = 0;
            virtual void salvar() = 0;
            virtual void mover() = 0;
    };
} // Personagens

#endif //JOGO_PERSONAGEM_H