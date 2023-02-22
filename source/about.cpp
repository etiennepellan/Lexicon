#include "about.h"
#include "ui_about.h"
#include <QPixmap>
#include <QDateTime>

About::About(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->appLabel->setText(APP_NAME_STR);
    ui->versionLabel->setText(QString("Version ") + VERSION_STR);
    ui->dateLabel->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy"));
}

About::~About()
{
    delete ui;
}
