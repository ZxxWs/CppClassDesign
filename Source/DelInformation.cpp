#include "../Header/DelInformation.h"
#include "qpushbutton.h"
#include <QtWidgets/QMainWindow>

DelInformation::DelInformation(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//connect(ui.SubButton, SIGNAL(clicked()), this, SLOT(ClickButton()));//将按钮和点击事件绑定

	this->DelGradeButton = ui.DelGradeButton;
	this->DelStuButton = ui.DelStuButton;
	this->SureButton = ui.SureButton;
	this->AgainSureButton = ui.AgainSureButton;
	this->CancelButton = ui.CancelButton;
	this->GradeComboBox = ui.GradeComboBox;
	this->StuComboBox = ui.StucomboBox;
	this->SureLabel = ui.SureLabel;
	this->StuLabel = ui.Stulabel;




}
