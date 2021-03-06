#include "exportdatadlg.h"
#include "ui_exportdatadlg.h"

# include <QFileDialog>
# include "itkutils.h"

ExportDataDlg::ExportDataDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDataDlg)
{
    ui->setupUi(this);

    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ExportDataDlg::~ExportDataDlg()
{
    delete ui;
}

void ExportDataDlg::on_imageSeriesFolderToolButton_released()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      tr("/home"),
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    ui->imageSeriesFolderLineEdit->setText(dirName);
    std::cout << "input dirs : " << dirName.toStdString() << std::endl;
}

void ExportDataDlg::on_buttonBox_accepted()
{
    if (ui->imageSeriesFolderLineEdit->text().isEmpty() || ui->outputFilePathlineEdit->text().isEmpty()) {
        return ;
    }

    try {        
        ItkUtils::DicomSeries2Raw<short, short>(ui->imageSeriesFolderLineEdit->text().toStdString(), ui->outputFilePathlineEdit->text().toStdString());
    } catch (std::exception &ex) {

        std::cerr << ex.what() << std::endl;
        return ;
    }
}

void ExportDataDlg::on_outputFilePathToolButton_released()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "/home/3dresearch/TestData/clinical/untitled.raw",
                               tr("raw (*.mha *.raw)"));
    ui->outputFilePathlineEdit->setText(fileName);
    std::cout << "output path : " << fileName.toStdString() << std::endl;
}
