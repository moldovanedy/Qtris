#ifndef STATISTICS_VIEW_H
#define STATISTICS_VIEW_H

#include <QWidget>
#include <QBoxLayout>

#include "play_area.h"
#include "piece_manager.h"

namespace UI
{
    class StatisticsView : public QWidget
    {
        Q_OBJECT

    public:
        explicit StatisticsView(QWidget *parent = nullptr);
        ~StatisticsView();

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        std::map<UI::Resources::PieceType, QLabel *> *_labels =
            new std::map<UI::Resources::PieceType, QLabel *>();
        QString _currentFile;
    };
}

#endif // STATISTICS_VIEW_H