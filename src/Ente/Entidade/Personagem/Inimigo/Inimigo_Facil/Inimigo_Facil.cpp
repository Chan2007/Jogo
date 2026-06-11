

#include "Inimigo_Facil.h"
#include "Ente/Entidade/Entidade.h"
#include "Ente/Entidade/Personagem/Jogador/Jogador.h"
#include "Gerenciador/Gerenciador_Colisao/Gerenciador_Colisao.h"
#include "Gerenciador/Gerenciador_Gravidade/Gerenciador_Gravidade.h"

Inimigo_Facil::Inimigo_Facil() :
    Inimigo(),
    raio(200.f),
    tiro(NULL)
{
    Ente::sementear();

    velocidadeMax = 250.f;
    nivelMaldade = 32;
    poder = 20;
    setVida(170);
    alcancePerseguicao = 300;
    alcanceAtaque = 110;
    elite = rand() % 10 < 5;
    cooldownAtaque = 10.5f;
    tempoUltimoAtaque = 0.0f;
    limiteDeslocamento = 600.f;
    caminhoArquivoSprite = Encontrar_Caminho::acharDiretorio_Arquivo("assets/sprites/spritesheets/Inimigos/minionrangedsheet2.png");
    if (!caminhoArquivoSprite.empty()) {
        if (getTextura().loadFromFile(caminhoArquivoSprite)) {
            getSprite().setTexture(getTextura());
            totalFramesAnimacao = 18;
            colunasSpritesheet = 9;
            tempoPorFrame = 0.08f;
            frameWidth = 1262;
            frameHeight = 1028;
            getSprite().setScale(0.08f, 0.08f);
            rectAtual = sf::IntRect(0, 0, frameWidth, frameHeight);
            getSprite().setTextureRect(rectAtual);
        }
        else {
            std::cerr << "Erro: não foi possivel carregar a spritesheet do inimigo facil em: " << caminhoArquivoSprite << std::endl;
        }
    }
    getSprite().setOrigin(static_cast<float>(frameWidth) / 2.f, static_cast<float>(frameHeight) / 2.f);
}

Inimigo_Facil::~Inimigo_Facil() {

}

void Inimigo_Facil::danificar(Personagens::Jogador* J) {
    if (J) {
        J->receberDano(causarDanoBasico());
        std::cout << "Minion atacou o jogador! Dano causado: " << causarDanoBasico() << std::endl;
    }
}

sf::FloatRect Inimigo_Facil::getTamanho() const {
    sf::FloatRect caixaImagem = getSprite().getGlobalBounds();
    // sprite: 128*2.5 = 320x320, origin no centro
    // hitbox menor e centralizada verticalmente no personagem
    float largura = 60.f;
    float altura = 60.f;
    return sf::FloatRect(
        caixaImagem.left + (caixaImagem.width / 2.f) - (largura / 2.f),
        caixaImagem.top + (caixaImagem.height / 2.f) - (altura / 2.f),
        largura,
        altura
    );
}

void Inimigo_Facil::executar() {

    if (estado == static_cast<int>(Personagens::ESTADO_MOVIMENTO)) {
        frameAcumulado += clockAnimacao.restart().asSeconds();

        if (frameAcumulado >= tempoPorFrame) {

            indexFrameAtual = (indexFrameAtual + 1) % totalFramesAnimacao;

            int coluna = indexFrameAtual % colunasSpritesheet;
            int linha = indexFrameAtual / colunasSpritesheet;

            rectAtual.left = coluna * frameWidth;
            rectAtual.top = linha * frameHeight;

            getSprite().setTextureRect(rectAtual);

            frameAcumulado -= tempoPorFrame;
        }   
    }
    else {

        indexFrameAtual = 0;
        rectAtual.left = 0;
        rectAtual.top = 0;
        getSprite().setTextureRect(rectAtual);

        clockAnimacao.restart();
        frameAcumulado = 0.0f;
    }

    const float dt = 0.016f;
    tempoUltimoAtaque += dt;

    sf::Vector2f posInimigo = getPosicao();

    Personagens::Jogador* alvoMaisProximo = NULL;
    float menorDistancia = -1.f;

    for (size_t i = 0; i < listaJogadores.size(); ++i) {
        Personagens::Jogador* j = listaJogadores[i];

        if (j != NULL && j->estaVivo()) {
            sf::Vector2f posJogador = j->getPosicao();

            float dx = posJogador.x - posInimigo.x;
            float dy = posJogador.y - posInimigo.y;
            float distancia = std::sqrt(dx * dx + dy * dy);

            if (menorDistancia < 0.0f || distancia < menorDistancia) {
                menorDistancia = distancia;
                alvoMaisProximo = j;
            }
        }
    }
    bool interagindo = false;

    if (alvoMaisProximo != NULL) {
        sf::Vector2f posAlvo = alvoMaisProximo->getSprite().getPosition();
        float dx = posAlvo.x - posInimigo.x;
        float dy = posAlvo.y - posInimigo.y;

        // Comportamento de Atacar
        if (menorDistancia <= getAlcanceAtaque()) {
            setVelocidade(sf::Vector2f(0.f, getVelocidade().y));
            interagindo = true;

            if (tempoUltimoAtaque >= cooldownAtaque && !alvoMaisProximo->getInvulneravel()) {

                Entidades::Projetil* novoTiro = new Entidades::Projetil();
                novoTiro->setPosicao(posInimigo);
                novoTiro->setDoJogador(false);

                float dirX = dx / menorDistancia;
                float dirY = dy / menorDistancia;

                novoTiro->setVelocidade(sf::Vector2f(dirX * novoTiro->getVelocidade().x, dirY * novoTiro->getVelocidade().y));

                Gerenciadores::Gerenciador_Colisao::getGerenciador().incluirEntidade(novoTiro);
                Gerenciadores::Gerenciador_Gravidade::getGerenciador().aplicarGravidade(novoTiro, true);

                if (Entidades::Entidade::getListaEntidades() != NULL) {
                    Entidades::Entidade::getListaEntidades()->incluirEntidade(novoTiro);
                }

                tempoUltimoAtaque = 0.0f;
            }
        }

        // Comportamento de Perseguir
        else if (menorDistancia <= alcancePerseguicao) {
            interagindo = true;

            if (dx > 0) {
                moverHorizontal(1.0f);
            }
            else {
                moverHorizontal(-1.0f);
            }
        }
    }

    if (!interagindo) {
        float distanciaNesteFrame = std::abs(velocidadeMax * dt);
        deslocamentoPatrulha += distanciaNesteFrame;

        // Se atingiu o limite de passos, vira para o outro lado
        if (deslocamentoPatrulha >= limiteDeslocamento) {
            inverterPatrulha();
        }

        moverHorizontal(direcaoPatrulha);
    }

    sf::Vector2f pos = getPosicao();
    pos.x += getVelocidade().x * dt;
    setPosicao(pos);
}

void Inimigo_Facil::mover() {
}

void Inimigo_Facil::salvar() {
}
