//
// Created by Максим Сустаев on 16.07.2026.
//

#include "mainmenuwindow.h"

#include <QVBoxLayout>
#include <QMessageBox>

MainMenuWindow::MainMenuWindow(QWidget* parent) : QWidget(parent) {

    titleLabel = new QLabel("Pomodoro", this);
    titleLabel->setObjectName("titleLabel");

    timerButton = new QPushButton("Таймер", this);
    timerButton->setObjectName("primaryButton");
    tasksButton = new QPushButton("Задачи", this);
    statsButton = new QPushButton("Статистика", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 40, 30, 30);
    layout->setSpacing(15);
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(timerButton);
    layout->addWidget(tasksButton);
    layout->addWidget(statsButton);
    setLayout(layout);

    connect(timerButton, &QPushButton::clicked, this, &MainMenuWindow::timerButtonClicked);
    connect(tasksButton, &QPushButton::clicked, this, &MainMenuWindow::tasksButtonClicked);
    connect(statsButton, &QPushButton::clicked, this, &MainMenuWindow::statsButtonClicked);
}

void MainMenuWindow::onStubButtonClicked() {
    QMessageBox::information(this, "В разработке", "Эта функция еще не реализовона.");
}
