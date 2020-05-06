#define _CRT_SECURE_NO_WARNINGS

#pragma execution_character_set("UTF-8")//����qt�ı��룬���û�У����������������
#include "../Header/AlterDetailUI.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>

using namespace std;

//��������ʾ���ڣ�0��ʾ�޸�ѧ��ϸ��ɹ���1��ʾ�޸�ʧ��
//�����tag����Ϊint��Ϊ�˷�ֹ֮���б�Ĵ��ڵ�������ʾ��Ϣ��
AlterDetailUI::AlterDetailUI(int tag,QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->ShowLabel = ui.ShowLabel;
	this->BackButton = ui.BackButton;

	connect(ui.BackButton, SIGNAL(clicked()), this, SLOT(ClickBackButton()));//����ť�͵���¼���
	if (tag==0) {
		ShowLabel->setText("ѧ��ϸ���޸ĳɹ���");
	}
	else if(tag==1)
	{
		ShowLabel->setText("ѧ��ϸ���޸�ʧ�ܡ�");
	}
	
}

void AlterDetailUI::ClickBackButton() {//���ذ�ť����¼�
	emit sendsignal();//�������洫�ݱ��ر���Ϣ
	this->close();
}

void AlterDetailUI::closeEvent(QCloseEvent* event) {
	emit sendsignal(); // �������洫�ݱ��ر���Ϣ
}
