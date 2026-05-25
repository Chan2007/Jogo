#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <QPoint>
#include <QPushButton>

class QEvent;
class QGraphicsOpacityEffect;
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

public:
    explicit MenuButton(const QString& text, QWidget *parent = 0);

    virtual QSize sizeHint() const;
    void iniciarAnimacaoEntrada(int atrasoMs);

    qreal hoverProgress() const;
    void setHoverProgress(qreal value);

    qreal pressProgress() const;
    void setPressProgress(qreal value);

    QPoint contentOffset() const;
    void setContentOffset(const QPoint& value);

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
    QPoint m_contentOffset;
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_hoverAnimation;
    QPropertyAnimation *m_pressAnimation;
    QPropertyAnimation *m_offsetAnimation;
    QPropertyAnimation *m_opacityAnimation;
};

#endif // MENU_BUTTON_H
