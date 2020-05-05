#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
#include "../Header/AlterDetailUI.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>


using namespace std;

AlterDetailUI::AlterDetailUI(bool tag,QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->ShowLabel = ui.ShowLabel;
	this->BackButton = ui.BackButton;
	connect(ui.BackButton, SIGNAL(clicked()), this, SLOT(ClickBackButton()));//����ť�͵���¼���
	if (tag) {
		ShowLabel->setText("ѧ��ϸ���޸ĳɹ���");
	}
	else
	{
		ShowLabel->setText("ѧ��ϸ���޸�ʧ�ܡ�");
	}
}

void AlterDetailUI::ClickBackButton() {
	emit sendsignal();
	this->close();
}

void AlterDetailUI::closeEvent(QCloseEvent* event) {
	emit sendsignal();
}
