#include "particlewidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QTime>
#include <cmath>
#include <QResizeEvent>

ParticleWidget::ParticleWidget(QWidget *parent)
    : QWidget(parent)
    , m_quantidade(160)
    , m_distanciaConexao(150.0f)
    , m_tamanhoAnterior()
{
    // ISSO É VITAL: Faz os cliques do mouse "atravessarem" as partículas 
    // e atingirem os botões do seu menu que estão atrás/na frente delas!
    setAttribute(Qt::WA_TransparentForMouseEvents);
    
    // Deixa o fundo desse widget transparente
    setAttribute(Qt::WA_TranslucentBackground);

    QRandomGenerator(static_cast<uint>(QTime::currentTime().msec()));
    iniciarParticulas();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(atualizarParticulas()));
    m_timer.start(1);
}

void ParticleWidget::iniciarParticulas()
{
    if (width() <= 0 || height() <= 0)
        return;

    m_particulas.clear();
    for (int i = 0; i < m_quantidade; ++i) {
        Particle p;
        p.x = static_cast<float>(qrand() % qMax(1, width()));
        p.y = static_cast<float>(qrand() % qMax(1, height()));

        // Movimento mais leve para ficar mais suave visualmente
        p.vx = ((qrand() % 100) / 100.0f - 0.5f) * 0.65f;
        p.vy = ((qrand() % 100) / 100.0f - 0.5f) * 0.65f;

        m_particulas.push_back(p);
    }
}

void ParticleWidget::resizeEvent(QResizeEvent *event)
{
    if (event) {
        const QSize tamanhoNovo = event->size();
        if (tamanhoNovo != m_tamanhoAnterior && tamanhoNovo.width() > 0 && tamanhoNovo.height() > 0) {
            m_tamanhoAnterior = tamanhoNovo;
            iniciarParticulas();
        }
    }
}

void ParticleWidget::atualizarParticulas()
{
    if (width() <= 0 || height() <= 0)
        return;

    for (int i = 0; i < m_particulas.size(); ++i) {
        m_particulas[i].x += m_particulas[i].vx;
        m_particulas[i].y += m_particulas[i].vy;

        // Bate e volta nas paredes usando o tamanho real do widget
        if (m_particulas[i].x < 0.0f) {
            m_particulas[i].x = 0.0f;
            m_particulas[i].vx *= -1;
        } else if (m_particulas[i].x > width()) {
            m_particulas[i].x = width();
            m_particulas[i].vx *= -1;
        }

        if (m_particulas[i].y < 0.0f) {
            m_particulas[i].y = 0.0f;
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
    painter.setRenderHint(QPainter::Antialiasing, true);

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
                int alpha = static_cast<int>(opacidade * 120.0f);

                // Linha fina (espessura 1)
                painter.setPen(QPen(QColor(corR, corG, corB, alpha), 1));
                painter.drawLine(QPointF(m_particulas[i].x, m_particulas[i].y),
                                 QPointF(m_particulas[j].x, m_particulas[j].y));
            }
        }
    }

    // 2. Desenha os nós (pontos PEQUENOS como na imagem)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(corR, corG, corB, 225));

    for (int i = 0; i < m_particulas.size(); ++i) {
        painter.drawEllipse(QPointF(m_particulas[i].x, m_particulas[i].y), 3.2, 3.2);
    }
}