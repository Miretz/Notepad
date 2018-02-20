#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontDialog>

#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    setCentralWidget(ui->centralWidget);
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(
                    this, "Warning", "Cannot open file: "
                             + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void Notepad::on_actionSave_triggered()
{
    QString fileName;
    if(currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    }
    else
    {
        fileName = currentFile;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(
                    this, "Warning", "Cannot save file: "
                        + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void Notepad::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(
                    this, "Warning", "Cannot save file: "
                        + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void Notepad::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void Notepad::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void Notepad::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void Notepad::on_actionUndo_triggered()
{
     ui->textEdit->undo();
}

void Notepad::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void Notepad::on_actionFont_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::on_actionSearch_triggered()
{
    on_searchButton_clicked();
}

void Notepad::on_searchButton_clicked()
{
    QString searchString = ui->lineEdit->text();

    bool found = ui->textEdit->find(searchString, QTextDocument::FindWholeWords);

    //wrap around
    if(!found)
    {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        ui->textEdit->setTextCursor(cursor);
        found = ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
    }

    //text not found after wrap - does not exist
    if(!found)
    {
        QMessageBox::warning(
                    this, "Not found", "Keword not found: " + searchString);
    }
}
