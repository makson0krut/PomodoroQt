//
// Created by Максим Сустаев on 16.07.2026.
//

#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class MainMenuWindow : public QWidget {
    Q_OBJECT
public:
    MainMenuWindow(QWidget* parent = nullptr);
signals:
    void timerButtonClicked();
    void statsButtonClicked();
    void tasksButtonClicked();
private slots:
    void onStubButtonClicked();
private:
    QPushButton* timerButton;
    QPushButton* tasksButton;
    QPushButton* statsButton;
    QLabel* titleLabel;
};



#endif //MAINMENUWINDOW_H
