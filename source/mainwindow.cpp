#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listio.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings_(this),
    list_edit_(this, &settings_),
    about_(this)
{
    ui->setupUi(this);

    entries_displayed_ = false;

    settings_.LoadFromSystem();

    setWindowTitle(APP_NAME_STR);
    move(settings_.GetWindowPosition());
    resize(settings_.GetWindowSize());

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(slOpenFile()));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(slStart()));
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(slFinish()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(slOpenFile()));
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(slOpenPreferences()));
    connect(ui->actionEdit_3, SIGNAL(triggered()), this, SLOT(slEditList()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::CheckResults()
{
    const int entries_count = ui->entriesLayout->count();
    int correct_answer = 0;

    for(int i = 0; i < entries_count; i++)
    {
        EntryDisplay *entry = static_cast<EntryDisplay *>(ui->entriesLayout->itemAt(i)->widget());
        correct_answer += entry->IsCorrect();
    }

    return (correct_answer * 100 / entries_count);
}

void MainWindow::ClearEntries()
{
    QLayoutItem *item;

    while((item = ui->entriesLayout->takeAt(0)))
    {
        item->widget()->hide();
        delete item;
    }

    entries_displayed_ = false;

    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(settings_.IsAutoSaveEnabled()) settings_.SaveToSystem();
    event->accept();

    return;
}

void MainWindow::DisplayEntries()
{
    std::vector<sEntryModel> entries = entries_; // local copy needed for randomization bellow
    const size_t entries_max_display = settings_.GetEntriesMaximum();
    const size_t entries_size = entries.size();
    const size_t entries_displayed_count = (entries_size > entries_max_display) ? entries_max_display : entries_size; // display the maximum number of entries wanted by the user

    for(size_t i = 0; i < entries_displayed_count; i++)
    {
        size_t random_int;
        sEntryModel entry;

        if(settings_.IsOrderShuffleEnabled())
        {
            random_int = static_cast<size_t>(QRandomGenerator::global()->bounded(0, static_cast<int>(entries.size()))); // needs to be entries.size() because of the progressive erasing of vector

            entry = entries.at(random_int);
            entries.erase(entries.begin() + long(random_int));
        }
        else entry = entries.at(i);

        // picking a random question from entry
        random_int = static_cast<size_t>(QRandomGenerator::global()->bounded(0, static_cast<int>(entry.questions_.size())));

        EntryDisplay *entry_display = new EntryDisplay(this);
        entry_display->SetHint("<i>" + entry.hint_ + "<\\i>");
        entry_display->SetQuestion(entry.questions_.at(random_int));
        entry_display->SetAnswers(entry.answers_);

        if(settings_.IsSideShuffleEnabled())
        {
            random_int = static_cast<size_t>(QRandomGenerator::global()->bounded(0, 2));

            // if randomized, questions become answers and vice versa
            if(random_int)
            {
                // picking a random answer from entry
                random_int = static_cast<size_t>(QRandomGenerator::global()->bounded(0, static_cast<int>(entry.answers_.size())));
                entry_display->SetQuestion(entry.answers_.at(random_int));
                entry_display->SetAnswers(entry.questions_);
            }
        }

        ui->entriesLayout->addWidget(entry_display);
    }

    entries_displayed_ = true;

    return;
}

void MainWindow::DisplayError(const QString &error_msg)
{
    QMessageBox::critical(this, "Error", error_msg);
    return;
}

void MainWindow::DisplayResult(int result_percent)
{
    QString result_str =
            "You answered " +
            QString::number(result_percent) +
            "% of the questions correctly.";

    if(settings_.IsTimerEnabled())
    {
        result_str += "\nTime : " + QString::number((timer_.elapsed() / 1000.0), 'f', 1) + "s";
    }

    QMessageBox::information(this, "Result", result_str);

    return;
}

QPoint MainWindow::GetWindowPosition() const
{
    return pos();
}

QSize MainWindow::GetWindowSize() const
{
    return size();
}

bool MainWindow::LoadEntries(const QString &file_path)
{
    if(!entries_.empty())
    {
        entries_.clear(); // if not the new entries get loaded on top of the previous ones
    }

    ListIO::sListData list_data;
    QString error_msg;
    const bool load_ret = ListIO::Load(file_path, list_data, error_msg);

    if(load_ret)
    {
        ui->listLabel->setText(list_data.name_);
        ui->instructionsLabel->setText(list_data.instructions_);
        entries_ = list_data.entries_;
    }

    else DisplayError(error_msg);

    return load_ret;
}

void MainWindow::slAbout()
{
    if(about_.isHidden()) about_.show();
    else about_.hide();

    return;
}

void MainWindow::slEditList()
{
    list_edit_.show();
    return;
}

void MainWindow::slOpenFile()
{
    if(entries_displayed_)
    {
        static const QString warning_msg = "Opening another list will discard the current test.\nContinue ?";
        const QMessageBox::StandardButton answer = QMessageBox::question(this, "Open", warning_msg);

        if(QMessageBox::StandardButton::Yes == answer) ClearEntries();
        else return;
    }

    const QString last_file_path = settings_.IsUsingLastFilePath() ? settings_.GetLastOpenedFilePath() : "";
    const QString file_path = QFileDialog::getOpenFileName(this, "Open file", last_file_path, "JSON file (*.json)");

    if(!file_path.isEmpty())
    {
        settings_.SetLastOpenedFilePath(file_path);

        const bool loading_ret = LoadEntries(file_path);

        if(loading_ret) ui->startButton->setEnabled(true);
        else DisplayError("Invalid entry in " + file_path);
    }

    return;
}

void MainWindow::slOpenPreferences()
{
    settings_.show();
    return;
}

void MainWindow::slStart()
{
    if(entries_displayed_) ClearEntries();

    DisplayEntries();

    if(settings_.IsTimerEnabled())
    {
        timer_.start(); // counts time elapsed from this moment
        interval_timer_.start(100); // triggers connect every 100ms
    }

    ui->openButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->finishButton->setEnabled(true);

    return;
}

void MainWindow::slFinish()
{
    if(timer_.isValid()) interval_timer_.stop();

    const int results = CheckResults();
    DisplayResult(results);

    ui->openButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->finishButton->setEnabled(false);

    return;
}
