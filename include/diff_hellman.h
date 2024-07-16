#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<random>
#include<cmath>

using ull=unsigned long long;

using namespace std;

class DifHel
{
public:
	//公钥素数q，原根a
	ull a,q;
private:
	//私钥
	ull sKey;
	
private:
	
public:
	DifHel();
	~DifHel();

	
	static bool isPrime(ull n, int k);
	// 快速幂取模
	static ull powerMod(ull base, ull exp, ull mod);

	// 判断 g 是否是 q 的原根
	static bool isPrimitiveRoot(ull g, ull q);

	//求q一个原根
	static ull findPrimitiveRoot(ull q);
	
	//生成指定位数随机数
	static ull generateRandomNumber(int digits);

	//生成指定位数素数
	static ull generatePrime(int digits);
	
	//米勒罗宾素性检测
	static bool millerRabinTest(ull d, ull n);
	
};
