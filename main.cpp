#include "Tz.h"
#include "Header/Search.h"
#include <QtWidgets/QApplication>


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")
MYSQL* mysql = new MYSQL; //mysql����  
MYSQL_FIELD* fd;    //�ֶ�������  
char field[32][32];    //���ֶ�����ά����  
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ��� 
bool ConnectDatabase();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*Tz w;
	w.show();*/
	Search s;
	s.show();
    ConnectDatabase();
	return a.exec();
}



bool ConnectDatabase()
{
    //��ʼ��mysql  
    mysql_init(mysql);
    //����false������ʧ�ܣ�����true�����ӳɹ�  
    if (!(mysql_real_connect(mysql, "localhost", "root", "123456", "zxx", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
    {
        printf("Error connecting to database:%s\n", mysql_error(mysql));
        return false;
    }
    else
    {
        printf("Connected...\n");
        return true;
    }
    return true;
}