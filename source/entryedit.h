#ifndef ENTRYEDIT_H
#define ENTRYEDIT_H

#include <QWidget>
#include "listedit.h"
#include "entrymodel.h"

namespace Ui {
class EntryEdit;
}

class EntryEdit : public QWidget
{
    Q_OBJECT

public:

    EntryEdit();
    EntryEdit(const sEntryModel& model);
    ~EntryEdit();

    const std::vector<QString> GetAnswers() const;
    const QString GetHint(void) const;
    const std::vector<QString> GetQuestions() const;
    void InitAsListEditEntry(ListEdit *list_edit);
    bool IsChecked(void) const;
    bool IsValid(void) const;
    void SetAnswers(const std::vector<QString>& answers);
    void SetChecked(bool checked);
    void SetHint(const QString& hint);
    void SetQuestions(const std::vector<QString>& questions);

    sEntryModel GetModel() const;
    void SetModel(const sEntryModel& model);

private:

    Ui::EntryEdit *ui;
    bool isValid_;

private:

    void ClearQuestions();
    void ClearAnswers();
    void SetEntryStyle();
    void ViewChanged();

private slots:

    void slAddAnswer();
    void slAddQuestion();
    void slRemoveAnswer();
    void slRemoveQuestion();
    void slCheckValidity(void);
};

#endif // ENTRYEDIT_H
