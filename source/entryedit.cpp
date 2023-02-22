#include "entryedit.h"
#include "ui_entryedit.h"
#include "listedit.h"

EntryEdit::EntryEdit() : ui(new Ui::EntryEdit)
{
    ui->setupUi(this);

    isValid_ = false;
    SetEntryStyle();

    connect(ui->questionLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(slCheckValidity()));
    connect(ui->answerLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(slCheckValidity()));
}

EntryEdit::~EntryEdit()
{
    delete ui;
}

const QString EntryEdit::GetAnswer(void) const
{
    return ui->answerLineEdit->text();
}

const QString EntryEdit::GetHint(void) const
{
    return ui->hintLineEdit->text();
}

EntryModel EntryEdit::GetModel() const
{
    EntryModel model;
    model.SetAnswer(GetAnswer());
    model.SetHint(GetHint());
    model.SetQuestion(GetQuestion());
    return model;
}

const QString EntryEdit::GetQuestion(void) const
{
    return ui->questionLineEdit->text();
}

void EntryEdit::InitAsListEditEntry(ListEdit *list_edit)
{
    if(nullptr != list_edit)
    {
        connect(ui->checkBox, SIGNAL(stateChanged(int)), list_edit, SLOT(slEntryToggled()));
        connect(ui->questionLineEdit, SIGNAL(textChanged(const QString)), list_edit, SLOT(slModified()));
        connect(ui->answerLineEdit, SIGNAL(textChanged(const QString)), list_edit, SLOT(slModified()));
        connect(ui->hintLineEdit, SIGNAL(textChanged(const QString)), list_edit, SLOT(slModified()));
    }

    return;
}

bool EntryEdit::IsChecked(void) const
{
    return ui->checkBox->isChecked();
}

bool EntryEdit::IsValid(void) const
{
    return isValid_;
}

void EntryEdit::SetAnswer(const QString& answer)
{
    ui->answerLineEdit->setText(answer);
    return;
}

void EntryEdit::SetChecked(bool checked)
{
    ui->checkBox->setChecked(checked);
    return;
}

void EntryEdit::SetEntryStyle()
{
    const QString stylesheet = isValid_ ? "" : ".QFrame { border: 1px solid rgba(255,170,0,137); }";
    setStyleSheet(stylesheet);

    return;
}

void EntryEdit::SetHint(const QString& hint)
{
    ui->hintLineEdit->setText(hint);
    return;
}

void EntryEdit::SetQuestion(const QString& question)
{
    ui->questionLineEdit->setText(question);
    return;
}

void EntryEdit::SetModel(const EntryModel& model)
{
    SetAnswer(model.GetAnswer());
    SetHint(model.GetHint());
    SetQuestion(model.GetQuestion());
    return;
}

void EntryEdit::slCheckValidity()
{
    isValid_ = !ui->questionLineEdit->text().isEmpty() && !ui->answerLineEdit->text().isEmpty();
    SetEntryStyle();

    return;
}
