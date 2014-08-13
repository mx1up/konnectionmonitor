#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QDialog>

namespace Ui {
    class AboutWidget;
}

namespace nsKonnectionMonitor {

class AboutWidget : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWidget(QWidget *parent = 0);
    ~AboutWidget();

    void setVersion(QString ver);
private:
    Ui::AboutWidget *ui;
};

} // namespace nsKonnectionMonitor

#endif // ABOUTWIDGET_H
