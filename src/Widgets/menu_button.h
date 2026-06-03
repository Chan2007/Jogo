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
    Q_PROPERTY(qreal hoverProgress READ getHoverProgress WRITE setHoverProgress)
    Q_PROPERTY(qreal pressProgress READ getPressProgress WRITE setPressProgress)
    Q_PROPERTY(QPoint contentOffset READ getContentOffset WRITE setContentOffset)
    Q_PROPERTY(qreal borderAngle READ getBorderAngle WRITE setBorderAngle)
    Q_PROPERTY(qreal opacityValue READ getOpacityValue WRITE setOpacityValue)

public:
    explicit MenuButton(QWidget *parent = NULL);
    explicit MenuButton(const QString& text, QWidget *parent = NULL);
    virtual QSize sizeHint() const;
    void iniciarAnimacaoEntrada(int atrasoMs);
    void setAlinhamento(Qt::Alignment alignment);

    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    qreal getHoverProgress() const;
    void setHoverProgress(qreal value);

    qreal getPressProgress() const;
    void setPressProgress(qreal value);

    QPoint getContentOffset() const;
    void setContentOffset(const QPoint& value);

    qreal getBorderAngle() const;
    void setBorderAngle(qreal angle);

    qreal getOpacityValue() const;
    void setOpacityValue(qreal value);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    void decorarBotao(QPainter& painter, const QRect& area) const;
    void animarHover(qreal destino);
    void animarPress(qreal destino, int duracaoMs);
    static QColor interpolarCor(const QColor &corA, const QColor &corB, double t) ;

    bool hovered;
    qreal hoverProgress;
    qreal pressProgress;
    qreal borderAngle;
    qreal opacityValue;
    QPoint contentOffset;
    QPropertyAnimation *hoverAnimation;
    QPropertyAnimation *pressAnimation;
    QPropertyAnimation *offsetAnimation;
    QPropertyAnimation *opacityAnimation;
    QPropertyAnimation *borderAnimation;
};

#endif // MENU_BUTTON_H