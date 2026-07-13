#include "jogo.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Fase/Primeira_Fase/Primeira_Fase.h"
#include "Ente/Fase/Segunda_Fase/Segunda_Fase.h"
#include "Ente/Menu/Pausa.h"
#include "Gerenciador/Gerenciador_Estado/Caretaker.h"
#include "Gerenciador/Gerenciador_Estado/Gerenciador_Estado.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"

bool Jogo::inicializado = false;

Jogo::Jogo() : faseAtual(NULL), telaPausa(new Pausa()), gerenciadorTextura(),
               gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
               gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
               gerenciadorEstado(Gerenciadores::Gerenciador_Estado::getGerenciador()),
               gerenciadorInput(Gerenciadores::Gerenciador_Input::getGerenciador()),
               musicaLigada(true), carregandoSave(false), estadoTela(TelaMenu), telaAnterior(TelaMenu)
{
    gerenciadorEstado.setJogo(this);
}

Jogo::~Jogo() {
    if (gerenciadorGrafico.isOpen())
        gerenciadorGrafico.close();
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }
    if (telaPausa) {
        delete telaPausa;
        telaPausa = NULL;
    }
    inicializado = false;
}

void Jogo::inicializar() {
    if (estaAberto()) return;

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    gerenciadorGrafico.setSize(videoMode);

    gerenciadorGrafico.create(videoMode, "League++", sf::Style::Default);
    gerenciadorGrafico.setFramerateLimit(60);

    inicializado = true;
}
void Jogo::mudarEstado(EstadoTela novoEstado, const std::string& nomeJ1, const QString& campeaoJ1,
                       const std::string& nomeJ2, const QString& campeaoJ2, bool multiplayer)
{
    switch (novoEstado) {
        case TelaFase1:
            if (estadoTela == TelaPausa)
                estadoTela = novoEstado;
            else {
                if (faseAtual) {
                    delete faseAtual;
                    faseAtual = NULL;
                }
                faseAtual = new Fases::Primeira_Fase(this, nomeJ1, campeaoJ1, nomeJ2, campeaoJ2, multiplayer);
                estadoTela = novoEstado;
            }
            break;

        case TelaFase2:
            if (estadoTela == TelaPausa)
                estadoTela = novoEstado;
            else {
                if (faseAtual) {
                    delete faseAtual;
                    faseAtual = NULL;
                }
                faseAtual = new Fases::Segunda_Fase(this, nomeJ1, campeaoJ1, nomeJ2, campeaoJ2, multiplayer);
                estadoTela = novoEstado;
            }
            break;

        case TelaMenu:
            if (faseAtual) {
                delete faseAtual;
                faseAtual = NULL;
            }
            if (telaPausa) {
                delete telaPausa;
                telaPausa = NULL;
            }
            estadoTela = TelaMenu;
            gerenciadorGrafico.close();
            break;

        case TelaPausa:
            if (estadoTela == TelaFase1 || estadoTela == TelaFase2 || estadoTela == TelaMenu)
                telaAnterior = estadoTela;

            if (!telaPausa) telaPausa = new Pausa();
            estadoTela = TelaPausa;
            break;

        default:
            break;
    }
}
void Jogo::executar() {
    gerenciadorInput.inscrever(this);
    while (gerenciadorGrafico.isOpen()) {

        sf::Event evento;
        bool estadoMudouNesteFrame = false;

        while (gerenciadorGrafico.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                gerenciadorGrafico.close();
                break;
            }

            EstadoTela estadoAntes = estadoTela;
            gerenciadorInput.notificarObservadores(evento);

            if (estadoTela != estadoAntes) {
                estadoMudouNesteFrame = true;
                break;
            }

            if (estadoTela == TelaPausa && telaPausa && evento.type == sf::Event::MouseButtonPressed &&
                evento.mouseButton.button == sf::Mouse::Left)
            {
                int clicado = telaPausa->clicado();
                if (clicado != -1) {
                    onClickPausa(clicado);
                    estadoMudouNesteFrame = true;
                    break;
                }
            }
        }

        // SE O ESTADO MUDOU, INTERROMPE O FRAME AQUI E RECOMEÇA O LOOP PRINCIPAL
        if (estadoMudouNesteFrame) continue;

        // Só executa a física da fase se ela existir E a tela atual for de jogo
        if (estadoTela == TelaFase1 || estadoTela == TelaFase2) {
            if (faseAtual) faseAtual->executar();
        }

        // Renderização
        gerenciadorGrafico.clear();

        if (faseAtual && (estadoTela == TelaFase1 || estadoTela == TelaFase2)) {
            if (faseAtual) faseAtual->desenhar();
        }
        if (estadoTela == TelaPausa && telaPausa) {
            if (telaPausa) telaPausa->desenhar();
        }
        if (estadoTela == TelaMenu) {
            break;
        }

        gerenciadorGrafico.show();
    }

    gerenciadorAudio.stop();
    gerenciadorInput.desinscrever(this);
    inicializado = false;
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }
}
void Jogo::setMusica(const bool ligada) {
    // Altera o estado diretamente no Gerenciador Global
    gerenciadorAudio.ativarMusica(ligada);
    musicaLigada = ligada;

    if (inicializado && faseAtual) {
        faseAtual->setMusica(ligada);
    }
}
void Jogo::setVolume(const float volume) const {
    gerenciadorAudio.setVolume(volume);

    if (inicializado && faseAtual)
        faseAtual->setVolume(volume);
}

