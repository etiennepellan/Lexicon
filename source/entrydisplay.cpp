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
    const bool is_correct = (answer_ == ui->answerLineEdit->text());

    ui->answerLineEdit->setReadOnly(true);
    ui->answerLineEdit->setStyleSheet(is_correct ? "border: 1px solid green" : "border: 1px solid red");

    if(!is_correct)
    {
        QLabel *answerLabel = new QLabel(this);
        answerLabel->setText(answer_);
        answerLabel->setStyleSheet("color: red");

        ui->formLayout->addWidget(answerLabel);
    }

    return is_correct;
}

void EntryDisplay::SetAnswer(const QString& answer)
{
    answer_ = answer;
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
