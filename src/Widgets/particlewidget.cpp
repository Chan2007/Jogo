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
        // Iniciar partículas em uma área levemente maior para evitar bordas visíveis de cara
        p.x = static_cast<float>(qrand() % qMax(1, width() + 200)) - 100.0f;
        p.y = static_cast<float>(qrand() % qMax(1, height() + 200)) - 100.0f;

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

        // Em vez de rebater na parede, faz a partícula reaparecer do outro lado
        // usando uma margem confortável para fora da tela, assim elas entram suavemente
        float margem = 100.0f;

        if (m_particulas[i].x < -margem) {
            m_particulas[i].x = width() + margem;
        } else if (m_particulas[i].x > width() + margem) {
            m_particulas[i].x = -margem;
        }

        if (m_particulas[i].y < -margem) {
            m_particulas[i].y = height() + margem;
        } else if (m_particulas[i].y > height() + margem) {
            m_particulas[i].y = -margem;
        }
    }
    update();
}
// 1. Implementação da antiga lambda como um método da classe
float ParticleWidget::getFade(float x, float y) const
{
    float marginX = 100.0f;
    float marginY = 100.0f;
    float fadeX = 1.0f;
    float fadeY = 1.0f;

    if (x < marginX) {
        fadeX = qMax(0.0f, x / marginX);
    } else if (x > width() - marginX) {
        fadeX = qMax(0.0f, (width() - x) / marginX);
    }

    if (y < marginY) {
        fadeY = qMax(0.0f, y / marginY);
    } else if (y > height() - marginY) {
        fadeY = qMax(0.0f, (height() - y) / marginY);
    }

    return fadeX * fadeY;
}

// 2. O seu paintEvent atualizado sem C++11
void ParticleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
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
                // Chama o novo méthodo auxiliar no lugar da lambda
                float fadeI = getFade(m_particulas[i].x, m_particulas[i].y);
                float fadeJ = getFade(m_particulas[j].x, m_particulas[j].y);
                float baseFade = (fadeI + fadeJ) * 0.5f;

                // Opacidade suave como na imagem de referência conjugada as bordas
                float opacidade = 1.0f - (distancia / m_distanciaConexao);
                int alpha = static_cast<int>(opacidade * 120.0f * baseFade);

                if (alpha > 0) {
                    painter.setPen(QPen(QColor(corR, corG, corB, alpha), 1));
                    painter.drawLine(QPointF(m_particulas[i].x, m_particulas[i].y),
                                     QPointF(m_particulas[j].x, m_particulas[j].y));
                }
            }
        }
    }

    // 2. Desenha os nós (pontos PEQUENOS como na imagem, com fade out nas bordas)
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < m_particulas.size(); ++i) {
        // Chama o novo méthodo auxiliar no lugar da lambda
        float fade = getFade(m_particulas[i].x, m_particulas[i].y);
        int alpha = static_cast<int>(225 * fade);
        if (alpha > 0) {
            painter.setBrush(QColor(corR, corG, corB, alpha));
            painter.drawEllipse(QPointF(m_particulas[i].x, m_particulas[i].y), 3.2, 3.2);
        }
    }
}