#include "menu_button.h"
#include <QEvent>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QTimer>

MenuButton::MenuButton(const QString& text, QWidget *parent)
    : QPushButton(text, parent)
    , hovered(false)
    , m_hoverProgress(0.0)
    , m_pressProgress(0.0)
    , m_contentOffset(QPoint(-18, 0))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_hoverAnimation(new QPropertyAnimation(this, "hoverProgress", this))
    , m_pressAnimation(new QPropertyAnimation(this, "pressProgress", this))
    , m_offsetAnimation(new QPropertyAnimation(this, "contentOffset", this))
    , m_opacityAnimation(new QPropertyAnimation(m_opacityEffect, "opacity", this))
{
    QFont botaoFont("Palatino Linotype");
    if (!botaoFont.exactMatch())
        botaoFont = QFont("Book Antiqua");
    if (!botaoFont.exactMatch())
        botaoFont = QFont("Georgia");

    botaoFont.setPointSize(18);
    botaoFont.setWeight(QFont::Normal);
    setFont(botaoFont);

    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    setMouseTracking(true);
    setMinimumHeight(100);

    // Suprime completamente a renderização nativa do QPushButton
    // (texto, borda e fundo padrão do QStyle) para que apenas
    // o paintEvent customizado desenhe o conteúdo do botão.
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setStyleSheet("MenuButton { color: transparent; background: transparent; border: none; }");

    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(0.0);

    m_hoverAnimation->setDuration(170);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_pressAnimation->setDuration(90);
    m_pressAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_offsetAnimation->setDuration(220);
    m_offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_opacityAnimation->setDuration(280);
    m_opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

QSize MenuButton::sizeHint() const
{
    return QSize(320, 1000);
}

void MenuButton::iniciarAnimacaoEntrada(int atrasoMs)
{
    if (atrasoMs < 0)
        atrasoMs = 0;

    QTimer::singleShot(atrasoMs, this, SLOT(update()));
    QTimer::singleShot(atrasoMs, m_opacityAnimation, SLOT(stop()));
    QTimer::singleShot(atrasoMs, m_offsetAnimation, SLOT(stop()));

    m_opacityAnimation->setStartValue(m_opacityEffect->opacity());
    m_opacityAnimation->setEndValue(1.0);

    m_offsetAnimation->setStartValue(QPoint(-18, 0));
    m_offsetAnimation->setEndValue(QPoint(0, 0));

    QTimer::singleShot(atrasoMs, m_opacityAnimation, SLOT(start()));
    QTimer::singleShot(atrasoMs, m_offsetAnimation, SLOT(start()));
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

void MenuButton::enterEvent(QEvent *event)
{
    hovered = true;
    animarHover(1.0);
    QPushButton::enterEvent(event);
}

void MenuButton::leaveEvent(QEvent *event)
{
    hovered = false;
    animarHover(0.0);
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
    const QRect strokeRect = area.adjusted(8, 10, -52, -10);
    const int alphaBase = 80 + static_cast<int>(160.0 * m_hoverProgress);
    const int alphaGlow = 22 + static_cast<int>(70.0 * m_hoverProgress);
    const QColor baseColor = isDown() ? QColor(236, 237, 232, 245) : QColor(244, 245, 240, alphaBase);
    const QColor edgeColor(255, 255, 255, 70 + static_cast<int>(80.0 * m_hoverProgress));

    painter.save();
    painter.setPen(Qt::NoPen);

    painter.setBrush(baseColor);
    painter.drawRoundedRect(strokeRect.adjusted(10, 2, -18, -2), 9, 9);
    painter.drawRoundedRect(strokeRect.adjusted(0, 6, -2, -6), 7, 7);
    painter.drawRoundedRect(strokeRect.adjusted(16, 0, 0, 0), 8, 8);

    painter.setBrush(QColor(255, 255, 255, alphaGlow));
    painter.drawRoundedRect(strokeRect.adjusted(12, 8, -40, -12), 6, 6);

    painter.setPen(QPen(edgeColor, 1));
    painter.drawLine(strokeRect.left() + 12, strokeRect.top() + 3, strokeRect.right() - 36, strokeRect.top() + 3);
    painter.drawLine(strokeRect.left() + 5, strokeRect.bottom() - 3, strokeRect.right() - 18, strokeRect.bottom() - 4);

    painter.setBrush(QColor(231, 191, 87, 215));
    static const QPoint marcador[] = {
        QPoint(14, 0),
        QPoint(0, 8),
        QPoint(14, 16),
        QPoint(10, 8)
    };

    QPolygon polygon;
    for (int i = 0; i < 4; ++i)
        polygon << QPoint(strokeRect.left() - 20 + marcador[i].x(), strokeRect.center().y() - 8 + marcador[i].y());

    painter.drawPolygon(polygon);
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
    // Garante que o progresso fique estritamente entre 0.0 e 1.0
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    // Interpolação linear clássica para cada canal (Red, Green, Blue, Alpha)
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

    const QRect area = rect().adjusted(0, 0, -1, -1);
    const bool desenharHover = (m_hoverProgress > 0.01) || isDown() || (m_pressProgress > 0.01);

    if (desenharHover)
        desenharPincelada(painter, area);

    // Definição das cores usando construtores explícitos (C++03 não aceita inicialização por chaves {})
    const QColor sombraNormal(255, 255, 255, 130);
    const QColor textoNormal(0, 0, 0);

    const QColor sombraHover(255, 255, 255, 65);
    const QColor textoHover(64, 69, 77);

    // Substituição do qMax por uma lógica simples inline (compatível com qualquer versão do Qt)
    double progressoVisual = (m_hoverProgress > m_pressProgress) ? m_hoverProgress : m_pressProgress;

    // Chamada do méthodo da classe para calcular as cores intermediárias
    const QColor sombraAtual = interpolarCor(sombraNormal, sombraHover, progressoVisual);
    const QColor textoAtual  = interpolarCor(textoNormal, textoHover, progressoVisual);

    const int pressShift = static_cast<int>(2.0 * m_pressProgress);
    const QPoint offset = m_contentOffset + QPoint(0, pressShift);

    // Renderização com os valores interpolados
    painter.setPen(sombraAtual);
    painter.drawText(area.adjusted(25 + offset.x(), 25 + offset.y(), -18, 0), Qt::AlignVCenter | Qt::AlignLeft, text());

    painter.setPen(textoAtual);
    painter.drawText(area.adjusted(25 + offset.x(), 25 + offset.y(), -18, -2), Qt::AlignVCenter | Qt::AlignLeft, text());
}
