#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include "qsfmlwidget.h"
#include "Animador_Fundo/animador_fundo.h"
#include "Gerenciador/Gerenciador_Grafico/Gerenciador_Textura/Gerenciador_Textura.h"

class BackgroundWidget : public QSFMLWidget
{
    Q_OBJECT

public:
    explicit BackgroundWidget(QWidget *parent = 0);

protected:
    virtual void OnInit();
    virtual void OnUpdate();

private:
    Gerenciador_Textura gerenciadorTextura;
    Animador_Fundo animadorFundo;
    sf::Vector2u ultimoTamanho;
    bool fundoCarregado;
};

#endif // BACKGROUNDWIDGET_H
