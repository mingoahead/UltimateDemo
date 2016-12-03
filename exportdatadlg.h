#ifndef EXPORTDATADLG_H
#define EXPORTDATADLG_H

#include <QDialog>

namespace Ui {
class ExportDataDlg;
}

class ExportDataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDataDlg(QWidget *parent = 0);
    ~ExportDataDlg();

private slots:
    void on_imageSeriesFolderToolButton_released();
    void on_buttonBox_accepted();

    void on_outputFilePathToolButton_released();

private:
    Ui::ExportDataDlg *ui;
};

#endif // EXPORTDATADLG_H
