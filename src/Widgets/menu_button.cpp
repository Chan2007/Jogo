#include "menu_button.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QConicalGradient>

MenuButton::MenuButton(QWidget *parent)
    : MenuButton("", parent)
{
}

MenuButton::MenuButton(const QString& text, QWidget *parent)
    : QPushButton(text, parent)
    , hovered(false)
    , m_hoverProgress(0.0)
    , m_pressProgress(0.0)
    , m_borderAngle(0.0)
    , m_opacityValue(1.0)
    , m_contentOffset(QPoint(-18, 0))
    , m_hoverAnimation(new QPropertyAnimation(this, "hoverProgress", this))
    , m_pressAnimation(new QPropertyAnimation(this, "pressProgress", this))
    , m_offsetAnimation(new QPropertyAnimation(this, "contentOffset", this))
    , m_opacityAnimation(new QPropertyAnimation(this, "opacityValue", this))
    , m_borderAnimation(new QPropertyAnimation(this, "borderAngle", this))
{

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_StyledBackground, true);

    m_hoverAnimation->setDuration(170);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_pressAnimation->setDuration(90);
    m_pressAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_offsetAnimation->setDuration(220);
    m_offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_opacityAnimation->setDuration(280);
    m_opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_borderAnimation->setStartValue(0.0);
    m_borderAnimation->setEndValue(360.0);
    m_borderAnimation->setDuration(1800);
    m_borderAnimation->setLoopCount(-1);
    m_borderAnimation->setEasingCurve(QEasingCurve::Linear);
}

QSize MenuButton::sizeHint() const
{
    return QSize(220, 400);
}

void MenuButton::iniciarAnimacaoEntrada(int atrasoMs)
{
    if (atrasoMs < 0)
        atrasoMs = 0;

    QTimer::singleShot(atrasoMs, this, SLOT(update()));
    QTimer::singleShot(atrasoMs, m_opacityAnimation, SLOT(stop()));
    QTimer::singleShot(atrasoMs, m_offsetAnimation, SLOT(stop()));

    m_opacityAnimation->setStartValue(m_opacityValue);
    m_opacityAnimation->setEndValue(1.0);

    m_offsetAnimation->setStartValue(QPoint(-18, 0));
    m_offsetAnimation->setEndValue(QPoint(0, 0));

    QTimer::singleShot(atrasoMs, m_opacityAnimation, SLOT(start()));
    QTimer::singleShot(atrasoMs, m_offsetAnimation, SLOT(start()));
}

void MenuButton::setTextAlignment(Qt::Alignment alignment)
{
    m_textAlignment = alignment;
    update();
}
qreal MenuButton::hoverProgress() const
{
    return m_hoverProgress;
}

void MenuButton::setHoverProgress(qreal value)
{
    m_hoverProgress = value;
    update();
}

qreal MenuButton::pressProgress() const
{
    return m_pressProgress;
}

void MenuButton::setPressProgress(qreal value)
{
    m_pressProgress = value;
    update();
}

QPoint MenuButton::contentOffset() const
{
    return m_contentOffset;
}

void MenuButton::setContentOffset(const QPoint& value)
{
    m_contentOffset = value;
    update();
}

qreal MenuButton::borderAngle() const { return m_borderAngle; }

void MenuButton::setBorderAngle(qreal angle)
{
    m_borderAngle = angle;
    update();
}

qreal MenuButton::opacityValue() const
{
    return m_opacityValue;
}

void MenuButton::setOpacityValue(qreal value)
{
    m_opacityValue = value;
    update();
}

void MenuButton::enterEvent(QEvent *event)
{
    hovered = true;
    animarHover(1.0);
    m_borderAnimation->start();
    QPushButton::enterEvent(event);
}

void MenuButton::leaveEvent(QEvent *event)
{
    hovered = false;
    animarHover(0.0);
    connect(m_hoverAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (m_hoverProgress < 0.01)
            m_borderAnimation->stop();
    }, Qt::UniqueConnection);
    QPushButton::leaveEvent(event);
}

void MenuButton::mousePressEvent(QMouseEvent *event)
{
    animarPress(1.0, 80);
    QPushButton::mousePressEvent(event);
}

void MenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    animarPress(0.0, 130);
    QPushButton::mouseReleaseEvent(event);
}

