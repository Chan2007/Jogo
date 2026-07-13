#include "Fase.h"

#include <iostream>
#include <fstream>
#include <qmap.h>
#include <sstream>
#include <string>

#include "jogo.h"
#include "Listas/ListaEntidades.h"
#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Personagem.h"
#include "Ente/Entidade/Obstaculo/Plataforma/Plataforma.h"
#include "Ente/Entidade/Obstaculo/Obstaculo_Medio/Portal.h"
#include "Ente/Entidade/Obstaculo/Obstaculo_Dificil/Pinstouro.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Medio/Azulo.h"
#include "Ente/Entidade/Personagem/Inimigo/Inimigo_Facil/Minion.h"
#include "Ente/Entidade/Personagem/Inimigo/Chefe/DragaoAnciao.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Ente/Entidade/Projetil/Projetil.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Estado/Caretaker.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Grafico.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"
#include "Gerenciador/Gerenciador_Input/Gerenciador_Input.h"
#include "Sistema/Caminho/Encontrar_Caminho.h"


namespace Fases {
    Fase::Fase(Jogo* pJogo, const std::string& nomeJ1, const QString& campeaoJ1,
               const std::string& nomeJ2, const QString& campeaoJ2, bool jogador2Ativo) :
      Ente(), jogo(pJogo), LEntidades(), tamanhoJanela(sf::VideoMode::getDesktopMode()),
      diretorio_Audio(""), jogador1(NULL), jogador2(NULL),
      multiplayer(jogador2Ativo), CaretakerFase(new Gerenciadores::Caretaker()),
      gerenciadorGravidade(Gerenciadores::Gerenciador_Gravidade::getGerenciador()),
      gerenciadorColisao(&Gerenciadores::Gerenciador_Colisao::getGerenciador()),
      gerenciadorAudio(Gerenciadores::Gerenciador_Audio::getGerenciador()),
      gerenciadorInput(Gerenciadores::Gerenciador_Input::getGerenciador())
    {
        tamanhoJanela = Gerenciadores::Gerenciador_Grafico::getGerenciador().getSize();

        if (!jogo->getCarregandoSave()) {
            criarInimFaceis();
            criarPlataformas();
            criarJogadores(nomeJ1, campeaoJ1, nomeJ2, campeaoJ2, jogador2Ativo);
        }
    }

    void Fase::criarPlataformas()
    {
        Obstaculos::Plataforma *chao = new Obstaculos::Plataforma(Obstaculos::Plataforma::CHAO);
        if (chao) {
            chao->setPosicao(sf::Vector2f(
                static_cast<float>(tamanhoJanela.width),
                static_cast<float>(tamanhoJanela.height) - (chao->getTamanho().height) / 2.f));

            LEntidades.incluirEntidade(static_cast<Entidades::Entidade *>(chao));
            gerenciadorColisao->incluirEntidade(chao);
            gerenciadorGravidade.aplicarGravidade(chao, true);
        }
        else
            std::cerr << "Falha ao criar chão" << std::endl;

        Obstaculos::Plataforma *novaPlat;
        sementear();
        const int fator = rand() % 3 + 8;
        for (int i = 0; i < fator; i++) {
            double media = 3;
            double desvio_padrao = 2;
            if (std::fabs(gerar_num_norm(media, desvio_padrao) - media) < desvio_padrao)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL1);
            else if (std::fabs(gerar_num_norm(media, desvio_padrao) - media) > 1.5 * desvio_padrao)
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL2);
            else
                novaPlat = new Obstaculos::Plataforma(Obstaculos::Plataforma::NORMAL3);

            if (!novaPlat)
                std::cerr << "Falha ao criar nova plataforma." << std::endl;

