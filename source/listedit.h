#ifndef LISTEDIT_H
#define LISTEDIT_H

#include <QWidget>
#include <QCloseEvent>
#include <QAbstractSlider>
#include "settings.h"

namespace Ui {
class ListEdit;
}

class ListEdit : public QWidget
{
    Q_OBJECT

public:

    ListEdit(QWidget *parent, Settings *settings);
    ~ListEdit();

public slots:

    void slEntryToggled(void);
    void slModified(void);

private:

    Ui::ListEdit *ui;
    Settings *settings_;
    bool entriesAreChecked_;
    bool entriesAreModified_;
    bool entriesAreValid_;
    bool headerIsModified_;

private:

    bool AskForSave(void);
    void Clear(void);
    void closeEvent(QCloseEvent *event);
    void CheckEntriesValidity(void);
    void ViewChanged(void);

private slots:

    void slAdd(void);
    void slClear(void);
    void slDelete(void);
    void slEntriesChecked(int);
    void slInstructionsChanged(const QString);
    void slLoad(void);
    void slNameChanged(const QString);
    void slSave(void);
};

#endif // LISTEDIT_H
