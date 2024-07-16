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

	//生成公开素数
	int generate_p();
	//生成公开原根
	int generate_g();

	//生成私钥ab
	int generate_private();

	//计算公钥AB
	int culculate_public();

	//计算共享秘钥K
	int culculate_shared_key();


};