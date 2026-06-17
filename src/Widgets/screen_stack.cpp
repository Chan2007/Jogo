#include "screen_stack.h"

namespace Widgets {
    ScreenStack::ScreenStack(QStackedWidget *container)
: stackedWidget(container)
, historico()
    {}

    void ScreenStack::setContainer(QStackedWidget *container) {
        stackedWidget = container;
        historico.clear();
    }

    void ScreenStack::setInitialScreen(QWidget *screen) {
        if (!stackedWidget || !screen) return;

        historico.clear();
        historico.push_back(screen);
        stackedWidget->setCurrentWidget(screen);
    }

    void ScreenStack::pushScreen(QWidget *screen) {
        if (!stackedWidget || !screen) return;

        if (!historico.isEmpty() && historico.back() == screen) {
            stackedWidget->setCurrentWidget(screen);
            return;
        }

        historico.push_back(screen);
        stackedWidget->setCurrentWidget(screen);
    }

    void ScreenStack::popScreen() {
        if (!stackedWidget || historico.size() <= 1) return;

        historico.pop_back();
        stackedWidget->setCurrentWidget(historico.back());
    }

    QWidget* ScreenStack::currentScreen() const {
        if (historico.isEmpty()) return 0;

        return historico.back();
    }
}
