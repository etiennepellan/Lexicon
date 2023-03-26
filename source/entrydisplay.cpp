#include "entrydisplay.h"
#include "ui_entrydisplay.h"

EntryDisplay::EntryDisplay(
        QWidget *parent
    ) : QWidget(parent),
    ui(new Ui::EntryDisplay)
{
    ui->setupUi(this);
    ui->hintLabel->hide();
}

EntryDisplay::~EntryDisplay()
{
    delete ui;
}

bool EntryDisplay::IsCorrect()
{
    bool is_correct = false;

    for(const auto& answer : answers_)
    {
        is_correct += (answer == ui->answerLineEdit->text());
    }

    ui->answerLineEdit->setReadOnly(true);
    ui->answerLineEdit->setStyleSheet(is_correct ? "border: 1px solid green" : "border: 1px solid red");

    if(!is_correct)
    {
        QLabel *answerLabel = new QLabel(this);
        answerLabel->setText(std::accumulate(answers_.begin(), answers_.end(), QString(" ")));
        answerLabel->setStyleSheet("color: red");

        ui->formLayout->addWidget(answerLabel);
    }

    return is_correct;
}

void EntryDisplay::SetAnswers(const std::vector<QString>& answers)
{
    answers_ = answers;
    return;
}

void EntryDisplay::SetHint(const QString &hint)
{
     ui->hintLabel->setText(hint);
     ui->hintLabel->show();
    return;
}

void EntryDisplay::SetQuestion(const QString &question)
{
    ui->questionLabel->setText(question);
    return;
}