bool Jogo::carregarJogo(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str());

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir save: " << caminho << std::endl;
        return false;
    }

    std::string cabecalho;
    std::getline(arquivo, cabecalho);

    if (cabecalho != "SAVE_PRIMEIRO_JOGO_V1") {
        std::cerr << "Arquivo de save invalido." << std::endl;
        arquivo.close();
        return false;
    }

    std::string linhaFase;
    std::getline(arquivo, linhaFase);

    std::istringstream iss(linhaFase);

    std::string palavraFase;
    int numeroFase = 0;

    iss >> palavraFase >> numeroFase;

    arquivo.close();

    if (palavraFase != "FASE") {
        std::cerr << "Linha de fase invalida no save." << std::endl;
        return false;
    }

    EstadoTela estadoDoSave;

    if (numeroFase == 1) {
        estadoDoSave = TelaFase1;
    }
    else if (numeroFase == 2) {
        estadoDoSave = TelaFase2;
    }
    else {
        std::cerr << "Numero de fase invalido: " << numeroFase << std::endl;
        return false;
    }
    setCarregandoSave(true);

    if (faseAtual == NULL || estadoTela != estadoDoSave) {
        mudarEstado(estadoDoSave);
    }

    if (!faseAtual) {
        std::cerr << "Erro: faseAtual nao foi criada." << std::endl;
        setCarregandoSave(false);
        return false;
    }

    bool resultado = faseAtual->carregarJogo(caminho);
    setCarregandoSave(false);

    return resultado;
}

bool Jogo::salvarJogoAtual(const std::string& caminho) {
    if (!faseAtual) return false;
    int numeroFase = (estadoTela == TelaFase1) ? 1 : 2;
    return faseAtual->salvarJogo(caminho, numeroFase);
}
void Jogo::gerenciarPausa() {
    if (estadoTela == TelaFase1 || estadoTela == TelaFase2) {
        mudarEstado(TelaPausa);
        if (telaPausa != NULL) {
            telaPausa->redimensionarTela();
            gerenciadorInput.inscrever(telaPausa);
        }
    }
    else if (estadoTela == TelaPausa) {
        gerenciadorInput.desinscrever(telaPausa);
        mudarEstado(telaAnterior);
    }
}
void Jogo::onClickPausa(int indice) {
    switch (indice) {
        case 0:
            gerenciarPausa();
            break;
        case 1:
            if (salvarJogoAtual("save.txt") )
                std::cout << "Jogo salvo com sucesso!" << std::endl;
            else
                std::cout << "Erro ao salvar o jogo" << std::endl;
            break;
        case 2:
            mudarEstado(TelaMenu);
            break;
        default:
            break;
    }
}
void Jogo::aoApertarTecla(const Gerenciadores::Tecla& evento) {
    if (!evento.pressionada) return;

    if (evento.acao == "salvar")
        salvarJogoAtual("save.txt");
    else if (evento.acao == "carregar")
        carregarJogo("save.txt");
    else if (evento.acao == "pausar")
        gerenciarPausa();
    else if (evento.acao == "pausa_confirmar" && estadoTela == TelaPausa && telaPausa)
        onClickPausa(telaPausa->getIndex());
}
