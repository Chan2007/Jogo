#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QSize>

struct Particle {
    float x;
    float y;
    float vx;
    float vy;
};

class ParticleWidget : public QWidget {
    Q_OBJECT
public:
    explicit ParticleWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void atualizarParticulas();

private:
    void iniciarParticulas();
    float getFade(float x, float y) const;

    QVector<Particle> m_particulas;
    QTimer m_timer;
    int m_quantidade;
    float m_distanciaConexao;
    QSize m_tamanhoAnterior;
};

#endif // PARTICLEWIDGET_H