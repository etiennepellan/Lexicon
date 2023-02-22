#include "listedit.h"
#include "ui_listedit.h"
#include "entryedit.h"
#include "listio.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ListEdit::ListEdit(QWidget *parent, Settings *settings) :
    QWidget(parent, Qt::Window),
    ui(new Ui::ListEdit),
    settings_(settings)
{
    ui->setupUi(this);

    ui->addButton->setToolTip(ui->addButton->shortcut().toString());
    ui->deleteButton->setToolTip(ui->deleteButton->shortcut().toString());

    // to apply stylesheet
    slNameChanged(QString());
    slInstructionsChanged(QString());

    entriesAreModified_ = false;
    headerIsModified_ = false;

    connect(ui->nameLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(slNameChanged(const QString)));
    connect(ui->instructionsLineEdit, SIGNAL(textChanged(const QString)), this, SLOT(slInstructionsChanged(const QString)));
    connect(ui->entriesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slEntriesChecked(int)));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(slAdd()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(slClear()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(slDelete()));
    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(slLoad()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(slSave()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

ListEdit::~ListEdit()
{
    delete ui;
}

bool ListEdit::AskForSave()
{
    return (QMessageBox::StandardButton::Yes == QMessageBox::question(this, "Save", "Current list has been modified.\nSave ?"));
}

void ListEdit::CheckEntriesValidity()
{
    const int entries_count = ui->entriesLayout->count();

    for(int i = 0; i < entries_count; i++)
    {
         EntryEdit const *entry = static_cast<EntryEdit *>(ui->entriesLayout->itemAt(i)->widget());
         entriesAreValid_ = entry->IsValid();

         if(!entriesAreValid_) break;
    }

    ViewChanged();

    return;
}

void ListEdit::Clear()
{
    ui->nameLineEdit->clear();
    ui->instructionsLineEdit->clear();
    slEntriesChecked(true);
    slDelete();

    return;
}

void ListEdit::closeEvent(QCloseEvent *event)
{
    slClear();
    event->accept();

    return;
}

void ListEdit::slAdd()
{
    EntryEdit *entry = new EntryEdit();
    entry->InitAsListEditEntry(this);
    ui->entriesLayout->addWidget(entry);
    ui->entriesScrollArea->verticalScrollBar()->setSliderPosition(ui->entriesScrollArea->verticalScrollBar()->maximum());

    ViewChanged();

    return;
}

void ListEdit::slClear()
{
    if(entriesAreModified_ || headerIsModified_)
    {
        const bool save = AskForSave();
        if(save) slSave();
    }

    // in case user doesn't want to save
    entriesAreModified_ = false;
    headerIsModified_ = false;

    Clear();

    return;
}

void ListEdit::slDelete()
{
    for(int i = 0; i < ui->entriesLayout->count(); i++)
    {
        EntryEdit *entry = static_cast<EntryEdit *>(ui->entriesLayout->itemAt(i)->widget());

        if(entry->IsChecked())
        {
            ui->entriesLayout->itemAt(i)->widget()->hide();
            ui->entriesLayout->takeAt(i);
            i--;
        }
    }

    if(0 == ui->entriesLayout->count()) entriesAreValid_ = false;

    ui->entriesCheckBox->setChecked(false);
    ViewChanged();

    return;
}

void ListEdit::slEntriesChecked(int state)
{
    const int entries_count = ui->entriesLayout->count();

    for(int i = 0; i < entries_count; i++)
    {
        EntryEdit *entry = static_cast<EntryEdit *>(ui->entriesLayout->itemAt(i)->widget());
        entry->SetChecked(state);
    }

    ViewChanged();

    return;
}

void ListEdit::slEntryToggled()
{
    const int entries_count = ui->entriesLayout->count();

    entriesAreChecked_ = false; // if not, never goes back to false

    for(int i = 0; i < entries_count; i++)
    {
        EntryEdit *entry = static_cast<EntryEdit *>(ui->entriesLayout->itemAt(i)->widget());
        entriesAreChecked_ += entry->IsChecked();
    }

    ViewChanged();

    return;
}

void ListEdit::slInstructionsChanged(const QString instructions)
{
    const QString stylesheet = instructions.isEmpty() ? "border: 1px solid rgba(255,170,0,137)" : "";
    ui->instructionsLineEdit->setStyleSheet(stylesheet);

    headerIsModified_ = true;
    ViewChanged();

    return;
}

void ListEdit::slLoad()
{
    const QString last_file_path = settings_->IsUsingLastFilePath() ? settings_->GetLastOpenedFilePath() : "";
    const QString file_path = QFileDialog::getOpenFileName(this, "Open file", last_file_path, "JSON file (*.json)");

    if(!file_path.isEmpty())
    {
        settings_->SetLastOpenedFilePath(file_path);

        if(entriesAreModified_ || headerIsModified_)
        {
            const bool save = AskForSave();
            if(save) slSave();
        }

        else entriesAreModified_ = false;

        Clear();

        ListIO::sListData list_data;
        QString error_msg;
        const bool load_ret = ListIO::Load(file_path, list_data, error_msg);

        if(load_ret)
        {
            ui->nameLineEdit->setText(list_data.name_);
            ui->instructionsLineEdit->setText(list_data.instructions_);
            headerIsModified_ = true;

            for(const auto& entry_model : list_data.entries_)
            {
                EntryEdit *entry_edit = new EntryEdit();
                entry_edit->SetModel(entry_model);
                entry_edit->InitAsListEditEntry(this);
                ui->entriesLayout->addWidget(entry_edit);
            }

            entriesAreModified_ = true;

            CheckEntriesValidity();
            ViewChanged();
        }

        else QMessageBox::critical(this, "Loading error", error_msg);
    }

    return;
}

void ListEdit::slModified()
{
    entriesAreModified_ = true;
    CheckEntriesValidity();
    ViewChanged();

    return;
}

void ListEdit::slNameChanged(const QString name)
{
    const QString stylesheet = name.isEmpty() ? "border: 1px solid rgba(255,170,0,137)" : "";
    ui->nameLineEdit->setStyleSheet(stylesheet);

    headerIsModified_ = true;
    ViewChanged();

    return;
}

void ListEdit::slSave()
{
    const QString last_file_path = settings_->IsUsingLastFilePath() ? settings_->GetLastOpenedFilePath() : "";
    const QString file_path = QFileDialog::getSaveFileName(this, "Select file", last_file_path, "JSON file (*.json)");

    if(!file_path.isEmpty())
    {
        settings_->SetLastOpenedFilePath(file_path);

        ListIO::sListData list_data;

        list_data.instructions_ = ui->instructionsLineEdit->text();
        list_data.name_ = ui->nameLineEdit->text();

        const int entries_count = ui->entriesLayout->count();

        for(int i = 0; i < entries_count; i++)
        {
            EntryEdit *entry = static_cast<EntryEdit *>(ui->entriesLayout->itemAt(i)->widget());
            list_data.entries_.push_back(entry->GetModel());
        }

        QString error_msg;
        const bool save_ret = ListIO::Save(file_path, list_data, error_msg);

        if(save_ret)
        {
            entriesAreModified_ = false;
            headerIsModified_ = false;
            ViewChanged();
        }

        else QMessageBox::critical(this, "Saving error", error_msg);
    }

    return;
}

void ListEdit::ViewChanged()
{
    const int entries_count = ui->entriesLayout->count();

    ui->addButton->setEnabled(entries_count < 100);
    ui->deleteButton->setEnabled(entries_count && entriesAreChecked_);
    ui->saveButton->setEnabled(
        entries_count &&
        (entriesAreModified_ || headerIsModified_) &&
        entriesAreValid_ &&
        !ui->instructionsLineEdit->text().isEmpty() &&
        !ui->nameLineEdit->text().isEmpty()
    );

    return;
}
