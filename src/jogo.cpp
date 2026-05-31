#include "jogo.h"
#include <iostream>
#include "Diretorio/Encontrar_Diretorio.h"

Jogo::Jogo() :
    totalFrames(376),
    max_intercalarFrames(5),
    intercalarFrames(4),
    Frames(0),
    gerenciadorTextura(),
    bgAnimation(&gerenciadorTextura),
    jogador(NULL),
    gerenciadorGravidade(NULL),
    m_window(),
    menuFont(),
    tituloText(),
    menuOptions(),
    menuPanel(),
    inicializado(false),
    menuPronto(false),
    musicaLigada(true),
    estadoTela(TelaMenu),
    opcaoSelecionada(0)
{
}

Jogo::~Jogo(){
    fechar();
}

bool Jogo::carregarRecursos()
{
    const sf::Vector2u tamanhoJanela = m_window.getSize();

    diretorio_Frame = Encontrar_Diretorio::acharDiretorio_Arquivo("assets/bg_frames/");
    diretorio_Audio = Encontrar_Diretorio::acharDiretorio_Arquivo("assets/bg_audios/bg_music");
    diretorio_Fonte = Encontrar_Diretorio::acharDiretorio_Arquivo("SFML-2.6.0/examples/island/resources/tuffy.ttf");

    bgAnimation.setTargetSize(tamanhoJanela);
    if (tamanhoJanela.x == 0 || tamanhoJanela.y == 0) {
        std::cerr << "Erro: Tamanho da janela é inválido para configurar o fundo animado." << std::endl;
        return false;
    }
    if (jogador != NULL)
        jogador->setPosicao(sf::Vector2f(40.0f, 40.0f));

    if (!diretorio_Frame.empty()) {
        if (bgAnimation.loadFrames(diretorio_Frame, totalFrames, intercalarFrames, 4, 3))
            std::cout << "Frames de background carregados com sucesso!" << std::endl;
        else
            std::cerr << "Falha ao carregar os frames de background." << std::endl;
    }

    if (!diretorio_Audio.empty()) {
        const std::string caminho_completo_musica =
            Encontrar_Diretorio::concatenarEnderecos(diretorio_Audio, "Aurora_s-Theme.ogg");
        if (bgMusic.loadMusic(caminho_completo_musica)) {
            bgMusic.setVolume(50.0f);
            bgMusic.setLoop(true);
            if (musicaLigada)
                bgMusic.play();
        }
    }

    if (diretorio_Fonte.empty()) {
        std::cerr << "Falha ao localizar a fonte do menu." << std::endl;
        return false;
    }

    if (!menuFont.loadFromFile(diretorio_Fonte)) {
        std::cerr << "Falha ao carregar a fonte do menu." << std::endl;
        return false;
    }
    if (!jogador) {
        jogador = new Personagens::Jogador();

        Gerenciadores::Gerenciador_Colisao::getInstancia().incluirEntidade(jogador);

        jogador->setCampeao(Personagens::CAMPEAO_NAAFIRI);
        jogador->setPosicao(sf::Vector2f(640.0f, 400.0f));
        std::cout << "Jogador criado: " << jogador->getNome() << std::endl;

        gerenciadorGravidade = new Gerenciadores::Gerenciador_Gravidade();
        gerenciadorGravidade->aplicarGravidade(jogador, true);
        jogador->setGerenciadorGravidade(gerenciadorGravidade);
    }
    return configurarMenu();
}

bool Jogo::configurarMenu()
{
    const sf::Vector2u tamanhoJanela = m_window.getSize();
    const float larguraPainel = 420.f;
    const float alturaPainel = 320.f;

    menuPanel.setSize(sf::Vector2f(larguraPainel, alturaPainel));
    menuPanel.setFillColor(sf::Color(15, 15, 20, 210));
    menuPanel.setOutlineThickness(2.f);
    menuPanel.setOutlineColor(sf::Color(220, 220, 230, 180));
    menuPanel.setPosition(
        (static_cast<float>(tamanhoJanela.x) - larguraPainel) / 2.f,
        (static_cast<float>(tamanhoJanela.y) - alturaPainel) / 2.f
    );

    tituloText.setFont(menuFont);
    tituloText.setString("Jogo LoL");
    tituloText.setCharacterSize(42);
    tituloText.setFillColor(sf::Color::White);
    tituloText.setPosition(menuPanel.getPosition().x + 32.f, menuPanel.getPosition().y + 28.f);

    const char* labels[] = { "Iniciar Jogo", "Musica: Ligada", "Sair" };
    menuOptions.clear();
    menuOptions.resize(3);

    for (std::size_t i = 0; i < menuOptions.size(); ++i) {
        menuOptions[i].setFont(menuFont);
        menuOptions[i].setCharacterSize(28);
        menuOptions[i].setString(labels[i]);
        menuOptions[i].setPosition(
            menuPanel.getPosition().x + 36.f,
            menuPanel.getPosition().y + 110.f + static_cast<float>(i) * 58.f
        );
    }

    atualizarMenuVisual();
    menuPronto = true;
    return true;
}

