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

Jogo* Jogo::jogo = NULL;
bool Jogo::inicializado = false;

Jogo::Jogo() : faseAtual(NULL),
               zelador(new Caretaker()),
               telaPausa(new Pausa()),
               jogador1(),
               jogador2(),
               gerenciadorTextura(),
               gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
               gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
               gerenciadorEstado(Gerenciadores::Gerenciador_Estado::getGerenciador()),
               multiplayer(false),
               musicaLigada(true),
               carregandoSave(false),
               estadoTela(TelaMenu),
               telaAnterior(TelaMenu)
{
    gerenciadorEstado.setJogo(this);
    jogador1 = new Personagens::Jogador(sf::Vector2f(50.0, 50.0), sf::Color::Green);
    jogador2 = new Personagens::Jogador(sf::Vector2f(
            desktop.width - 800.0, 50), sf::Color::Blue);

}

Jogo::~Jogo() {
    if (gerenciadorGrafico.isOpen()) {
        gerenciadorGrafico.close();
    }
    if (faseAtual) {
        delete faseAtual;
        faseAtual = NULL;
    }
    if (zelador) {
        delete zelador;
        zelador = NULL;
    }
    if (telaPausa) {
        delete telaPausa;
        telaPausa = NULL;
    }
    if (jogador1) {
        delete jogador1;
        jogador1 = NULL;
    }
    if (jogador2) {
        delete jogador2;
        jogador2 = NULL;
    }
    inicializado = false;
}

void Jogo::inicializar() {
    if (inicializado && gerenciadorGrafico.isOpen()) return;

    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    gerenciadorGrafico.setSize(videoMode);

    gerenciadorGrafico.create(videoMode, "Jogo League++", sf::Style::Default);
    gerenciadorGrafico.setFramerateLimit(60);

    inicializado = true;
    relogio.restart();
}
void Jogo::mudarEstado(const EstadoTela novoEstado) {

    switch (novoEstado) {
        case TelaFase1:
            if (estadoTela == TelaPausa)
                estadoTela = novoEstado;
            else {
                if (faseAtual) {
                    delete faseAtual;
                    faseAtual = NULL;
                }
                inicializar();
                if (!carregandoSave) { conferirJogadores(); }
                faseAtual = new Fases::Primeira_Fase();
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
                inicializar();
                if (!carregandoSave) { conferirJogadores(); }
                faseAtual = new Fases::Segunda_Fase();
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
            if (jogador1) {
                delete jogador1;
                jogador1 = NULL;
            }
            if (jogador2) {
                delete jogador2;
                jogador2 = NULL;
            }
            estadoTela = TelaMenu;
            gerenciadorGrafico.close();
            break;

        case TelaPausa:
            // Salva qual fase o jogador estava jogando (TelaFase1 ou TelaFase2)
            if (estadoTela == TelaFase1 || estadoTela == TelaFase2 || estadoTela == TelaMenu)
                telaAnterior = estadoTela;

            if (!telaPausa)
                telaPausa = new Pausa();

            estadoTela = TelaPausa;
            break;

        default:
            break;
    }
}
void Jogo::executar() {
    while (gerenciadorGrafico.isOpen()) {

        sf::Event evento;
        bool estadoMudouNesteFrame = false;

        while (gerenciadorGrafico.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) gerenciadorGrafico.close();

            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::F5) {
                    salvarJogoAtual("save.txt");
                    continue;
                }
                if (evento.key.code == sf::Keyboard::F9) {
                    carregarJogo("save.txt");
                    continue;
                }
            }

            if (estadoTela == TelaPausa && telaPausa) {
                if (evento.type == sf::Event::KeyPressed) {
                    if (evento.key.code == sf::Keyboard::Up) {
                        telaPausa->atualizarItemSelecionado(-1);
                    }
                    else if (evento.key.code == sf::Keyboard::Down) {
                        telaPausa->atualizarItemSelecionado(1);
                    }
                    else if (evento.key.code == sf::Keyboard::Enter) {
                        processarCliqueBotaoPausa(telaPausa->getIndiceSelecionado());
                        estadoMudouNesteFrame = true;
                        break;
                    }
                }
                if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                    int botaoClicado = telaPausa->cliqueMouse();
                    if (botaoClicado != -1) {
                        processarCliqueBotaoPausa(botaoClicado);
                        estadoMudouNesteFrame = true;
                        break;
                    }
                }
            }
            else if (faseAtual && (estadoTela == TelaFase1 || estadoTela == TelaFase2)) {
                faseAtual->processarEventos(evento);
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
    if (!faseAtual) {
        std::cerr << "Nenhuma fase atual para salvar." << std::endl;
        return false;
    }

    int numeroFase = 0;

    if (estadoTela == TelaFase1) {
        numeroFase = 1;
    }
    else if (estadoTela == TelaFase2) {
        numeroFase = 2;
    }
    else if (estadoTela == TelaPausa) {
        numeroFase = (telaAnterior == TelaFase1) ? 1 : (telaAnterior == TelaFase2) ? 2 : 0;
    }
    else {
        std::cerr << "Estado atual nao e uma fase." << std::endl;
        return false;
    }

    // Cria os mementos usando as funções de JogadorMemento
    if (jogador1)
        zelador->addMemento(jogador1->salvarMemento());

    if (multiplayer && jogador2)
        zelador->addMemento(jogador2->salvarMemento());

    zelador->salvarEmArquivoTXT("ranking.txt", numeroFase);
    return faseAtual->salvarJogo(caminho, numeroFase);
}

void Jogo::conferirJogadores() {
    if (jogador1 == NULL) {
        jogador1 = new Personagens::Jogador(sf::Vector2f(50.0f, 50.0f), sf::Color::Green);
    }

    if (jogador2 == NULL) {
        jogador2 = new Personagens::Jogador(sf::Vector2f(desktop.width - 800.0f, 50.0f), sf::Color::Blue);
    }
}
void Jogo::solicitarPausa() {
    if (estadoTela == TelaFase1 || estadoTela == TelaFase2) {
        mudarEstado(TelaPausa);

        if (telaPausa != NULL)
            telaPausa->ajustarPosicoes();
    }
    else if (estadoTela == TelaPausa) {
        mudarEstado(telaAnterior);
    }
}
void Jogo::processarCliqueBotaoPausa(int indice) {
    switch (indice) {
        case 0: // Retomar Jogo
            mudarEstado(telaAnterior);
            break;

        case 1: // Salvar Jogo
            if (salvarJogoAtual("save.txt"))
                std::cout << "Jogo Salvo com Sucesso a partir da Pausa!" << std::endl;
            else
                std::cerr << "Falha ao salvar o jogo." << std::endl;
            break;
        case 2: // Voltar para o Menu
            mudarEstado(TelaMenu);
            break;
        default:
            break;
    }
}