            bool posicaoValida = false;
            int tentativas = 0;
            while (!posicaoValida && tentativas < 500) {
                const int sizex = tamanhoJanela.width - novaPlat->getTamanho().width;
                const int sizey = tamanhoJanela.height - novaPlat->getTamanho().height - chao->getTamanho().height /
                                  2 - 200;
                novaPlat->getSprite().setPosition(
                    static_cast<int>(gerar_num_binom(0, tamanhoJanela.width)) % sizex + novaPlat->getTamanho().width
                    / 2,
                    static_cast<int>(gerar_num_binom(0, tamanhoJanela.width)) % sizey + novaPlat->getTamanho().
                    height / 2 + 200
                );

                sf::FloatRect hitboxExpandida = novaPlat->getSprite().getGlobalBounds();
                hitboxExpandida.left -= 20.f;
                hitboxExpandida.top -= 20.f;
                hitboxExpandida.width += 40.f;
                hitboxExpandida.height += 40.f;
                if (gerenciadorColisao->verificarPosicaoLivre(hitboxExpandida))
                    posicaoValida = true;
                tentativas++;
            }
            if (posicaoValida) {
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade *>(novaPlat));
                gerenciadorColisao->incluirEntidade(novaPlat);
                gerenciadorGravidade.aplicarGravidade(novaPlat, true);
            } else delete novaPlat;
        }
        novaPlat = NULL;
    }

    Fase::~Fase() {
        gerenciadorInput.desinscrever(jogador1);
        if (multiplayer && jogador2) gerenciadorInput.desinscrever(jogador2);

        if (gerenciadorColisao) gerenciadorColisao->limpar();
        gerenciadorGravidade.limpar();

        if (jogador1) LEntidades.removerEntidade(static_cast<Entidades::Entidade *>(jogador1));
        if (jogador2) LEntidades.removerEntidade(static_cast<Entidades::Entidade *>(jogador2));

        if (jogador1) { delete jogador1; jogador1 = NULL; }
        if (jogador2) { delete jogador2; jogador2 = NULL; }

        LEntidades.limparLista();

        if (CaretakerFase) {
            delete CaretakerFase;
            CaretakerFase = NULL;
        }
        if (jogo) jogo = NULL;
    }

    bool Fase::trocarMusica(const int fase) const
    {
        if (diretorio_Audio.empty()) {
            std::cerr << "Sem música disponível! " << std::endl;
            return false;
        }
        gerenciadorAudio.stop();
        std::string nomeArquivo = "";
        if (fase == 1)
            nomeArquivo = "Aurora_s-Theme.ogg";
        else if (fase == 2)
            nomeArquivo = "Down-to-a-Dusty-Plain.ogg";

        const std::string caminho_musica = Encontrar_Caminho::concatenarEnderecos(diretorio_Audio, nomeArquivo);

        // Carrega e configura através do gerenciador de áudio
        gerenciadorAudio.loadMusic(caminho_musica);

        // O próprio méthodo play() do Gerenciador já deve checar internamente se a música está ligada
        gerenciadorAudio.play();
        return true;
    }
    void Fase::criarInimFaceis()
    {
        Personagens::Minion *minion = NULL;
        sementear();
        const int fator = rand() % 8 + 3;
        for (int i = 1; i <= fator; i++) {
            minion = new Personagens::Minion();
            if (minion) {
                minion->setPosicao(sf::Vector2f(150 * i, rand() % tamanhoJanela.height));
                gerenciadorColisao->incluirEntidade(minion);
                gerenciadorGravidade.aplicarGravidade(minion, true);
                LEntidades.incluirEntidade(static_cast<Entidades::Entidade *>(minion));
            }
        }
        minion = NULL;
    }
    bool Fase::verificarLimitesJanela(Entidades::Entidade *entidade)
    {
        if (!entidade) return false;

        const sf::Vector2f posicaoAtual = entidade->getPosicao();
        const sf::FloatRect tamanho = entidade->getTamanho();

        float limiteLargura = static_cast<float>(tamanhoJanela.width);
        float limiteAltura = static_cast<float>(tamanhoJanela.height);

        sf::Vector2f novaPosicao = posicaoAtual;
        bool colidiuBorda = false;

        float metadeLargura = tamanho.width / 2.0f;
        float metadeAltura = tamanho.height / 2.0f;

        // BORDA ESQUERDA/DIREITA
        if (novaPosicao.x - metadeLargura < 0.0f) {
            novaPosicao.x = metadeLargura;
            colidiuBorda = true;
        } else if (novaPosicao.x + metadeLargura > limiteLargura) {
            novaPosicao.x = limiteLargura - metadeLargura;
            colidiuBorda = true;
        }

        // BORDA SUPERIOR/INFERIOR
        if (novaPosicao.y - metadeAltura < 0.0f) {
            novaPosicao.y = metadeAltura;
            colidiuBorda = true;
        } else if (novaPosicao.y + metadeAltura > limiteAltura) {
            novaPosicao.y = limiteAltura - metadeAltura;
            colidiuBorda = true;
            gerenciadorGravidade.aoTocarChao(entidade, sf::Vector2f(0.0f, -1.0f));
        }

        if (colidiuBorda) {
            entidade->setPosicao(novaPosicao);
            entidade->setColisao(true);
        }
        return colidiuBorda;
    }
    void Fase::definirLimitesJanela()
    {
        Lista<Entidades::Entidade>::IteratorLista it = LEntidades.getPrimeiro();
        Lista<Entidades::Entidade>::IteratorLista fim(NULL);
        while (it != fim) {
            verificarLimitesJanela(&(*it));
            ++it;
        }
    }
    const QStringList Fase::CAMPEOES = {
        "NAAFIRI", "JHIN", "LUX", "EVELYNN",
        "GWEN", "PYKE", "SETT", "SHACO", "VIEGO"
    };

    Personagens::Jogador::Campeao Fase::defCampeao(const QString &texto)
    {
        static QMap<QString, Personagens::Jogador::Campeao> tabela;
        if (tabela.isEmpty()) {
            tabela.insert("NAAFIRI", Personagens::Jogador::NAAFIRI);
            tabela.insert("JHIN", Personagens::Jogador::JHIN);
            tabela.insert("LUX", Personagens::Jogador::LUX);
            tabela.insert("EVELYNN", Personagens::Jogador::EVELYNN);
            tabela.insert("GWEN", Personagens::Jogador::GWEN);
            tabela.insert("PYKE", Personagens::Jogador::PYKE);
            tabela.insert("SETT", Personagens::Jogador::SETT);
            tabela.insert("SHACO", Personagens::Jogador::SHACO);
            tabela.insert("VIEGO", Personagens::Jogador::VIEGO);
        }

        QString chave = texto.toUpper().trimmed();
        return tabela.value(chave, Personagens::Jogador::NAAFIRI);
        // return tabela.value(chave, random());
    }

    Personagens::Jogador::Campeao Fase::randomCampeao()
    {
        static bool inicializado = false;
        if (!inicializado) {
            srand(static_cast<unsigned int>(time(NULL)));
            inicializado = true;
        }

        Personagens::Jogador::Campeao campeoes[] = {
            Personagens::Jogador::NAAFIRI, Personagens::Jogador::JHIN,
            Personagens::Jogador::LUX, Personagens::Jogador::EVELYNN,
            Personagens::Jogador::GWEN, Personagens::Jogador::PYKE,
            Personagens::Jogador::SETT, Personagens::Jogador::SHACO,
            Personagens::Jogador::VIEGO
        };

        return campeoes[rand() % (sizeof(campeoes) / sizeof(campeoes[0]))];
    }

    void Fase::criarJogadores(const std::string &nomeJ1, const QString &campeaoJ1,
                          const std::string &nomeJ2, const QString &campeaoJ2, bool m)
    {
        multiplayer = m;

        std::string nome1 = nomeJ1.empty() ? "Anonimo" : nomeJ1;
        QString textCamp1 = campeaoJ1.isEmpty() ? CAMPEOES.first() : campeaoJ1;

        jogador1 = new Personagens::Jogador(sf::Vector2f(50.0f, 50.0f), sf::Color::Green);
        jogador1->setIdJogador(1);
        jogador1->setNome(nome1);
        jogador1->setCampeao(defCampeao(textCamp1));
        registrarJogador(jogador1);

        if (multiplayer) {
            std::string nome2 = nomeJ2.empty() ? "Anonimo2" : nomeJ2;
            QString textCamp2 = campeaoJ2.isEmpty() ? CAMPEOES.first() : campeaoJ2;

            jogador2 = new Personagens::Jogador(
                sf::Vector2f(tamanhoJanela.width - 800.0f, 50.0f),
                sf::Color::Blue);
            jogador2->setIdJogador(2);
            jogador2->setNome(nome2);
            jogador2->setCampeao(defCampeao(textCamp2));
            registrarJogador(jogador2);
        }
    }

    bool Fase::salvarJogo(const std::string &caminho, int numeroFase)
    {
        std::ofstream arquivo(caminho.c_str());

        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir arquivo para salvar: " << caminho << std::endl;
            return false;
        }

        arquivo << "SAVE_PRIMEIRO_JOGO_V1" << '\n';
        arquivo << "FASE " << numeroFase << '\n';

        LEntidades.salvarTodas(arquivo);

        arquivo.close();

        std::cout << "Jogo salvo com sucesso!" << std::endl;
        return true;
    }

    void Fase::limparJogo()
    {
        gerenciadorGravidade.limpar();

        if (gerenciadorColisao) {
            gerenciadorColisao->limpar();
        }

        if (jogador1 != NULL) {
            gerenciadorInput.desinscrever(jogador1);
            LEntidades.removerEntidade(static_cast<Entidades::Entidade *>(jogador1));
        }

        if (jogador2 != NULL && multiplayer) {
            gerenciadorInput.desinscrever(jogador2);
            LEntidades.removerEntidade(static_cast<Entidades::Entidade *>(jogador2));
        }

        LEntidades.limparLista();

        // jogador1 = NULL;
        // jogador2 = NULL

        // Limpa as referências estáticas que os inimigos tinham dos jogadores
        Personagens::Inimigo::limparJogadores();
    }

    void Fase::registrarEntidade(Entidades::Entidade *e)
    {
        if (e == NULL) return;

        LEntidades.incluirEntidade(e);
        gerenciadorColisao->incluirEntidade(e);
        gerenciadorGravidade.aplicarGravidade(e, true);
    }

    void Fase::registrarJogador(Personagens::Jogador *j)
    {
        if (!j) return;

        if (j->getIdJogador() == 1) {
            jogador1 = j;
        } else if (j->getIdJogador() == 2) {
            jogador2 = j;
            multiplayer = true;
        }

        gerenciadorInput.inscrever(j);
        gerenciadorColisao->incluirEntidade(j);
        gerenciadorGravidade.aplicarGravidade(j, true);
        LEntidades.incluirEntidade(static_cast<Entidades::Entidade *>(j));

        Personagens::Inimigo::incluirJogador(j);
    }

    bool Fase::lerDadosEntidade(std::istream &entrada, Entidades::Entidade *e)
    {
        if (!e) return false;

        std::string nome;
        float x;
        float y;
        bool colisao;
        bool vigente;

        entrada >> nome >> x >> y >> colisao >> vigente;

        if (entrada.fail()) return false;

        e->setNome(nome);
        e->setPosicao(sf::Vector2f(x, y));
        e->setColisao(colisao != 0);
        e->setVigente(vigente != 0);

        return true;
    }

    bool Fase::lerDadosPersonagem(std::istream &entrada, Personagens::Personagem *p)
    {
        if (!p) return false;

        float vx;
        float vy;
        float vida;
        float vidaMaxima;
        int estado;
        bool invulneravel;

        entrada >> vx >> vy >> vida >> vidaMaxima >> estado >> invulneravel;

        if (entrada.fail()) return false;

        p->setVelocidade(sf::Vector2f(vx, vy));
        p->setVidaMaxima(static_cast<int>(vidaMaxima));
        p->setVida(static_cast<int>(vida));
        p->setEstado(static_cast<Personagens::Estado>(estado));
        p->setInvulneravel(invulneravel != 0);

        return true;
    }

    bool Fase::lerDadosInimigo(std::istream &entrada, Personagens::Inimigo *i)
    {
        if (!i) return false;

        float poder;
        float tempoUltimoAtaque;
        float direcaoPatrulha;
        float deslocamentoPatrulha;
        bool elite;
        bool interagindo;

        entrada >> poder >> tempoUltimoAtaque >> direcaoPatrulha >> deslocamentoPatrulha >> elite >> interagindo;

        if (entrada.fail())
            return false;

        i->setPoder(static_cast<int>(poder));
        i->setTempoUltimoAtaque(tempoUltimoAtaque);
        i->setDirecaoPatrulha(direcaoPatrulha);
        i->setDeslocamentoPatrulha(deslocamentoPatrulha);
        i->setElite(elite != 0);
        i->setInteragindo(interagindo != 0);

        return true;
    }

    bool Fase::carregarJogo(const std::string &caminho)
    {
        std::ifstream arquivo(caminho.c_str());

        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir arquivo de save: " << caminho << std::endl;
            return false;
        }

        std::string linha;

        std::getline(arquivo, linha);

        if (linha != "SAVE_PRIMEIRO_JOGO_V1") {
            std::cerr << "Arquivo de save invalido." << std::endl;
            arquivo.close();
            return false;
        }

        std::getline(arquivo, linha);

        limparJogo();

        bool carregouTudo = true;
        while (std::getline(arquivo, linha)) {
            if (linha.empty())
                continue;

            if (!carregarLinhaEntidade(linha)) {
                std::cerr << "Erro ao carregar linha: " << linha << std::endl;
                carregouTudo = false;
            }
        }
        if (jogador1 == NULL) {
            std::cerr << "Erro: Jogador 1 nao foi carregado do save." << std::endl;
            carregouTudo = false;
        }
        arquivo.close();

        if (carregouTudo) { std::cout << "Jogo carregado com sucesso." << std::endl; } else {
            std::cout << "Carregou com erros." << std::endl;
        }

        return carregouTudo;
    }

    bool Fase::carregarLinhaEntidade(const std::string &linha)
    {
        std::istringstream entrada(linha);

        std::string tipo;
        entrada >> tipo;

        if (tipo == "JOGADOR" || tipo == "NAAFIRI") {
            Personagens::Jogador *jogador = new Personagens::Jogador();
            if (!jogador) return false;

            jogador->setCampeao(Personagens::Jogador::NAAFIRI);

            if (!lerDadosEntidade(entrada, jogador)) {
                delete jogador;
                return false;
            }
            if (!lerDadosPersonagem(entrada, jogador)) {
                delete jogador;
                return false;
            }

            float pontos; int abates; int idJogador;
            entrada >> pontos >> abates >> idJogador;
            if (entrada.fail()) {
                delete jogador;
                return false;
            }

            // Rejeita IDs inválidos ou slots já ocupados
            if (idJogador == 1 && jogador1 != NULL) {
                delete jogador;
                return false;
            }
            if (idJogador == 2 && jogador2 != NULL) {
                delete jogador;
                return false;
            }
            if (idJogador != 1 && idJogador != 2) {
                delete jogador;
                return false;
            }

            jogador->setIdJogador(idJogador);
            jogador->setPontos(pontos);
            jogador->setAbates(abates);
            jogador->atualizarBarra();

            registrarJogador(jogador); //  multiplayer = true se idJogador == 2

            std::cout << "Jogador carregado: " << jogador->getNome() << " ID: " << jogador->getIdJogador() << std::endl;

            return true;

        }
        else if (tipo == "INIMIGO_FACIL" || tipo == "MINION") {
            Personagens::Minion *minion = new Personagens::Minion();

            if (!lerDadosEntidade(entrada, minion)) {
                delete minion;
                return false;
            }

            if (!lerDadosPersonagem(entrada, minion)) {
                delete minion;
                return false;
            }

            if (!lerDadosInimigo(entrada, minion)) {
                delete minion;
                return false;
            }

            float raio;
            entrada >> raio;

            registrarEntidade(minion);
            return true;
        }
        else if (tipo == "INIMIGO_MEDIO" || tipo == "AZULO") {
            Personagens::Azulo *azulo = new Personagens::Azulo();

            if (!lerDadosEntidade(entrada, azulo)) {
                delete azulo;
                return false;
            }

            if (!lerDadosPersonagem(entrada, azulo)) {
                delete azulo;
                return false;
            }

            if (!lerDadosInimigo(entrada, azulo)) {
                delete azulo;
                return false;
            }

            int tamanho;
            entrada >> tamanho;

            registrarEntidade(azulo);
            return true;
        } else if (tipo == "DRAGAO_ANCIAO") {
            Personagens::DragaoAnciao *dragao = new Personagens::DragaoAnciao();

            if (!lerDadosEntidade(entrada, dragao)) {
                delete dragao;
                return false;
            }

            if (!lerDadosPersonagem(entrada, dragao)) {
                delete dragao;
                return false;
            }

            if (!lerDadosInimigo(entrada, dragao)) {
                delete dragao;
                return false;
            }

            short int forca;
            entrada >> forca;

            if (entrada.fail()) {
                delete dragao;
                return false;
            }

            registrarEntidade(dragao);

            Entidades::Projetil *p = new Entidades::Projetil();
            if (p) {
                p->setDoJogador(false);
                p->setVigente(false);

                dragao->setProjetil(p);

                registrarEntidade(p);
            }
            return true;
        } else if (tipo == "PROJETIL") {
            /*
            Entidades::Projetil* projetil = new Entidades::Projetil();

            if (!lerDadosEntidade(entrada, projetil)) {
                delete projetil;
                return false;
            }

            int ativo;
            float velX;
            float velY;
            int doJogador;

            entrada >> ativo >> velX >> velY >> doJogador;

            if (entrada.fail()) {
                delete projetil;
                return false;
            }

            projetil->setVelocidade(sf::Vector2f(velX, velY));
            projetil->setDoJogador(doJogador != 0);
            projetil->setAtivo(ativo != 0);

            registrarEntidade(projetil);
            */
            return true;
        } else if (tipo == "PLATAFORMA") {
            Obstaculos::Plataforma *plataforma = new Obstaculos::Plataforma();

            if (!lerDadosEntidade(entrada, plataforma)) {
                delete plataforma;
                return false;
            }

            int tipoPlataforma;

            entrada >> tipoPlataforma;

            if (entrada.fail()) {
                delete plataforma;
                return false;
            }

            plataforma->setTipo(static_cast<Obstaculos::Plataforma::TipoPlataforma>(tipoPlataforma));
            registrarEntidade(plataforma);
            return true;
        } else if (tipo == "PORTAL") {
            Obstaculos::Portal *portal = new Obstaculos::Portal();

            if (!lerDadosEntidade(entrada, portal)) {
                delete portal;
                return false;
            }

            registrarEntidade(portal);
            return true;
        } else if (tipo == "PINSTOURO") {
            Obstaculos::Pinstouro *pinstouro = new Obstaculos::Pinstouro();

            if (!lerDadosEntidade(entrada, pinstouro)) {
                delete pinstouro;
                return false;
            }

            registrarEntidade(pinstouro);
            return true;
        }

        return false;
    }
    Gerenciadores::Memento * Fase::salvarMemento() const
    {
        return new FaseMemento(*this);
    }

    void Fase::restaurarMemento(const Gerenciadores::Memento *memento)
    {
        Ente::restaurarMemento(memento);
        const FaseMemento *pMemento = dynamic_cast<const FaseMemento *>(memento);
        if (pMemento) {
            LEntidades = pMemento->LEntidadesMemento;
            tamanhoJanela = pMemento->tamanhoJanelaMemento;
            diretorio_Audio = pMemento->diretorio_AudioMemento;
        }
    }
} // Fases
