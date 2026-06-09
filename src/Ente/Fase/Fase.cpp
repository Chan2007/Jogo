//
// Created by Henrique on 09/05/2026.
//

#include "Fase.h"

#include <iostream>

#include "Listas/ListaEntidades.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Facil/Inimigo_Facil.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"


namespace Fases {
    Fase::Fase() :
      gerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade::getGerenciador()),
      gerenciadorColisao(Gerenciadores::Gerenciador_Colisao::getGerenciador()),
      audio(Gerenciadores::Gerenciador_Audio::getGerenciador())
    {
        desktop = sf::VideoMode::getDesktopMode();
        criarInimFaceis();
        criarPlataformas();
    }

    void Fase::criarPlataformas() {
        Obstaculos::Plataforma* chao = new Obstaculos::Plataforma(Obstaculos::Plataforma::CHAO);
        if (chao) {
            chao->setPosicao(sf::Vector2f(static_cast<float>(desktop.width)/2.f, static_cast<float>(desktop.height) - (chao->getTamanho().height)/2.f));

            LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(chao));
            gerenciadorColisao.incluirEntidade(chao);
        }
        else {std::cerr << "Falha ao criar chão" << std::endl;}

        Obstaculos::Plataforma* novaPlat;
        Ente::sementear();
        const int fator = rand() % 8 + 3;
        for (int i = 0; i < fator; i++) {
            if (rand() % 10 <5)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL1);
            else if ((rand()%10)<4)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL2);
            else
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL3);

            if (!novaPlat) {std::cerr << "Falha ao criar nova plataforma." << std::endl;}

            bool posicaoValida = false;
            int tentativas = 0;
            sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();
            while (!posicaoValida && tentativas < 500) {
                const int sizex = janela.getSize().x - novaPlat->getTamanho().width;
                const int sizey = janela.getSize().y - novaPlat->getTamanho().height - chao->getTamanho().height / 2;
                novaPlat->getSprite().setPosition(rand() % sizex + novaPlat->getTamanho().width / 2, rand() % sizey + novaPlat->getTamanho().height / 2);

                sf::FloatRect hitboxExpandida = novaPlat->getSprite().getGlobalBounds();
                hitboxExpandida.left -= 20.f;
                hitboxExpandida.top -= 20.f;
                hitboxExpandida.width += 40.f;
                hitboxExpandida.height += 40.f;
                if (gerenciadorColisao.verificarPosicaoLivre(hitboxExpandida))
                    posicaoValida = true;

                tentativas++;
            }
            if (posicaoValida) {
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(novaPlat));
                gerenciadorColisao.incluirEntidade(novaPlat);
            }
            else delete novaPlat;
        }
        novaPlat = NULL;
    }

    Fase::~Fase() {
        // Esvazia e deleta os ponteiros de entidades acumulados na fase
        LEntidades.limparLista();
        // Limpa as referências de ponteiros nos gerenciadores para o próximo estado
        gerenciadorColisao.limpar();
        gerenciadorGravidade.limpar();
    }

    bool Fase::trocarMusica(const int fase) const {
        if (diretorio_Audio.empty()) {
            std::cerr << "Sem música disponível! " << std::endl;
            return false;
        }
        audio.stop();
        std::string nomeArquivo = "";
        if (fase == 1)
            nomeArquivo = "Aurora_s-Theme.ogg";
        else if (fase == 2)
            nomeArquivo = "Down-to-a-Dusty-Plain.ogg";

        const std::string caminho_musica = Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, nomeArquivo);

        // Carrega e configura através do gerenciador de áudio
        audio.loadMusic(caminho_musica);

        // O próprio méthodo play() do Gerenciador já deve checar internamente se a música está ligada
        audio.play();
        return true;
    }
    void Fase::criarInimFaceis(){
        Inimigo_Facil* minion = NULL;
        Ente::sementear();
        const int fator = rand() % 8 + 3;
        sf::RenderWindow& janela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getJanela();
        for (int i = 0; i < fator; i++) {
            minion = new Inimigo_Facil();
            if (minion) {
                minion->setPosicao(sf::Vector2f(720.f, 560.f));
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade*>(minion));
                gerenciadorColisao.incluirEntidade(minion);
            }
        }
        minion = NULL;
    }
} // Fases