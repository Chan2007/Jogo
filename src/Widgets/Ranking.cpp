//
// Created by Henrique on 04/07/2026.
//

#include "Ranking.h"

#include <QFont>
#include <QHeaderView>
#include <QTableWidgetItem>

namespace Widgets {
    bool Ranking::compararPontos(const Gerenciadores::Caretaker::DadosJogadorSalvo& a, const Gerenciadores::Caretaker::DadosJogadorSalvo& b) {
        return a.pontos > b.pontos;
    }

    void Ranking::loadRanking(QTableWidget* table) {
        if (!table) return;
        const int LIMITE_RANKING = 10;

        Gerenciadores::Caretaker caretaker;
        std::vector<Gerenciadores::Caretaker::DadosJogadorSalvo> dados = caretaker.carregarDeArquivoTXT("ranking.txt");

        std::sort(dados.begin(), dados.end(), compararPontos);
        if (static_cast<int>(dados.size()) > LIMITE_RANKING)
            dados.resize(LIMITE_RANKING);

        table->setRowCount(0);
        table->setColumnCount(3);

        QFont fonteConteudo("Segoe UI", 14);
        table->setFont(fonteConteudo);

        QFont fonteCabecalho("Segoe UI", 14, QFont::Bold);
        table->horizontalHeader()->setFont(fonteCabecalho);

        table->verticalHeader()->setDefaultSectionSize(44);
        table->verticalHeader()->setVisible(false);

        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

        for (int i = 0; i < static_cast<int>(dados.size()); ++i) {
            table->insertRow(i);

            QTableWidgetItem* itemFase = new QTableWidgetItem(QString::number(dados[i].fase));
            itemFase->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, 0, itemFase);

            QTableWidgetItem* itemNome = new QTableWidgetItem(QString::fromStdString(dados[i].nome));
            itemNome->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            table->setItem(i, 1, itemNome);

            QTableWidgetItem* itemPontos = new QTableWidgetItem(QString::number(static_cast<int>(dados[i].pontos)));
            itemPontos->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, 2, itemPontos);
        }
    }
} // Widgets