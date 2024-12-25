#include "high_scores.h"

using namespace UI::Menus;

HighScores::HighScores(QWidget *parent) {
    this->setStyleSheet("background-image: url();");
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    QBoxLayout *verticalBoxContainer = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    verticalBoxContainer->setSpacing(0);
    verticalBoxContainer->setContentsMargins(0, 0, 0, 0);

    // add to verticalContainer
    {
        QWidget *topSpacer = new QWidget();
        verticalBoxContainer->addWidget(topSpacer, 1);

        QBoxLayout *horizontalContainer = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        horizontalContainer->setSpacing(0);
        horizontalContainer->setContentsMargins(0, 0, 0, 0);

        // add to horizontalContainer
        {
            QWidget *leftSpacer = new QWidget();
            horizontalContainer->addWidget(leftSpacer, 1);

            horizontalContainer->addLayout(this->getMainContent());

            QWidget *rightSpacer = new QWidget();
            horizontalContainer->addWidget(rightSpacer, 1);

            verticalBoxContainer->addLayout(horizontalContainer);
        }

        QWidget *bottomSpacer = new QWidget();
        verticalBoxContainer->addWidget(bottomSpacer, 1);
    }
}

HighScores::~HighScores() {

}

QVBoxLayout *HighScores::getMainContent() {
    QVBoxLayout *root = new QVBoxLayout();
    root->setSpacing(15);

    QLabel *title = new QLabel("High scores", this);
    title->setFont(MainWindow::getInstance()->getAppFont());
    title->setStyleSheet("font-size: 36px;");
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    //---------- TABLE ----------
    QTableWidget *table = new QTableWidget(1, 5, this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setFont(MainWindow::getInstance()->getAppFont());
    table->setHorizontalHeaderLabels({ "Name", "Score", "Lines", "Starting level", "Date achieved" });
    table->setMinimumWidth(550);

    root->addWidget(table);


    //---------- BUTTONS ----------
    QBoxLayout *actionBar = new QHBoxLayout();
    actionBar->setAlignment(Qt::AlignRight);
    actionBar->setSpacing(15);

    //add the buttons
    {
        QPushButton *resetScoresButton = new QPushButton(this);
        resetScoresButton->setFont(MainWindow::getInstance()->getAppFont());
        resetScoresButton->setText("Reset scores");
        actionBar->addWidget(resetScoresButton);

        QPushButton *goBackButton = new QPushButton(this);
        goBackButton->setFont(MainWindow::getInstance()->getAppFont());
        goBackButton->setText("Go back");
        actionBar->addWidget(goBackButton);
    }
    root->addLayout(actionBar);

    return root;
}