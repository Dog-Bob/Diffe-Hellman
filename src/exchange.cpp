#include"exchange.h"

Exchange::Exchange()
{

}
Exchange::Exchange(int f)
{
	this->f = f;
	generate_p();
	generate_g();
}
Exchange::~Exchange()
{

}

//生成公开素数
int Exchange::generate_p()
{
	p = DifHel::generatePrime(BITS);
	return 0;
}
//生成公开原根
int Exchange::generate_g()
{
	g = DifHel::findPrimitiveRoot(p);
	return 0;
}

//生成私钥
int Exchange::generate_private()
{
	if(f==0)
		a = DifHel::generateRandomNumber(BITS)%p;
	else 
		b = DifHel::generateRandomNumber(BITS)%p;
	return 0;
}

//计算公钥
int Exchange::culculate_public()
{
	if(f==0)
		A = DifHel::powerMod(g, a, p);
	else 
		B = DifHel::powerMod(g, b, p);
	return 0;
}

//计算共享秘钥K
int Exchange::culculate_shared_key()
{
	if(f==0)
		K = DifHel::powerMod(B, a, p);
	else
		K = DifHel::powerMod(A, b, p);
	return 0;
}



