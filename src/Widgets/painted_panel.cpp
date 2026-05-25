#include "painted_panel.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPainterPath>

PaintedPanel::PaintedPanel(QWidget *parent)
    : QFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void PaintedPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF frameRect = rect().adjusted(8, 8, -8, -8);
    const qreal radius = 18.0;

    // Sombra externa suave
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 55));
    painter.drawRoundedRect(frameRect.adjusted(6, 6, 6, 6), radius, radius);

    // Fundo em gradiente branco/cinza suave
    QLinearGradient fillGradient(frameRect.topLeft(), frameRect.bottomLeft());
    fillGradient.setColorAt(0.0,  QColor(255, 255, 255, 252));
    fillGradient.setColorAt(0.45, QColor(239, 239, 239, 250));
    fillGradient.setColorAt(1.0,  QColor(204, 204, 204, 248));

    painter.setPen(Qt::NoPen);
    painter.setBrush(fillGradient);
    painter.drawRoundedRect(frameRect, radius, radius);

    // Moldura externa preta
    painter.setPen(QPen(QColor(0, 0, 0, 220), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(frameRect.adjusted(1, 1, -1, -1), radius - 1.0, radius - 1.0);

    // Molde interno claro para dar profundidade
    painter.setPen(QPen(QColor(255, 255, 255, 120), 1));
    painter.drawRoundedRect(frameRect.adjusted(5, 5, -5, -5), radius - 4.0, radius - 4.0);

    // Linhas horizontais decorativas com contraste preto/cinza
    const qreal cx = frameRect.center().x();
    const qreal margem = frameRect.width() * 0.14;

    // linha topo
    QLinearGradient linhaTop(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
    linhaTop.setColorAt(0.0,  QColor(0, 0, 0, 0));
    linhaTop.setColorAt(0.3,  QColor(0, 0, 0, 110));
    linhaTop.setColorAt(0.5,  QColor(80, 80, 80, 160));
    linhaTop.setColorAt(0.7,  QColor(0, 0, 0, 110));
    linhaTop.setColorAt(1.0,  QColor(0, 0, 0, 0));
    painter.setPen(QPen(QBrush(linhaTop), 1));
    painter.drawLine(QPointF(frameRect.left() + margem, frameRect.top() + 14),
                     QPointF(frameRect.right() - margem, frameRect.top() + 14));

    // linha base
    QLinearGradient linhaBase(frameRect.left() + margem, 0, frameRect.right() - margem, 0);
    linhaBase.setColorAt(0.0,  QColor(0, 0, 0, 0));
    linhaBase.setColorAt(0.3,  QColor(0, 0, 0, 90));
    linhaBase.setColorAt(0.5,  QColor(60, 60, 60, 140));
    linhaBase.setColorAt(0.7,  QColor(0, 0, 0, 90));
    linhaBase.setColorAt(1.0,  QColor(0, 0, 0, 0));
    painter.setPen(QPen(QBrush(linhaBase), 1));
    painter.drawLine(QPointF(frameRect.left() + margem, frameRect.bottom() - 14),
                     QPointF(frameRect.right() - margem, frameRect.bottom() - 14));

    // Losango central ornamental em preto/cinza
    const qreal cy = frameRect.top() + 14;
    QPolygonF losango;
    losango << QPointF(cx,     cy - 4)
            << QPointF(cx + 4, cy    )
            << QPointF(cx,     cy + 4)
            << QPointF(cx - 4, cy    );

    painter.setPen(QPen(QColor(0, 0, 0, 180), 1));
    painter.setBrush(QColor(180, 180, 180, 110));
    painter.drawPolygon(losango);
}
