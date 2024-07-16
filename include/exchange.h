#pragma once

#include"diff_hellman.h"
#include<iostream>

using namespace std;

class Exchange
{
private:
	int a,b;
	const int BITS = 8;
	int f;
	~Exchange();
public:
	int p, g, A,B,K;

	Exchange();
	//0 A,1 B
	Exchange(int f);

	//���ɹ�������
	int generate_p();
	//���ɹ���ԭ��
	int generate_g();

	//����˽Կab
	int generate_private();

	//���㹫ԿAB
	int culculate_public();

	//���㹲����ԿK
	int culculate_shared_key();


};