void MenuButton::desenharPincelada(QPainter& painter, const QRect& area) const
{
    const QRect strokeRect = area.adjusted(4, 4, -4, -4);

    painter.save();
    painter.setPen(Qt::NoPen);

    // 1. Criação do Gradiente Horizontal (Esquerda para a Direita)
    QLinearGradient fundoGradiente(strokeRect.left(), 0, strokeRect.right(), 0);

    fundoGradiente.setColorAt(1.00, QColor(255, 255, 255, 0)); // Borda Esquerda: Transparente
    fundoGradiente.setColorAt(0.75, QColor(235, 235, 235, 255));
    fundoGradiente.setColorAt(0.50, QColor(215, 215, 215, 255));
    fundoGradiente.setColorAt(0.25, QColor(195, 195, 195, 255));
    fundoGradiente.setColorAt(0.00, QColor(175, 175, 175, 255)); // Borda Direita: Preto

    // Aplica o gradiente como preenchimento do botão
    painter.setBrush(QBrush(fundoGradiente));
    painter.drawRoundedRect(strokeRect, 8, 8);

    // 2. Borda Preta Superior e Inferior (Apenas quando NÃO está em Hover/Pressionado)
    // Calculamos o alpha inverso: se m_hoverProgress for 0, alpha será 220 (totalmente visível)
    // Se m_hoverProgress for 1, alpha será 0 (totalmente invisível)
    int alphaBorda = static_cast<int>(220.0 * (1.0 - m_hoverProgress));

    // Se o botão for clicado (isDown), forçamos o desaparecimento imediato da borda
    if (isDown()) {
        alphaBorda = 0;
    }

    if (alphaBorda > 0) {
        QPen bordaPretaPen(QColor(0, 0, 0, alphaBorda), 2);
        painter.setPen(bordaPretaPen);

        // Linha Superior
        painter.drawLine(QPoint(strokeRect.left(), strokeRect.top()),
                         QPoint(strokeRect.right(), strokeRect.top()));

        // Linha Inferior
        painter.drawLine(QPoint(strokeRect.left(), strokeRect.bottom()),
                         QPoint(strokeRect.right(), strokeRect.bottom()));
    }

    // 3. Feedback visual do Hover (Luz interna branca que aparece com o mouse)
    if (m_hoverProgress > 0.01 || isDown()) {
        painter.setPen(Qt::NoPen);
        int hoverAlpha = static_cast<int>(40.0 * m_hoverProgress);
        painter.setBrush(QColor(255, 255, 255, hoverAlpha));
        painter.drawRoundedRect(strokeRect, 8, 8);
    }

    // Marcador decorativo à esquerda (Triângulo)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 255));
    QPoint marcadorPontos[] = {
        QPoint(strokeRect.left() + 8, strokeRect.center().y() - 6),
        QPoint(strokeRect.left() + 2, strokeRect.center().y()),
        QPoint(strokeRect.left() + 8, strokeRect.center().y() + 6)
    };
    painter.drawPolygon(marcadorPontos, 3);

    painter.restore();
}
void MenuButton::animarHover(qreal destino)
{
    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(m_hoverProgress);
    m_hoverAnimation->setEndValue(destino);
    m_hoverAnimation->start();
}

void MenuButton::animarPress(qreal destino, int duracaoMs)
{
    m_pressAnimation->stop();
    m_pressAnimation->setDuration(duracaoMs);
    m_pressAnimation->setStartValue(m_pressProgress);
    m_pressAnimation->setEndValue(destino);
    m_pressAnimation->start();
}

QColor MenuButton::interpolarCor(const QColor &corA, const QColor &corB, double t) const
{
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    int r = static_cast<int>(corA.red()   + (corB.red()   - corA.red())   * t);
    int g = static_cast<int>(corA.green() + (corB.green() - corA.green()) * t);
    int b = static_cast<int>(corA.blue()  + (corB.blue()  - corA.blue())  * t);
    int a = static_cast<int>(corA.alpha() + (corB.alpha() - corA.alpha()) * t);

    return QColor(r, g, b, a);
}

void MenuButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setOpacity(m_opacityValue);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    const QRect area = rect().adjusted(0, 0, -1, -1);

    const bool desenharHover = (m_hoverProgress > 0.01) || isDown() || (m_pressProgress > 0.01);

    if (desenharHover)
        desenharPincelada(painter, area);

    const QColor sombraNormal(0, 0, 0, 255);
    const QColor textoNormal(200, 200, 200);

    const QColor sombraHover(255, 255, 255, 65);
    const QColor textoHover(64, 69, 77);

    double progressoVisual = (m_hoverProgress > m_pressProgress) ? m_hoverProgress : m_pressProgress;

    const QColor sombraAtual = interpolarCor(sombraNormal, sombraHover, progressoVisual);
    const QColor textoAtual  = interpolarCor(textoNormal, textoHover, progressoVisual);

    const int pressShift = static_cast<int>(2.0 * m_pressProgress);
    const QPoint offset = m_contentOffset + QPoint(0, pressShift);

    painter.setPen(sombraAtual);
    painter.drawText(area.adjusted(26 + offset.x(), 0, -25, 2), m_textAlignment, text());

    painter.setPen(textoAtual);
    painter.drawText(area.adjusted(25 + offset.x(), 0, -25, -2), m_textAlignment, text());

    if (m_hoverProgress < 0.01) {  // Desvanece enquanto progresso < 1%
        const int alpha = static_cast<int>(255.0 * (1.0 - (m_hoverProgress / 0.3)));
        painter.setPen(QPen(QColor(0, 0, 0, alpha), 2));
        painter.drawLine(area.left(), area.top(), area.right(), area.top());        // borda superior
        painter.drawLine(area.left(), area.bottom(), area.right(), area.bottom());  // borda inferior
    }
    // Borda animada no hover (sobrescreve a borda normal)
    if (m_hoverProgress > 0.01) {
        // borda animada (aparece logo, sobrescreve a reta)
        const QRectF r = QRectF(area).adjusted(1, 1, -1, -1);

        QConicalGradient grad(r.center(), m_borderAngle);
        grad.setColorAt(0.00, QColor(0,   0,   0,   static_cast<int>(255 * m_hoverProgress)));
        grad.setColorAt(0.40, QColor(80,  80,  80,  static_cast<int>(160 * m_hoverProgress)));
        grad.setColorAt(0.70, QColor(180, 180, 180, static_cast<int>(60  * m_hoverProgress)));
        grad.setColorAt(1.00, QColor(0,   0,   0,   0));

        QPen bordaPen;
        bordaPen.setBrush(QBrush(grad));
        bordaPen.setWidthF(2.0);
        bordaPen.setJoinStyle(Qt::MiterJoin);

        painter.setPen(bordaPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(r, 8, 8);
    }
}