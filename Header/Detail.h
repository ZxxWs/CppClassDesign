#pragma once
#include <iostream>
using namespace std;

//ѧ��ϸ����
class Detail
{
private:
	string Event;
	string Time;
	double Score;
public:
	Detail();
	Detail(string Sdetail);
	//void setEvent(string Event);
	//void setTime(string Time);
	//void setScore(double Score);
	string getEvent();
	string getTime();
	double getScore();
};
//
//double TurnScore(string Score) {
//	int front, back;
//	front = (int)Score[0] - '0';
//	back = (int)Score[2] - '0';
//	double re = front + (0.1 * back);
//	return re;
//}
//Detail::Detail() {
//	//Ĭ�Ϲ��캯��
//	//���û��Ĭ�Ϲ��캯����Studen���ڳ�ʼ����ʱ������
//}
//
//Detail::Detail(string Sdetail) {
//	int i = Sdetail.find("#");
//	this->Event = Sdetail.substr(0, i - 1);
//	Sdetail = Sdetail.substr(i + 1);
//	i = Sdetail.find("#");
//	this->Time = Sdetail.substr(0, i - 1);
//	Sdetail = Sdetail.substr(i + 1);
//	this->Score = TurnScore(Sdetail);
//}
//string Detail::getEvent() {
//	return this->Event;
//}
//
//string Detail::getTime() {
//	return this->Time;
//}
//
//double Detail::getScore() {
//	return this->Score;
//}