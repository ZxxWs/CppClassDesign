#pragma once
#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_Search.h"


class Search : public QMainWindow
{
	Q_OBJECT

public:
	Search(QWidget* parent = Q_NULLPTR);

	QComboBox* GradeComboBox = new QComboBox;
	QComboBox* NumComboBox = new QComboBox;
	QLabel* LabNum = new QLabel;
	QLabel* LabName = new QLabel;
	QLabel* LabGrade = new QLabel;
	QLabel* LabScore = new QLabel;
	QLabel* LabRemake = new QLabel;
	QTableView* ShowTable=new QTableView;
	QPushButton* AddButton = new QPushButton;
	QPushButton* DelButton= new QPushButton;
	QPushButton* AlterButton=new QPushButton;
	QPushButton* OutButton=new QPushButton;

public slots:
	void GradeComboBoxChanged();//����б������ֵ�����˸ı�֮���Զ�ʶ��
	void NumComboBoxChanged();//ѧ�ź��б�������
	void ClickAddButton();//��Ӱ�ť�������������
	void ClickDelButton();//��Ӱ�ť�������������
	void ClickAlterButton();//��Ӱ�ť�������������
	void ClickOutButton();//��Ӱ�ť�������������

private:
	Ui::Search ui;
};
