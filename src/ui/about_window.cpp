#include "about_window.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace UI;

AboutWindow::AboutWindow() : QDialog() {
    this->setFixedSize(450, 300);
    this->setModal(true);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

    QLabel *titleLabel = new QLabel("Qtris", this);
    titleLabel->setStyleSheet("font-size: 24px");
    titleLabel->setAlignment(Qt::AlignHCenter);
    layout->addWidget(titleLabel);

    QLabel *descriptionLabel = new QLabel(
        "This project is an exercise trying to implement in the best way possible the game-play for the original Tetris NES. \
Only the mechanics are implemented with fidelity, as all the menus are custom, as well as the design of the play area.\n\
License: MIT",
this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(descriptionLabel);

    layout->addStretch(1);

    QPushButton *okButton = new QPushButton("OK", this);
    this->connect(okButton, &QPushButton::clicked, this, [=]() {
        this->close();
        });
    layout->addWidget(okButton);

    this->setLayout(layout);
}

AboutWindow::~AboutWindow() {}

void AboutWindow::showDialog() {
    this->exec();
}