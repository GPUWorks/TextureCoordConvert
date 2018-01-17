#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TextureCoordConvert.h"

class TextureCoordConvert : public QMainWindow
{
    Q_OBJECT

public:
    TextureCoordConvert(QWidget *parent = Q_NULLPTR);

private slots:
	void on_pushButtonBrowser_clicked();
	void on_pushButtonBrowserOutput_clicked();
	void on_pushButtonConvert_clicked();

private:
	void oneProcess(QString filePath, QString convertedFilePath);

private:
    Ui::TextureCoordConvertClass ui;

	int convertedCount;
};
