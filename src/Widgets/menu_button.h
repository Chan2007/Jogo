#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <QPoint>
#include <QPushButton>

class QEvent;
class QMouseEvent;
class QPainter;
class QPaintEvent;
class QPropertyAnimation;

class MenuButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)
    Q_PROPERTY(qreal pressProgress READ pressProgress WRITE setPressProgress)
    Q_PROPERTY(QPoint contentOffset READ contentOffset WRITE setContentOffset)
    Q_PROPERTY(qreal borderAngle READ borderAngle WRITE setBorderAngle)
    Q_PROPERTY(qreal opacityValue READ opacityValue WRITE setOpacityValue)

public:
    explicit MenuButton(QWidget *parent = NULL);
    explicit MenuButton(const QString& text, QWidget *parent = NULL);
    virtual QSize sizeHint() const;
    void iniciarAnimacaoEntrada(int atrasoMs);
    void setAlinhamento(Qt::Alignment alignment);

    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    qreal hoverProgress() const;
    void setHoverProgress(qreal value);

    qreal pressProgress() const;
    void setPressProgress(qreal value);

    QPoint contentOffset() const;
    void setContentOffset(const QPoint& value);

    qreal borderAngle() const;
    void setBorderAngle(qreal angle);

    qreal opacityValue() const;
    void setOpacityValue(qreal value);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    void desenharPincelada(QPainter& painter, const QRect& area) const;
    void animarHover(qreal destino);
    void animarPress(qreal destino, int duracaoMs);
    QColor interpolarCor(const QColor &corA, const QColor &corB, double t) const;

    bool hovered;
    qreal m_hoverProgress;
    qreal m_pressProgress;
    qreal m_borderAngle;
    qreal m_opacityValue;
    QPoint m_contentOffset;
    QPropertyAnimation *m_hoverAnimation;
    QPropertyAnimation *m_pressAnimation;
    QPropertyAnimation *m_offsetAnimation;
    QPropertyAnimation *m_opacityAnimation;
    QPropertyAnimation *m_borderAnimation;
};

#endif // MENU_BUTTON_H