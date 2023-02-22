#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::Settings),
    sys_settings_(getprogname())
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    connect(ui->enableTimerCheckbox, SIGNAL(stateChanged(int)), this, SLOT(slEnableTimer(int)));
    connect(ui->useLastOpenedCheckbox, SIGNAL(stateChanged(int)), this, SLOT(slUseLastPathToggled(int)));
    connect(ui->preferedPathButton, SIGNAL(clicked()), this, SLOT(slPreferedPath()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->defaultButton, SIGNAL(clicked()), this, SLOT(slSetDefault()));
    connect(ui->clearCacheButton, SIGNAL(clicked()), this, SLOT(slClearCache()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::closeEvent(QCloseEvent *event)
{
    UpdateModel();
    event->accept();

    return;
}

// if the configuration is saved on system, download and apply it
void Settings::LoadFromSystem()
{
    file_prefered_path_ = sys_settings_.contains(str_file_prefered_path_) ?
        sys_settings_.value(str_file_prefered_path_).toString() : default_prefered_path_;

    file_use_last_path_ = sys_settings_.contains(str_file_use_last_path_) ?
        sys_settings_.value(str_file_use_last_path_).toBool() : default_use_last_path_;

    file_last_path_ = sys_settings_.contains(str_file_last_path_) ?
        sys_settings_.value(str_file_last_path_).toString() : default_last_path_;

    entries_maximum_ = sys_settings_.contains(str_entries_maximum_) ?
        sys_settings_.value(str_entries_maximum_).toInt() : default_entries_maximum_;

    entries_order_shuffle_ = sys_settings_.contains(str_entries_order_shuffle_) ?
        sys_settings_.value(str_entries_order_shuffle_).toBool() : default_order_shuffle_;

    entries_side_shuffle_ = sys_settings_.contains(str_entries_side_shuffle_) ?
        sys_settings_.value(str_entries_side_shuffle_).toBool() : default_side_shuffle_;

    window_size_ = sys_settings_.contains(str_window_size_) ?
        sys_settings_.value(str_window_size_).toSize() : default_window_size_;

    window_pos_ = sys_settings_.contains(str_window_pos_) ?
        sys_settings_.value(str_window_pos_).toPoint() : default_window_pos_;

    test_timer_enabled_ = sys_settings_.contains(str_test_timer_enabled_) ?
        sys_settings_.value(str_test_timer_enabled_).toBool() : default_test_timer_enabled_;

    auto_save_ = sys_settings_.contains(str_auto_save_) ?
        sys_settings_.value(str_auto_save_).toBool() : default_auto_save_;

    UpdateView();

    return;
}

void Settings::SaveToSystem()
{
    mw = qobject_cast<MainWindow *>(this->parent());
    window_size_ = mw->GetWindowSize();
    window_pos_ = mw->GetWindowPosition();

    sys_settings_.setValue(str_file_prefered_path_, file_prefered_path_);
    sys_settings_.setValue(str_file_use_last_path_, file_use_last_path_);
    sys_settings_.setValue(str_file_last_path_, file_last_path_);
    sys_settings_.setValue(str_entries_maximum_, entries_maximum_);
    sys_settings_.setValue(str_entries_order_shuffle_, entries_order_shuffle_);
    sys_settings_.setValue(str_entries_side_shuffle_, entries_side_shuffle_);
    sys_settings_.setValue(str_window_size_, window_size_);
    sys_settings_.setValue(str_window_pos_, window_pos_);
    sys_settings_.setValue(str_test_timer_enabled_, test_timer_enabled_);
    sys_settings_.setValue(str_auto_save_, auto_save_);
}

void Settings::SetLastOpenedFilePath(const QString file_path)
{
    file_last_path_ = file_path;
    return;
}

void Settings::slClearCache()
{
    QMessageBox::StandardButton answer = QMessageBox::question(
        this,
        "Clear cache",
        "This action will delete Lexicon's settings saved on your system.\nContinue ?"
    );

    if(QMessageBox::StandardButton::Yes == answer)
    {
        sys_settings_.clear();

        QMessageBox::information(this, "Clear cache", "Cache has been cleared.");
    }

    return;
}

void Settings::slEnableTimer(int checked)
{
    test_timer_enabled_ = checked;
    return;
}

void Settings::slPreferedPath()
{
    const QString path = QFileDialog::getOpenFileName(this, "Open", file_last_path_, "JSON (*.json)");

    if(!path.isEmpty()) ui->entryPreferedFilePath->setText(path);

    return;
}

void Settings::slSetDefault()
{
    file_prefered_path_ = default_prefered_path_;
    file_use_last_path_ = default_use_last_path_;
    file_last_path_ = default_last_path_;
    entries_maximum_ = default_entries_maximum_;
    entries_order_shuffle_ = default_order_shuffle_;
    entries_side_shuffle_ = default_side_shuffle_;
    test_timer_enabled_ = default_test_timer_enabled_;
    auto_save_ = default_auto_save_;

    UpdateView();
    slUseLastPathToggled(default_use_last_path_);

    return;
}

void Settings::slUseLastPathToggled(int checked)
{
    ui->filePathLabel->setEnabled(!checked);
    ui->entryPreferedFilePath->setEnabled(!checked);
    ui->preferedPathButton->setEnabled(!checked);

    return;
}

void Settings::UpdateModel()
{
    file_prefered_path_ = ui->entryPreferedFilePath->text();
    file_use_last_path_ = ui->useLastOpenedCheckbox->isChecked();
    entries_maximum_ = ui->maximumSpinBox->value();
    entries_order_shuffle_ = ui->swapOrderCheckbox->isChecked();
    entries_side_shuffle_ = ui->swapSideCheckbox->isChecked();
    test_timer_enabled_ = ui->enableTimerCheckbox->isChecked();
    auto_save_ = ui->saveCheckbox->isChecked();

    return;
}

void Settings::UpdateView()
{
    ui->entryPreferedFilePath->setText(file_prefered_path_);
    ui->useLastOpenedCheckbox->setChecked(file_use_last_path_);
    ui->maximumSpinBox->setValue(entries_maximum_);
    ui->swapOrderCheckbox->setChecked(entries_order_shuffle_);
    ui->swapSideCheckbox->setChecked(entries_side_shuffle_);
    ui->enableTimerCheckbox->setChecked(test_timer_enabled_);
    ui->saveCheckbox->setChecked(auto_save_);

    return;
}
