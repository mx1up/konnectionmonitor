#include "aboutwidget.h"
#include "ui_aboutwidget.h"

namespace nsKonnectionMonitor {

AboutWidget::AboutWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

void AboutWidget::setVersion(QString ver)
{
    ui->versionLabel->setText(ver);
}

} // namespace nsKonnectionMonitor
