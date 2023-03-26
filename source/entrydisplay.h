#ifndef ENTRYDISPLAY_H
#define ENTRYDISPLAY_H

#include <QWidget>

namespace Ui {
class EntryDisplay;
}

class EntryDisplay : public QWidget
{
    Q_OBJECT

    public:

        EntryDisplay(
            QWidget *parent//,
            //const QString question,
            //const QString answer
        );

        ~EntryDisplay();

        bool IsCorrect(void);
        void SetAnswers(const std::vector<QString>& answer);
        void SetHint(const QString& hint);
        void SetQuestion(const QString& question);

    private:

        Ui::EntryDisplay *ui;
        std::vector<QString> answers_;
};

#endif // ENTRYDISPLAY_H
