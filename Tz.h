#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Tz.h"

class Tz : public QMainWindow
{
	Q_OBJECT

public:
	Tz(QWidget *parent = Q_NULLPTR);

private:
	Ui::TzClass ui;
};
