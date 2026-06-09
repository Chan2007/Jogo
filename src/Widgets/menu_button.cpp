#include "menu_button.h"
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QConicalGradient>

MenuButton::MenuButton(QWidget *parent): MenuButton("", parent)
{
}

MenuButton::MenuButton(const QString& text, QWidget *parent)
    : QPushButton(text, parent)
    , hovered(false)
    , hoverProgress(0.0)
    , pressProgress(0.0)
    , borderAngle(0.0)
    , opacityValue(1.0)
    , contentOffset(QPoint(-18, 0))
    , hoverAnimation(new QPropertyAnimation(this, "hoverProgress", this))
    , pressAnimation(new QPropertyAnimation(this, "pressProgress", this))
    , offsetAnimation(new QPropertyAnimation(this, "contentOffset", this))
    , opacityAnimation(new QPropertyAnimation(this, "opacityValue", this))
    , borderAnimation(new QPropertyAnimation(this, "borderAngle", this))
{

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_StyledBackground, true);

    hoverAnimation->setDuration(1000);
    hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    pressAnimation->setDuration(1000);
    pressAnimation->setEasingCurve(QEasingCurve::OutCubic);

    offsetAnimation->setDuration(500);
    offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);

    opacityAnimation->setDuration(500);
    opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);

    borderAnimation->setStartValue(0.0);
    borderAnimation->setEndValue(360.0);
    borderAnimation->setDuration(2000);
    borderAnimation->setLoopCount(-1);
    borderAnimation->setEasingCurve(QEasingCurve::Linear);

    connect(hoverAnimation, SIGNAL(finished()), this, SLOT(onHoverAnimationFinished()));
}

QSize MenuButton::sizeHint() const {
    return QSize(220, 400);
}

void MenuButton::iniciarAnimacaoEntrada(int atrasoMs) const {
    if (atrasoMs < 0)
        atrasoMs = 0;

    QTimer::singleShot(atrasoMs, this, SLOT(update()));
    QTimer::singleShot(atrasoMs, opacityAnimation, SLOT(stop()));
    QTimer::singleShot(atrasoMs, offsetAnimation, SLOT(stop()));

    opacityAnimation->setStartValue(opacityValue);
    opacityAnimation->setEndValue(1.0);

    offsetAnimation->setStartValue(QPoint(-18, 0));
    offsetAnimation->setEndValue(QPoint(0, 0));

    QTimer::singleShot(atrasoMs, opacityAnimation, SLOT(start()));
    QTimer::singleShot(atrasoMs, offsetAnimation, SLOT(start()));
}

void MenuButton::setAlinhamento(const Qt::Alignment alignment) {
    textAlignment = alignment;
    update();
}
qreal MenuButton::getHoverProgress() const {
    return hoverProgress;
}

void MenuButton::setHoverProgress(const qreal value) {
    hoverProgress = value;
    update();
}

qreal MenuButton::getPressProgress() const {
    return pressProgress;
}

void MenuButton::setPressProgress(const qreal value) {
    pressProgress = value;
    update();
}

QPoint MenuButton::getContentOffset() const {
    return contentOffset;
}

void MenuButton::setContentOffset(const QPoint& value) {
    contentOffset = value;
    update();
}

qreal MenuButton::getBorderAngle() const { return borderAngle; }

void MenuButton::setBorderAngle(const qreal angle) {
    borderAngle = angle;
    update();
}

qreal MenuButton::getOpacityValue() const
{
    return opacityValue;
}

void MenuButton::setOpacityValue(const qreal value) {
    opacityValue = value;
    update();
}

void MenuButton::enterEvent(QEvent *event) {
    hovered = true;
    animarHover(1.0);
    borderAnimation->start();
    QPushButton::enterEvent(event);
}

void MenuButton::leaveEvent(QEvent *event) {
    hovered = false;
    animarHover(0.0);
    QPushButton::leaveEvent(event);
}

void MenuButton::mousePressEvent(QMouseEvent *event) {
    animarPress(1.0, 100);
    QPushButton::mousePressEvent(event);
}

void MenuButton::mouseReleaseEvent(QMouseEvent *event) {
    animarPress(0.0, 100);
    QPushButton::mouseReleaseEvent(event);
}

