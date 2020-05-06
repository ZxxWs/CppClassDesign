#pragma once

#include <QtWidgets/QMainWindow>
#include "../x64/Debug/uic/UI/ui_AlterDetailUi.h"
#include "qpushbutton.h"

class AlterDetailUI : public QMainWindow
{
	Q_OBJECT

public:
	AlterDetailUI(int tag,QWidget* parent = Q_NULLPTR);//这个参数用于上个界面传参

	QPushButton* BackButton;
	QLabel* ShowLabel;


public slots:
	void ClickBackButton();//按钮点击事件
	
private:
	Ui::AlterDetailUI ui;

signals:
	void sendsignal();//这个函数用户向主界面通知关闭的消息

protected:
	void closeEvent(QCloseEvent* event);//关闭界面的逻辑，主要是给上个界面来传递参数

};
