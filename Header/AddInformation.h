#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_AddInformation.h"
#include <qtextedit.h>
#include <QLabel>

class AddInformation : public QMainWindow
{
	Q_OBJECT

public:
	AddInformation(QWidget* parent = Q_NULLPTR);
	QPushButton* AddGradeButton;
	QPushButton* AddStuButton;
	QPushButton* SureButton;
	QLabel* GradeListLable;
	QLabel* NameLable;
	QLabel* NumLabel;
	QLabel* RemarkLable;
	QLabel* TagLable;
	QComboBox* GradeComboBox;
	QTextEdit* RemarkTextEdit;
	QLineEdit* NumLineEdit;
	QLineEdit* NameLineEdit;

public slots:
	void ClickGradeButton();//��Ӱ�ť�������������
	void ClickStuButton();//��Ӱ�ť�������������
	void ClickSureButton();//ȷ�ϰ�ť�������������

private:
	Ui::AddInformation ui;
};
