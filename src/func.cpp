#include"func.h"

string p_A_g(int p, int A, int g)
{
	string sp = to_string(p);
	string sa = to_string(A);
	string sg = to_string(g);
	string tmp="";
	int i = 0;
	for (auto c : sp)
		tmp+=c;
	tmp += '\0';
	for (auto c : sa)
		tmp+= c;
	tmp+= '\0';
	for (auto c : sg)
		tmp+= c;
	tmp+= '\0';
	return tmp;
}

//½«int i->string   '\0'½áÎ²
string itoc(int i)
{
	string si = to_string(i);
	string tmp = "";
	for (auto c : si)
		tmp+= c;
	tmp+= '\0';
	return tmp;
}

//½«s->int
int parseCH(string s)
{
	int tmp = stoi(s);
	return tmp;
}