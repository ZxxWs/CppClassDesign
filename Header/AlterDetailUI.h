#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_AlterDetailUi.h"
#include "qpushbutton.h"

class AlterDetailUI : public QMainWindow
{
	Q_OBJECT

public:
	AlterDetailUI(int tag,QWidget* parent = Q_NULLPTR);//������������ϸ����洫��

	QPushButton* BackButton;
	QLabel* ShowLabel;


public slots:
	void ClickBackButton();//��ť����¼�
	
private:
	Ui::AlterDetailUI ui;

signals:
	void sendsignal();//��������û���������֪ͨ�رյ���Ϣ

protected:
	void closeEvent(QCloseEvent* event);//�رս�����߼�����Ҫ�Ǹ��ϸ����������ݲ���

};
