//
// Created by Henrique on 04/07/2026.
//

#ifndef JOGO_RANKING_H
#define JOGO_RANKING_H

#include <QWidget>
#include "Gerenciador/Gerenciador_Estado/Caretaker.h"

class QTableWidget;
namespace Widgets {
    class Ranking: public QWidget {
        Q_OBJECT
        public:
            explicit Ranking(QWidget *parent = NULL);
            ~Ranking();
            static void loadRanking(QTableWidget* table);
            void atualizarTabela();
        signals:
            void voltarPressionado();
        private:
            static bool compararPontos(const Gerenciadores::Caretaker::DadosJogadorSalvo& a, const Gerenciadores::Caretaker::DadosJogadorSalvo& b);
    };
} // Widgets

#endif //JOGO_RANKING_H