#include "entryedit.h"
#include "ui_entryedit.h"
#include "listedit.h"

EntryEdit::EntryEdit() : ui(new Ui::EntryEdit)
{
    ui->setupUi(this);

    isValid_ = false;
    SetEntryStyle();

    connect(ui->addQuestionButton, SIGNAL(clicked()), this, SLOT(slAddQuestion()));
    connect(ui->removeQuestionButton, SIGNAL(clicked()), this, SLOT(slRemoveQuestion()));
    connect(ui->questionLineEdit1, SIGNAL(textChanged(const QString)), this, SLOT(slCheckValidity()));

    connect(ui->addAnswerButton, SIGNAL(clicked()), this, SLOT(slAddAnswer()));
    connect(ui->removeAnswerButton, SIGNAL(clicked()), this, SLOT(slRemoveAnswer()));
    connect(ui->answerLineEdit1, SIGNAL(textChanged(const QString)), this, SLOT(slCheckValidity()));
}

EntryEdit::~EntryEdit()
{
    delete ui;
}

void EntryEdit::ClearQuestions()
{
    QLayoutItem *item;

    while((item = ui->questionVLayout->itemAt(0)))
    {
        delete item;
    }
}

void EntryEdit::ClearAnswers()
{
    QLayoutItem *item;

    while((item = ui->answerVLayout->itemAt(0)))
    {
        delete item;
    }
}

const std::vector<QString> EntryEdit::GetAnswers() const
{
    std::vector<QString> answers;

    for(int i=0; i < ui->answerVLayout->count(); ++i)
    {
        QLineEdit *answer_line_edit = dynamic_cast<QLineEdit*>(ui->answerVLayout->takeAt(i));
        answers.push_back(answer_line_edit->text());
    }

    return answers;
}

const QString EntryEdit::GetHint(void) const
{
    return ui->hintLineEdit->text();
}

sEntryModel EntryEdit::GetModel() const
{
    sEntryModel model;
    model.questions_ = GetQuestions();
    model.answers_ = GetAnswers();
    model.hint_ = GetHint();
    return model;
}

const std::vector<QString> EntryEdit::GetQuestions() const
{
    std::vector<QString> questions;

    for(int i=0; i < ui->answerVLayout->count(); ++i)
    {
        QLayoutItem *question_item = ui->answerVLayout->takeAt(i);
        QLineEdit *question_line_edit = dynamic_cast<QLineEdit*>(question_item);
        questions.push_back(question_line_edit->text());
    }

    return questions;
}

void EntryEdit::InitAsListEditEntry(ListEdit *list_edit)
{
    if(nullptr != list_edit)
    {
        connect(ui->checkBox, SIGNAL(stateChanged(int)), list_edit, SLOT(slEntryToggled()));
        connect(ui->questionLineEdit1, SIGNAL(textChanged(const QString)), list_edit, SLOT(slModified()));
        connect(ui->answerLineEdit1, SIGNAL(textChanged(const QString)), list_edit, SLOT(slModified()));
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

void EntryEdit::SetAnswers(const std::vector<QString>& answers)
{
    ClearAnswers();

    for(const auto& answer : answers)
    {
        QLineEdit* new_line_edit = new QLineEdit();
        new_line_edit->setText(answer);

        ui->answerVLayout->addWidget(new_line_edit);
    }
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

void EntryEdit::SetQuestions(const std::vector<QString>& questions)
{
    ClearQuestions();

    for(const auto& question : questions)
    {
        QLineEdit* new_line_edit = new QLineEdit();
        new_line_edit->setText(question);

        ui->answerVLayout->addWidget(new_line_edit);
    }
}

void EntryEdit::SetModel(const sEntryModel& model)
{
    SetAnswers(model.answers_);
    SetHint(model.hint_);
    SetQuestions(model.questions_);
    return;
}

void EntryEdit::slAddAnswer()
{
    QLineEdit* new_line_edit = new QLineEdit();
    ui->answerVLayout->addWidget(new_line_edit);

    ViewChanged();
}

void EntryEdit::slAddQuestion()
{
    QLineEdit* new_line_edit = new QLineEdit();
    ui->questionVLayout->addWidget(new_line_edit);

    ViewChanged();
}

void EntryEdit::slRemoveAnswer()
{
    const auto answers_count = ui->answerVLayout->count();
    ui->answerVLayout->itemAt(answers_count - 1)->widget()->hide();
    auto* line_edit = ui->answerVLayout->takeAt(answers_count - 1);
    delete line_edit;

    ViewChanged();
}

void EntryEdit::slRemoveQuestion()
{
    const auto questions_count = ui->questionVLayout->count();
    ui->questionVLayout->itemAt(questions_count - 1)->widget()->hide();
    auto* line_edit = ui->questionVLayout->takeAt(questions_count - 1);
    delete line_edit;

    ViewChanged();
}

void EntryEdit::slCheckValidity()
{
    const size_t answers_count = ui->answerVLayout->count();
    const size_t questions_count = ui->questionVLayout->count();

    for(size_t i = 0; i < answers_count; ++i)
    {
        QLineEdit* line_edit = dynamic_cast<QLineEdit*>(ui->answerVLayout->itemAt(i));
        isValid_ &= !line_edit->text().isEmpty();
    }

    for(size_t i = 0; i < questions_count; ++i)
    {
        QLineEdit* line_edit = dynamic_cast<QLineEdit*>(ui->questionVLayout->itemAt(i));
        isValid_ &= !line_edit->text().isEmpty();
    }

    SetEntryStyle();

    return;
}

void EntryEdit::ViewChanged()
{
    const auto answers_count = ui->answerVLayout->count();
    const auto questions_count = ui->questionVLayout->count();

    ui->addAnswerButton->setEnabled(answers_count < 10);
    ui->removeAnswerButton->setEnabled(answers_count > 1);

    ui->addQuestionButton->setEnabled(questions_count < 10);
    ui->removeQuestionButton->setEnabled(questions_count > 1);
}
