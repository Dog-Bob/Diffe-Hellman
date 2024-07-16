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

//���ɹ�������
int Exchange::generate_p()
{
	p = DifHel::generatePrime(BITS);
	return 0;
}
//���ɹ���ԭ��
int Exchange::generate_g()
{
	g = DifHel::findPrimitiveRoot(p);
	return 0;
}

//����˽Կ
int Exchange::generate_private()
{
	if(f==0)
		a = DifHel::generateRandomNumber(BITS)%p;
	else 
		b = DifHel::generateRandomNumber(BITS)%p;
	return 0;
}

//���㹫Կ
int Exchange::culculate_public()
{
	if(f==0)
		A = DifHel::powerMod(g, a, p);
	else 
		B = DifHel::powerMod(g, b, p);
	return 0;
}

//���㹲����ԿK
int Exchange::culculate_shared_key()
{
	if(f==0)
		K = DifHel::powerMod(B, a, p);
	else
		K = DifHel::powerMod(A, b, p);
	return 0;
}