bool Jogo::inicializar()
{
    if (inicializado && m_window.isOpen()) {
        return true;
    }

    m_window.create(sf::VideoMode(1280, 720), "Jogo LoL", sf::Style::Default);
    m_window.setFramerateLimit(60);

    if (!carregarRecursos()) {
        return false;
    }

    inicializado = true;
    return true;
}

void Jogo::iniciarGameplay()
{
    if (!inicializado) {
        return;
    }

    estadoTela = TelaGameplay;
}

void Jogo::setMusicaLigada(bool ligada)
{
    musicaLigada = ligada;

    if (musicaLigada)
        bgMusic.play();
    else
        bgMusic.pause();

    atualizarMenuVisual();
}

void Jogo::setVolumeMusica(float volume)
{
    bgMusic.setVolume(volume);
}

bool Jogo::musicaEstaLigada() const
{
    return musicaLigada;
}

void Jogo::executar()
{
    if (!inicializar()) {
        return;
    }

    while (estaAberto()) {
        atualizar();
    }
}

void Jogo::processarEventoMenu(const sf::Event& evento)
{
    if (evento.type != sf::Event::KeyPressed) {
        return;
    }

    if (evento.key.code == sf::Keyboard::Up) {
        if (opcaoSelecionada == 0)
            opcaoSelecionada = menuOptions.size() - 1;
        else
            --opcaoSelecionada;
        atualizarMenuVisual();
    } else if (evento.key.code == sf::Keyboard::Down) {
        opcaoSelecionada = (opcaoSelecionada + 1) % menuOptions.size();
        atualizarMenuVisual();
    } else if (evento.key.code == sf::Keyboard::Enter || evento.key.code == sf::Keyboard::Space) {
        executarOpcaoMenu();
    }
}

void Jogo::processarEventoGameplay(const sf::Event& evento)
{
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        estadoTela = TelaMenu;
        atualizarMenuVisual();
    }
}

void Jogo::processarEventos()
{
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            inicializado = false;
            return;
        }

        if (event.type == sf::Event::Resized) {

            sf::FloatRect areaVisivel(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            m_window.setView(sf::View(areaVisivel));

            bgAnimation.setTargetSize(m_window.getSize());

            if (menuPronto) {
                configurarMenu();
            }
        }

        if (estadoTela == TelaMenu)
            processarEventoMenu(event);
        else
            processarEventoGameplay(event);
    }
}

void Jogo::atualizarMenuVisual()
{
    if (!menuPronto) {
        return;
    }

    menuOptions[1].setString(musicaLigada ? "Musica: Ligada" : "Musica: Desligada");

    for (std::size_t i = 0; i < menuOptions.size(); ++i) {
        if (i == opcaoSelecionada) {
            menuOptions[i].setFillColor(sf::Color(255, 215, 80));
            menuOptions[i].setStyle(sf::Text::Bold);
        } else {
            menuOptions[i].setFillColor(sf::Color(235, 235, 240));
            menuOptions[i].setStyle(sf::Text::Regular);
        }
    }
}

void Jogo::executarOpcaoMenu()
{
    switch (opcaoSelecionada) {
        case 0:
            estadoTela = TelaGameplay;
            break;
        case 1:
            musicaLigada = !musicaLigada;
            if (musicaLigada)
                bgMusic.play();
            else
                bgMusic.pause();
            atualizarMenuVisual();
            break;
        case 2:
            m_window.close();
            break;
        default:
            break;
    }
}

void Jogo::desenharMenu()
{
    bgAnimation.update();
    bgAnimation.draw(m_window);
    m_window.draw(menuPanel);
    m_window.draw(tituloText);

    for (std::size_t i = 0; i < menuOptions.size(); ++i)
        m_window.draw(menuOptions[i]);
}

void Jogo::desenharGameplay()
{
    float dt = relogio_fisica.restart().asSeconds();

    if (dt > 0.1f) { dt = 0.1f; }

    bgAnimation.update();
    m_window.clear(sf::Color::Black);
    bgAnimation.draw(m_window);

    if (jogador) {
        gerenciadorGravidade->executar(dt);
        Gerenciadores::Gerenciador_Colisao::getInstancia().executar(jogador, gerenciadorGravidade, m_window.getSize());
        jogador->atualizar();
        jogador->desenhar(m_window);
    }
}

void Jogo::atualizar()
{
    if (!inicializado || !m_window.isOpen()) {
        return;
    }

    processarEventos();
    if (!m_window.isOpen()) {
        return;
    }

    m_window.clear(sf::Color::Black);

    if (estadoTela == TelaMenu)
        desenharMenu();
    else
        desenharGameplay();

    m_window.display();
}

bool Jogo::estaAberto() const
{
    return m_window.isOpen();
}

void Jogo::fechar()
{
    if (m_window.isOpen()) {
        m_window.close();
    }
    if (jogador) {
        delete jogador;
        jogador = NULL;
    }

    if (gerenciadorGravidade) {
        delete gerenciadorGravidade;
        gerenciadorGravidade = NULL;
    }
    inicializado = false;
}
