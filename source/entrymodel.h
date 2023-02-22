#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H

#include <QString>

class EntryModel
{
public:

    EntryModel() {};

    const QString GetAnswer() const { return answer_; };
    const QString GetHint() const { return hint_; };
    const QString GetQuestion() const { return question_; };
    void SetAnswer(const QString &answer) { answer_ = answer; }
    void SetHint(const QString &hint) { hint_ = hint; }
    void SetQuestion(const QString &question) { question_ = question; }

private:

    QString answer_;
    QString hint_;
    QString question_;
};

#endif // ENTRYMODEL_H
