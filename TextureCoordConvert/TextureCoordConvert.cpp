#include "TextureCoordConvert.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

TextureCoordConvert::TextureCoordConvert(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.labelCurrentFile->setText("");


	ui.lineEditInputPath->setText(QDir::rootPath());
	ui.lineEditOutputPath->setText(QDir::rootPath());
}

void TextureCoordConvert::on_pushButtonBrowser_clicked()
{
	QString inputPath = QFileDialog::getExistingDirectory(this, "Select input path!", ui.lineEditInputPath->text());


	if (!inputPath.isEmpty()) {
		ui.lineEditInputPath->setText(inputPath);

	}
}

void TextureCoordConvert::on_pushButtonBrowserOutput_clicked()
{
	QString outputPath = QFileDialog::getExistingDirectory(this, "Select output path!", ui.lineEditOutputPath->text());


	if (!outputPath.isEmpty()) {
		ui.lineEditOutputPath->setText(outputPath);

	}
}

void TextureCoordConvert::on_pushButtonConvert_clicked() 
{
	QString inputPath = ui.lineEditInputPath->text();

	if (inputPath.isEmpty()) {
		QMessageBox::critical(this, "Error", "Select input folder!");
		return;
	}

	QString outputPath = ui.lineEditOutputPath->text();

	if (outputPath.isEmpty()) {
		QMessageBox::critical(this, "Error", "Select output folder!");
		return;
	}

	if (inputPath == outputPath) {
		QMessageBox::critical(this, "Error", "Input must be different to output!");
		return;
	}

	QDir directory(inputPath);

	QStringList filters;

	filters << "*.obj";

	QStringList filesList = directory.entryList(filters);

	QString fileName;

	if (filesList.size() == 0) {
		QMessageBox::critical(this, "Error", "There is no obj file!");

		return;
	}

	convertedCount = 0;

	foreach(fileName, filesList) {
		qDebug() << "FileName " << fileName;

		QString filePath = inputPath + "/" + fileName;
		QString convertedFilePath = outputPath + "/" + fileName;

		qApp->processEvents();

		ui.labelCurrentFile->setText(filePath);

		oneProcess(filePath, convertedFilePath);
	}

	ui.labelCurrentFile->setText("");

	QMessageBox::information(this, "Info", "done! count = " + QString::number(this->convertedCount));
}

void TextureCoordConvert::oneProcess(QString filePath, QString convertedFilePath) {
	QFile origObjeFile(filePath);
	QFile convertedObjFile(convertedFilePath);

	if (!convertedObjFile.open(QFile::WriteOnly)) {
		QMessageBox::critical(this, "Error", "Failed to create new file: " + convertedFilePath);

		return;
	}

	QString line;

	if (origObjeFile.open(QFile::ReadOnly)) {
		QTextStream in(&origObjeFile);
		
		QTextStream out(&convertedObjFile);

		do {
			line = in.readLine();

			QStringList tokens = line.split(" ");

			if (tokens.size() > 0 && tokens[0] == "vt") {

				if (tokens.size() < 3) {
					QMessageBox::critical(this, "Error", filePath + " invalid token size");
					continue;
				}

				QString strY = tokens[2];

				double y = strY.toDouble();

				y = 1 - y;

				if (y < 0) {
					QMessageBox::critical(this, "Error", filePath + " invalid y");
					continue;
				}

				QStringList newTokens;

				newTokens << tokens[0] << tokens[1] << QString::number(y);

				out << newTokens.join(" ") << "\n";
			}
			else {
				out << line << "\n";
			}
			

		} while (!line.isNull());
	}

	convertedCount++;
}