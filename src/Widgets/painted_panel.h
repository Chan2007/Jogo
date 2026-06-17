#ifndef PAINTED_PANEL_H
#define PAINTED_PANEL_H

#include <QFrame>

namespace Widgets {
    class PaintedPanel : public QFrame {
        Q_OBJECT

    public:
        explicit PaintedPanel(QWidget *parent = NULL);
    protected:
        virtual void paintEvent(QPaintEvent *event);
    };
}

#endif // PAINTED_PANEL_H
