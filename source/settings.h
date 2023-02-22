#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QGuiApplication>
#include <QScreen>

#define DEFAULT_WIN_WIDTH 400
#define DEFAULT_WIN_HEIGHT 500

class MainWindow; // forward declaration

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:

    Settings(QWidget *parent = nullptr);
    ~Settings();

    void LoadFromSystem();
    void SaveToSystem();
    void SetLastOpenedFilePath(const QString file_path);

    bool IsAutoSaveEnabled() const { return auto_save_; }
    bool IsOrderShuffleEnabled() const { return entries_order_shuffle_; }
    bool IsSideShuffleEnabled() const { return entries_side_shuffle_; }
    bool IsUsingLastFilePath() const { return file_use_last_path_; }
    int GetEntriesMaximum() const { return entries_maximum_; }
    const QString GetLastOpenedFilePath() const { return file_last_path_; }
    QPoint GetWindowPosition() const { return window_pos_; }
    QSize GetWindowSize() const { return window_size_; }
    bool IsTimerEnabled() const { return test_timer_enabled_; }

private:

    void closeEvent(QCloseEvent *event);
    void UpdateModel();
    void UpdateView();

private:

    Ui::Settings *ui;

    MainWindow *mw;
    QSettings sys_settings_;

    const QString str_window_size_ = "window/size";
    const QString str_window_pos_ = "window/position";
    const QString str_entries_maximum_ = "entries/maximum";
    const QString str_entries_order_shuffle_ = "entries/order_shuffle";
    const QString str_entries_side_shuffle_ = "entries/side_shuffle";
    const QString str_file_prefered_path_ = "file/prefered_path";
    const QString str_file_use_last_path_ = "file/use_last_path";
    const QString str_file_last_path_ = "file/last_path";
    const QString str_test_timer_enabled_ = "test/enable_timer";
    const QString str_auto_save_ = "auto_save";

    const QSize default_window_size_ = QSize(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
    const QPoint default_window_pos_ =
        QGuiApplication::primaryScreen()->geometry().center() -
        QPoint(DEFAULT_WIN_WIDTH / 2, DEFAULT_WIN_HEIGHT / 2);
    const int default_entries_maximum_ = 25;
    const bool default_order_shuffle_ = false;
    const bool default_side_shuffle_ = false;
    const QString default_prefered_path_ = "";
    const bool default_use_last_path_ = true;
    const QString default_last_path_ = "";
    const bool default_test_timer_enabled_ = true;
    const bool default_auto_save_ = true;

    QSize window_size_;
    QPoint window_pos_;
    int entries_maximum_;
    bool entries_order_shuffle_;
    bool entries_side_shuffle_;
    QString file_prefered_path_;
    bool file_use_last_path_;
    QString file_last_path_;
    bool test_timer_enabled_;
    bool auto_save_;

private slots:

    void slClearCache();
    void slEnableTimer(int checked);
    void slPreferedPath();
    void slSetDefault();
    void slUseLastPathToggled(int checked);
};

#endif // SETTINGS_H
