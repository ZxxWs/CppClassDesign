#pragma once
#include <iostream>
using namespace std;

//学分细则类
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

