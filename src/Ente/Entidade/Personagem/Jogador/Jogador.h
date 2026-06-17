
#ifndef JOGO_JOGADOR_H
#define JOGO_JOGADOR_H

#include "Sistema/Input/Observador_Input.h"
#include "Ente/Entidade/Personagem/Personagem.h"

namespace Gerenciadores {
    class Observador_Input;
    class Gerenciador_Gravidade;
}

namespace Obstaculos {
    class Plataforma;
}
 
namespace Personagens {
    class Inimigo;

    enum EscolhaCampeao {
        CAMPEAO_NAAFIRI,
        CAMPEAO_JHIN,
        CAMPEAO_LUX,
        CAMPEAO_EVELYNN,
        CAMPEAO_GWEN,
        CAMPEAO_PYKE,
        CAMPEAO_SETT,
        CAMPEAO_SHACO,
        CAMPEAO_VIEGO
    };

    class Jogador : public Personagem, public Gerenciadores::Observador_Input {
    private:
        bool movendoEsquerda;
        bool movendoDireita;
        bool pulando;
        bool atacando;
        bool usandoHabilidade;
    protected:
        float pontos;
        int abates;
        int idJogador;
        sf::RectangleShape fundoVida;
        sf::RectangleShape barraVida;
    public:
        Jogador(sf::Vector2f posicao = sf::Vector2f(20.0f, 40.0f),
        sf::Color corBarra = sf::Color(0, 255, 0));
        ~Jogador();

        void aoApertarTecla(const Gerenciadores::Tecla& evento);

        void executar();
        void salvar();
        void mover();
        void setCampeao(EscolhaCampeao campeao);
        void setIdJogador(const int id) { idJogador = id; }

        int getIdJogador() const { return idJogador; }
        float getPontos() const { return pontos; }
        int getAbates() const { return abates; }
        void adicionarPontos(float valor);
        void registrarAbate();

        void aceitar(VisitorColisao *visitor);

    };
} // Personagens

#endif //JOGO_JOGADOR_H