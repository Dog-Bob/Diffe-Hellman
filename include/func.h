#pragma once
#include<iostream>
#include<string>

using namespace std;

//将p,A,g转化为string pAg '\0'结尾
string p_A_g(int p, int A, int g);

//将int i->string '\0'结尾
string itoc(int i);

//将s->int
int parseCH(string s);
