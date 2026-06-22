
#include <iostream>

#include "Personagem.h"

namespace Personagens {

    Personagem::Personagem() :
        Entidade("Personagem"),
        velocidade(0.0f, 0.0f),
        aceleracao(0.0f, 0.0f),

        caminhoArquivoSpriteMovimento(""),
        caminhoArquivoSpritePulo(""),
        tempoPorFrame(0.12f),
        frameAcumulado(0.0f),
        indexFrameAtual(0),
        totalFramesAnimacao(0),
        colunasSpritesheet(0),
        linhasSpritesheet(0),
        frameWidth(0),
        frameHeight(0),
        texturaAtaque(new sf::Texture()),
        texturaPulo(new sf::Texture()),
        texturaMovimento(new sf::Texture()),
        texturaOcioso(new sf::Texture()),

        velocidadeMax(0.f),
        vida(100),
        vidaMaxima(100),
        poder(12),
        alcanceAtaque(125),
        chanceCritica(gerar_num_norm(50, 15.0f, 0.0f, 100.0f)),
        regeneracaoVida(2.0f),
        estado(static_cast<int>(ESTADO_OCIOSO)),
        tempoDano(0),
        invulneravel(false)
    {
    }

    Personagem::~Personagem() {
        if (texturaMovimento) delete texturaMovimento;
        if (texturaPulo) delete texturaPulo;
        if (texturaAtaque) delete texturaAtaque;
        if (texturaOcioso) delete texturaOcioso;
        texturaMovimento = NULL;
        texturaPulo = NULL;
        texturaAtaque = NULL;
        texturaOcioso = NULL;
    }

    sf::FloatRect Personagem::getTamanho() const {

        sf::FloatRect caixaImagem = getSprite().getGlobalBounds();

        float larguraHitbox = 70.f;
        float alturaHitbox = 90.f;

        float deslocamentoX = 12.0f;
        float deslocamentoY = -17.5f;

        return sf::FloatRect(
            caixaImagem.left + (caixaImagem.width / 2.0f) - (larguraHitbox / 2.0f) + deslocamentoX,
            caixaImagem.top + caixaImagem.height - alturaHitbox + deslocamentoY,
            larguraHitbox,
            alturaHitbox
        );
    }

    void Personagem::setVidaMaxima(float valor) {
        if (valor <= 0)
            return;
        vidaMaxima = valor;
        if (vida > vidaMaxima)
            vida = vidaMaxima;
    }

    void Personagem::setVida(float valor) {
        if (valor < 0)
            vida = 0;
        else if (valor > vidaMaxima)
            vida = vidaMaxima;
        else
            vida = valor;
        if (vida <= 0) {
            estado = static_cast<int>(ESTADO_MORTO);
            setVigente(false);
        }
        
    }

    float Personagem::receberDano(float dano) {
        if (dano <= 0 || !estaVivo()) return 0;
        setVida(vida - dano);
        std::cout << getNome() << " recebeu " << dano << " de dano. Vida: " << vida << std::endl;
        getSprite().setColor(sf::Color(255, 80, 80, 255));
        if (vida <= 0) estado = static_cast<int>(ESTADO_MORTO);
        tempoDano = 0.5f;
        clockDano.restart();
        return dano;
    }


    float Personagem::causarDanoBasico() const {
        return rand() % 101 < chanceCritica ? poder * (1 + chanceCritica/100) : poder;
    }

    void Personagem::curar(float valor) {
        if (valor > 0 && valor + vida < vidaMaxima) vida += valor;
    }

    void Personagem::regenerarVida(float dt) {
        if (dt <= 0.0f || !estaVivo())
            return;
        curar(static_cast<int>(regeneracaoVida * dt));
    }

    void Personagem::moverHorizontal(float direcao) {
        sf::Vector2f velAtual = getVelocidade();

        if (std::abs(velAtual.x) > velocidadeMax) { velAtual.x *= 0.95f; }
        else { velAtual.x = direcao * velocidadeMax; }

        setVelocidade(velAtual);
        estado = (direcao == 0.0f) ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MOVIMENTO);
    }

    void Personagem::resetarCombate() {
        velocidade = sf::Vector2f(0.0f, 0.0f);
        aceleracao = sf::Vector2f(0.0f, 0.0f);
        estado = estaVivo() ? static_cast<int>(ESTADO_OCIOSO) : static_cast<int>(ESTADO_MORTO);
    }

    void Personagem::salvarPersonagem() {
        if (buffer) {
            (*buffer) << velocidade.x << ' '
                << velocidade.y << ' '
                << vida << ' '
                << vidaMaxima << ' '
                << estado << ' '
                << invulneravel << ' ';
        }
    }
    Memento* Personagem::salvarMemento() const  {
        return new PersonagemMemento(*this);
    }

    void Personagem::restaurarMemento(const Memento* memento) {
        if (!memento) return;
        Entidade::restaurarMemento(memento);

        const PersonagemMemento* pMemento = dynamic_cast<const PersonagemMemento*>(memento);
        if (pMemento) {
            velocidade = pMemento->velocidadeMemento;
            aceleracao = pMemento->aceleracaoMemento;
            caminhoArquivoSpriteOcioso = pMemento->caminhoArquivoSpriteOciosoMemento;
            caminhoArquivoSpriteMovimento = pMemento->caminhoArquivoSpriteMovimentoMemento;
            caminhoArquivoSpritePulo = pMemento->caminhoArquivoSpritePuloMemento;
            caminhoArquivoSpriteAtaque = pMemento->caminhoArquivoSpriteAtaqueMemento;
            texturaPulo = pMemento->texturaPuloMemento;
            texturaMovimento = pMemento->texturaMovimentoMemento;
            texturaAtaque = pMemento->texturaAtaqueMemento;
            texturaOcioso = pMemento->texturaOciosoMemento;
            rectAtual = pMemento->rectAtualMemento;
            clockAnimacao = pMemento->clockAnimacaoMemento;
            tempoPorFrame = pMemento->tempoPorFrameMemento;
            frameAcumulado = pMemento->frameAcumuladoMemento;
            indexFrameAtual = pMemento->indexFrameAtualMemento;
            totalFramesAnimacao = pMemento->totalFramesAnimacaoMemento;
            colunasSpritesheet = pMemento->colunasSpritesheetMemento;
            frameWidth = pMemento->frameWidthMemento;
            frameHeight = pMemento->frameHeightMemento;
            velocidadeMax = pMemento->velocidadeMaxMemento;
            vida = pMemento->vidaMemento;
            vidaMaxima = pMemento->vidaMaximaMemento;
            poder = pMemento->poderMemento;
            alcanceAtaque = pMemento->alcanceAtaqueMemento;
            chanceCritica = pMemento->chanceCriticaMemento;
            regeneracaoVida = pMemento->regeneracaoVidaMemento;
            estado = pMemento->estadoMemento;
            clockDano = pMemento->clockDanoMemento;
            tempoDano = pMemento->tempoDanoMemento;
            invulneravel = pMemento->invulneravelMemento;

            // Atualiza o frame do ‘sprite’ com o rect que acabou de ser restaurado
            getSprite().setTextureRect(rectAtual);
        }
    }
}
