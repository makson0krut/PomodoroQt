//
// Created by Максим Сустаев on 19.07.2026.
//

#ifndef QTHELLOWORLD_STATSWINDOW_H
#define QTHELLOWORLD_STATSWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

class StatsWindow : public QWidget {
    Q_OBJECT
public:
    StatsWindow(QWidget *parent = nullptr);
    void loadStats();
signals:
    void backToMenuRequested();
private slots:
    void onClearClicked();
private:
    QTextEdit* statsDisplay;
    QPushButton* clearButton;
    QPushButton* backButton;
};


#endif //QTHELLOWORLD_STATSWINDOW_H
