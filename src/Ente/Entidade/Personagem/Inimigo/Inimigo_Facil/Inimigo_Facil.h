//
// Created by Henrique on 05/05/2026.
//

#ifndef JOGO_INIMIGO_FACIL_H
#define JOGO_INIMIGO_FACIL_H
#include "Ente/Entidade/Personagem/Inimigo/Inimigo.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"

namespace Personagens {
    class Jogador;
}

class Inimigo_Facil : public Personagens::Inimigo {
    private:
        float raio;
        int tamanho;
        short int forca;
    public:
        Inimigo_Facil();
        ~Inimigo_Facil();
        void danificar(Personagens::Jogador* J);
        void atualizar(Personagens::Jogador* jogador);
        void executar();
        void salvar();
        float getRaio(){return raio;}
        int getTamanho(){return tamanho;};
        short int getForca(){return forca;}
};


#endif //JOGO_INIMIGO_FACIL_H
