//
// Created by Максим Сустаев on 16.07.2026.
//

#include "mainmenuwindow.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainMenuWindow::MainMenuWindow(QWidget* parent) : QWidget(parent) {
    timerButton = new QPushButton("Таймер", this);
    tasksButton = new QPushButton("Задачи", this);
    statsButton = new QPushButton("Статичтика", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(timerButton);
    layout->addWidget(tasksButton);
    layout->addWidget(statsButton);
    setLayout(layout);

    connect(timerButton, &QPushButton::clicked, this, &MainMenuWindow::timerButtonClicked);
    connect(tasksButton, &QPushButton::clicked, this, &MainMenuWindow::onStubButtonClicked);
    connect(statsButton, &QPushButton::clicked, this, &MainMenuWindow::onStubButtonClicked);
}

void MainMenuWindow::onStubButtonClicked() {
    QMessageBox::information(this, "В разработке", "Эта функция еще не реализовона.");
}
