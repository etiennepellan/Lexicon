#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include "settings.h"
#include "listedit.h"
#include "entrydisplay.h"
#include "entryedit.h"
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        QPoint GetWindowPosition(void) const;
        QSize GetWindowSize(void) const;

    private:

        Ui::MainWindow *ui;
        Settings settings_;
        ListEdit list_edit_;
        About about_;
        std::vector<EntryModel> entries_;
        bool test_in_progress_;
        QElapsedTimer timer_;
        QTimer interval_timer_;
        bool entries_displayed_;

    private:

        int CheckResults(void);
        void ClearEntries(void);
        void closeEvent(QCloseEvent *event);
        bool LoadEntries(const QString &file_path);
        void DisplayEntries(void);
        void DisplayError(const QString &error_msg);
        void DisplayResult(int result_percent);

    private slots:

        void slAbout(void);
        void slEditList(void);
        void slFinish(void);
        void slOpenFile(void);
        void slOpenPreferences(void);
        void slStart(void);
};
#endif // MAINWINDOW_H
