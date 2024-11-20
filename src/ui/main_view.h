#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QResizeEvent>

#include <QDebug>
#include <QSizePolicy>

#include "menu_bar.h"

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

    QStackedWidget *getStackPanel();

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    MenuBar *_menuBar;

    QStackedWidget *mainStackPanel;
    QBoxLayout *_verticalContainer;
    QBoxLayout *_horizontalContainer;
    QWidget *_mainContent;
    QString _currentFile;
};

#endif // MAIN_VIEW_H
