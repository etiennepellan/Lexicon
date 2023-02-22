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
    EntryEdit(EntryModel& model);
    ~EntryEdit();

    const QString GetAnswer(void) const;
    const QString GetHint(void) const;
    const QString GetQuestion(void) const;
    void InitAsListEditEntry(ListEdit *list_edit);
    bool IsChecked(void) const;
    bool IsValid(void) const;
    void SetAnswer(const QString& answer);
    void SetChecked(bool checked);
    void SetHint(const QString& hint);
    void SetQuestion(const QString& question);

    EntryModel GetModel() const;
    void SetModel(const EntryModel& model);

private:

    Ui::EntryEdit *ui;
    bool isValid_;

private:

    void SetEntryStyle(void);

private slots:

    void slCheckValidity(void);
};

#endif // ENTRYEDIT_H
