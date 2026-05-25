#include "particlewidget.h"
#include <QPainter>
#include <cmath>
#include <cstdlib>

ParticleWidget::ParticleWidget(QWidget *parent)
    : QWidget(parent)
    , m_quantidade(45)            // Quantidade de bolinhas na tela
    , m_distanciaConexao(130.0f)  // Distância máxima para criar a teia/linha
{
    // ISSO É VITAL: Faz os cliques do mouse "atravessarem" as partículas 
    // e atingirem os botões do seu menu que estão atrás/na frente delas!
    setAttribute(Qt::WA_TransparentForMouseEvents);
    
    // Deixa o fundo desse widget transparente
    setAttribute(Qt::WA_TranslucentBackground);

    iniciarParticulas();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(atualizarParticulas()));
    m_timer.start(16); // Roda a ~60 FPS (1000ms / 60)
}

void ParticleWidget::iniciarParticulas()
{
    m_particulas.clear();
    for (int i = 0; i < m_quantidade; ++i) {
        Particle p;
        p.x = rand() % 1920; // Posição aleatória inicial
        p.y = rand() % 1080;
        
        // Velocidade aleatória entre -0.5 e 0.5
        p.vx = ((rand() % 100) / 100.0f - 0.5f) * 1.5f; 
        p.vy = ((rand() % 100) / 100.0f - 0.5f) * 1.5f;
        
        m_particulas.push_back(p);
    }
}

void ParticleWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void ParticleWidget::atualizarParticulas()
{
    // A caixa invisível: Da metade da tela (50%) para a direita,
    // e do meio da tela (50%) para baixo.
    float limiteEsquerdo = width() * 0.50f;
    float limiteTopo = height() * 0.50f;

    for (int i = 0; i < m_particulas.size(); ++i) {
        m_particulas[i].x += m_particulas[i].vx;
        m_particulas[i].y += m_particulas[i].vy;

        // Bate e volta nas paredes Esquerda e Direita
        if (m_particulas[i].x < limiteEsquerdo) {
            m_particulas[i].x = limiteEsquerdo;
            m_particulas[i].vx *= -1;
        } else if (m_particulas[i].x > width()) {
            m_particulas[i].x = width();
            m_particulas[i].vx *= -1;
        }

        // Bate e volta no Teto e no Chão
        if (m_particulas[i].y < limiteTopo) {
            m_particulas[i].y = limiteTopo;
            m_particulas[i].vy *= -1;
        } else if (m_particulas[i].y > height()) {
            m_particulas[i].y = height();
            m_particulas[i].vy *= -1;
        }
    }
    update();
}
void ParticleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Cor idêntica à da imagem, mas adaptada para aparecer no fundo branco
    const int corR = 0;
    const int corG = 0;
    const int corB = 0;

    // 1. Desenha as linhas de conexão PRIMEIRO (bem finas)
    for (int i = 0; i < m_particulas.size(); ++i) {
        for (int j = i + 1; j < m_particulas.size(); ++j) {

            float dx = m_particulas[i].x - m_particulas[j].x;
            float dy = m_particulas[i].y - m_particulas[j].y;
            float distancia = std::sqrt(dx * dx + dy * dy);

            if (distancia < m_distanciaConexao) {
                // Opacidade suave como na imagem de referência
                float opacidade = 1.0f - (distancia / m_distanciaConexao);
                int alpha = static_cast<int>(opacidade * 140);

                // Linha fina (espessura 1)
                painter.setPen(QPen(QColor(corR, corG, corB, alpha), 1));
                painter.drawLine(QPointF(m_particulas[i].x, m_particulas[i].y),
                                 QPointF(m_particulas[j].x, m_particulas[j].y));
            }
        }
    }

    // 2. Desenha os nós (pontos PEQUENOS como na imagem)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(corR, corG, corB, 200));

    for (int i = 0; i < m_particulas.size(); ++i) {
        // Raio 2.0 deixa as bolinhas pequenas e delicadas
        painter.drawEllipse(QPointF(m_particulas[i].x, m_particulas[i].y), 2.0, 2.0);
    }
}