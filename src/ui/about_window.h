#include <QDialog>

namespace UI {
    class AboutWindow : public QDialog
    {
        Q_OBJECT

    public:
        explicit AboutWindow();
        ~AboutWindow();

        void showDialog();
    };
}