void MenuButton::decorarBotao(QPainter& painter, const QRect& area) const {
    const QRect strokeRect = area.adjusted(4, 4, -4, -4);

    painter.save();
    painter.setPen(Qt::NoPen);

    // 1. Criação do Gradiente Horizontal (Esquerda para a Direita)
    QLinearGradient fundoGradiente(strokeRect.left(), 0, strokeRect.right(), 0);

    fundoGradiente.setColorAt(1.00, QColor(255, 255, 255, 0)); // Borda Esquerda: Transparente
    fundoGradiente.setColorAt(0.75, QColor(235, 235, 235, 255));
    fundoGradiente.setColorAt(0.50, QColor(215, 215, 215, 255));
    fundoGradiente.setColorAt(0.25, QColor(195, 195, 195, 255));
    fundoGradiente.setColorAt(0.00, QColor(175, 175, 175, 255)); // Borda Direita: Cinza claro

    // Aplica o gradiente como preenchimento do botão
    painter.setBrush(QBrush(fundoGradiente));
    painter.drawRoundedRect(strokeRect, 8, 8);

    // 2. Borda Preta Superior e Inferior (Apenas quando NÃO está em Hover/Pressionado)
    int alphaBorda = static_cast<int>(220.0 * (1.0 - hoverProgress));

    // Se o botão for clicado (isDown), a borda desaparece
    if (isDown()) alphaBorda = 0;

    if (alphaBorda > 0) {
        const QPen bordaPretaPen(QColor(0, 0, 0, alphaBorda), 2);
        painter.setPen(bordaPretaPen);

        // Linha Superior
        painter.drawLine(QPoint(strokeRect.left(), strokeRect.top()),
                         QPoint(strokeRect.right(), strokeRect.top()));

        // Linha Inferior
        painter.drawLine(QPoint(strokeRect.left(), strokeRect.bottom()),
                         QPoint(strokeRect.right(), strokeRect.bottom()));
    }

    // 3. Feedback visual do Hover
    if (hoverProgress > 0.01 || isDown()) {
        painter.setPen(Qt::NoPen);
        const int hoverAlpha = static_cast<int>(40.0 * hoverProgress);
        painter.setBrush(QColor(255, 255, 255, hoverAlpha));
        painter.drawRoundedRect(strokeRect, 8, 8);
    }

    // Marcador decorativo à esquerda (triângulo)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 255));
    const QPoint marcadorPontos[] = {
        QPoint(strokeRect.left() + 8, strokeRect.center().y() - 6),
        QPoint(strokeRect.left() + 2, strokeRect.center().y()),
        QPoint(strokeRect.left() + 8, strokeRect.center().y() + 6)
    };
    painter.drawPolygon(marcadorPontos, 3);

    painter.restore();
}
void MenuButton::animarHover(const qreal destino) const {
    hoverAnimation->stop();
    hoverAnimation->setStartValue(hoverProgress);
    hoverAnimation->setEndValue(destino);
    hoverAnimation->start();
}

void MenuButton::animarPress(const qreal destino, const int duracaoMs) const {
    pressAnimation->stop();
    pressAnimation->setDuration(duracaoMs);
    pressAnimation->setStartValue(pressProgress);
    pressAnimation->setEndValue(destino);
    pressAnimation->start();
}

QColor MenuButton::interpolarCor(const QColor &corA, const QColor &corB, double t) {
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    const int r = static_cast<int>(corA.red()   + (corB.red()   - corA.red())   * t);
    const int g = static_cast<int>(corA.green() + (corB.green() - corA.green()) * t);
    const int b = static_cast<int>(corA.blue()  + (corB.blue()  - corA.blue())  * t);
    const int a = static_cast<int>(corA.alpha() + (corB.alpha() - corA.alpha()) * t);

    return QColor(r, g, b, a);
}

void MenuButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setOpacity(opacityValue);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    const QRect area = rect().adjusted(0, 0, -1, -1);

    const bool desenharHover = hoverProgress > 0.01 || isDown() || pressProgress > 0.01;

    if (desenharHover) decorarBotao(painter, area);

    const QColor sombraNormal(0, 0, 0, 255);
    const QColor textoNormal(200, 200, 200);

    const QColor sombraHover(255, 255, 255, 65);
    const QColor textoHover(64, 69, 77);

    double progressoVisual = (hoverProgress > pressProgress) ? hoverProgress : pressProgress;

    const QColor sombraAtual = interpolarCor(sombraNormal, sombraHover, progressoVisual);
    const QColor textoAtual  = interpolarCor(textoNormal, textoHover, progressoVisual);

    const int pressShift = static_cast<int>(2.0 * pressProgress);
    const QPoint offset = contentOffset + QPoint(5, pressShift);

    painter.setPen(sombraAtual);
    painter.drawText(area.adjusted(26 + offset.x(), 0, -25, 2), textAlignment, text());

    painter.setPen(textoAtual);
    painter.drawText(area.adjusted(25 + offset.x(), 0, -25, -2), textAlignment, text());

    int alphaBordaNormal = static_cast<int>(255.0 * (1.0 - hoverProgress));

    // Se o botão for clicado, podemos ocultar para não chocar com o feedback visual
    if (isDown()) alphaBordaNormal = 0;

    if (alphaBordaNormal > 0) {
        painter.setPen(QPen(QColor(0, 0, 0, alphaBordaNormal), 6));
        painter.drawLine(area.left(), area.top(), area.right(), area.top());        // borda superior
        painter.drawLine(area.left(), area.bottom(), area.right(), area.bottom());  // borda inferior
    }

    // Borda animada no hover (sofrerá fade-out proporcional até sumir)
    if (hoverProgress > 0.001) {
        const QRectF r = QRectF(area).adjusted(2, 2, -2, -2);

        QConicalGradient grad(r.center(), borderAngle);
        // Multiplicar o Alpha por hoverProgress faz o gradiente sumir suavemente
        grad.setColorAt(0.00, QColor(0,   0,   0,   static_cast<int>(255 * hoverProgress)));
        grad.setColorAt(0.40, QColor(80,  80,  80,  static_cast<int>(160 * hoverProgress)));
        grad.setColorAt(0.70, QColor(180, 180, 180, static_cast<int>(60  * hoverProgress)));
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
void MenuButton::onHoverAnimationFinished() {
    // Se a animação terminou e o mouse não está mais em cima, desliga a rotação
    if (!hovered && getHoverProgress() < 0.01)
        borderAnimation->stop();
}
