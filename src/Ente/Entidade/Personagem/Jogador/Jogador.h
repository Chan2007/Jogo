
#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include "Ente/Entidade/Personagem/Personagem.h"
#include "Sistema/Input/Observador_Input.h"
#include <fstream>

namespace Gerenciadores {
    class Observador_Input;
    class Gerenciador_Gravidade;
}

namespace Obstaculos {
    class Plataforma;
}
 
namespace Personagens {
    class Inimigo;
    class Jogador : public Personagem, public Gerenciadores::Observador_Input {
        private:
            bool movendoEsquerda;
            bool movendoDireita;
            bool pulando;
            bool atacando;
            bool correndo;
            bool usandoHabilidade;
            float pontos;
            int abates;
            int idJogador;
            sf::RectangleShape fundoVida;
            sf::RectangleShape barraVida;
            int frameWidthOcioso;
            int frameHeightOcioso;
            int frameWidthMovimento;
            int frameHeightMovimento;
            int frameWidthPulo;
            int frameHeightPulo;
            int frameWidthAtaque;
            int frameHeightAtaque;
            class JogadorMemento : public PersonagemMemento {
                private:
                    bool movendoEsquerdaMemento;
                    bool movendoDireitaMemento;
                    bool pulandoMemento;
                    bool atacandoMemento;
                    bool correndoMemento;
                    bool usandoHabilidadeMemento;
                    float pontosMemento;
                    int abatesMemento;
                    int idJogadorMemento;
                    sf::RectangleShape fundoVidaMemento;
                    sf::RectangleShape barraVidaMemento;
                    int frameWidthOciosoMemento;
                    int frameHeightOciosoMemento;
                    int frameWidthMovimentoMemento;
                    int frameHeightMovimentoMemento;
                    int frameWidthPuloMemento;
                    int frameHeightPuloMemento;
                    int frameWidthAtaqueMemento;
                    int frameHeightAtaqueMemento;

                    JogadorMemento(const Jogador& j) : PersonagemMemento(j),
                    movendoEsquerdaMemento(j.movendoEsquerda), movendoDireitaMemento(j.movendoDireita),
                    pulandoMemento(j.pulando), atacandoMemento(j.atacando), correndoMemento(j.correndo),
                    usandoHabilidadeMemento(j.usandoHabilidade), pontosMemento(j.pontos),
                    abatesMemento(j.abates), idJogadorMemento(j.idJogador), fundoVidaMemento(j.fundoVida),
                    barraVidaMemento(j.barraVida), frameWidthOciosoMemento(j.frameWidthOcioso),
                    frameHeightOciosoMemento(j.frameHeightOcioso), frameWidthMovimentoMemento(j.frameWidthMovimento),
                    frameHeightMovimentoMemento(j.frameHeightMovimento), frameWidthPuloMemento(j.frameWidthPulo),
                    frameHeightPuloMemento(j.frameHeightPulo), frameWidthAtaqueMemento(j.frameWidthAtaque),
                    frameHeightAtaqueMemento(j.frameHeightAtaque) {}

                    ~JogadorMemento() {}
                    friend class Jogador;
                    void gravarNoArquivo(std::ofstream& arquivo, int fase) const  {
                        arquivo << fase << " " << nomeMemento << " " << pontosMemento;
                    }

                    std::string getTipo() const {
                        return "Jogador";
                    }
            };
        public:
            enum Campeao {
                NAAFIRI, JHIN, LUX, EVELYNN, GWEN, PYKE, SETT, SHACO, VIEGO
            };
            Jogador(sf::Vector2f posicao = sf::Vector2f(20.0f, 40.0f), sf::Color corBarra = sf::Color(0, 255, 0));
            ~Jogador();

            Gerenciadores::Memento* salvarMemento() const;
            void restaurarMemento(const Gerenciadores::Memento* memento);

            void aoApertarTecla(const Gerenciadores::Tecla& evento);

            void executar();
            void salvar();
            void mover();
            void setCampeao(Campeao c);

            sf::Texture* carregarTextura(const std::string &caminho);

            void setIdJogador(const int id) { idJogador = id; }
            int getIdJogador() const { return idJogador; }

            float getPontos() const { return pontos; }
            void setPontos(float p) { pontos = p; }

            void setAbates(int a) { abates = a; }

            void mudarPontos(float valor);
            void registrarAbate();

            // void aceitar(VisitorColisao *visitor);
            void desenharBarra();
            void atualizarBarra();
            void atualizarAnimacao();

    };
} // Personagens

#endif //JOGO_JOGADOR_H