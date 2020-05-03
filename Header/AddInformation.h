#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_AddInformation.h"


class AddInformation : public QMainWindow
{
	Q_OBJECT

public:
	AddInformation(QWidget* parent = Q_NULLPTR);


	//QLabel* lab = new QLabel;

public slots:
	

private:
	Ui::AddInformation ui;
};
