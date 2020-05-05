#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_Tz.h"
#include "qpushbutton.h"

class Tz : public QMainWindow
{
	Q_OBJECT

public:
	Tz(QWidget *parent = Q_NULLPTR);
	QLabel* PassLabel;
	QLabel* NewPassLabel;
	QLabel* SurePassLabel;
	QLabel* TagLabel;//���������ʾ��Ϣ
	QLineEdit* PassLineEdit;
	QLineEdit* NewPassLineEdit;
	QLineEdit* SurePassLineEdit;
	QPushButton* AlterPassButton;
	QPushButton* LogInButton;	//����������ȷ�ϰ�ť����

public slots:
	void ClickLogInButton();//��ť����¼�
	void ClickAlterPassButton();//��ť����¼�

private:
	Ui::TzClass ui;

};
