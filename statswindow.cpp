//
// Created by Максим Сустаев on 19.07.2026.
//

#include "statswindow.h"
#include <QVBoxLayout>
#include <fstream>
#include <sstream>

StatsWindow::StatsWindow(QWidget* parent) : QWidget(parent) {
    statsDisplay = new QTextEdit(this);
    statsDisplay->setReadOnly(true);

    clearButton = new QPushButton("Очистить статистику", this);
    backButton = new QPushButton("В меню", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);
    layout->addWidget(statsDisplay);
    layout->addWidget(clearButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(clearButton, &QPushButton::clicked, this, &StatsWindow::onClearClicked);
    connect(backButton, &QPushButton::clicked, this, &StatsWindow::backToMenuRequested);
}
void StatsWindow::loadStats() {
    std::ifstream file("pomodoro_stats.txt");
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    }
    if (buffer.str().empty()) {
        statsDisplay->setText("Пока нет завершенных циклов");
    } else {
        statsDisplay->setText(QString::fromStdString(buffer.str()));
    }
}
void StatsWindow::onClearClicked() {
    std::ofstream file("pomodoro_stats.txt", std::ios::trunc);
    file.close();
    loadStats();
}