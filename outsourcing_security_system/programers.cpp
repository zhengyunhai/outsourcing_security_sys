#include<NTL/ZZ.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vector.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<sys/time.h>
#include<windows.h> 
#include <cstdlib>
#include <cmath>
#include <NTL/config.h>
#include <NTL/mach_desc.h>
#include<NTL/matrix.h> 
#include<NTL/mat_ZZ.h>
NTL_CLIENT


void gen_pq(ZZ& p,ZZ& q,ZZ& g,int p_len,int q_len)//����p_len���ȵ�p��q_len���ȵ�q����Zp��ȡ����Ԫg 
{
	int i=0;
	ZZ h,seed,temp;
	int prob;
	seed=time(0);
	SetSeed(seed); 
	GenPrime(q,q_len,80);
	
	do{
		i++;
		RandomLen(temp,p_len-q_len);
		p=operator*(temp,q)+1;//180bit
		prob=ProbPrime(p,100);
	}while(!prob||(NumBits(p)!=p_len));
	RandomBnd(h,p);
	g=PowerMod(h,operator/(p-1,q),p);
}

void randpairs(ZZ p,ZZ q,ZZ g,int n,vec_ZZ& k,vec_ZZ& gk)//����n��ä������ 
{
	ZZ M,seed,x,X,a1,a2,a3,a4,a5,b1,b2,b3,b4,b5;
	int x1,x2,x3,x4,x5;
	int i;
	//int n,k,h;
	long int s1,s2,s3;
	mat_ZZ A,B;
	A.SetDims(1,5);
	B.SetDims(1,5);
	seed=time(0);
	SetSeed(seed);
	x1=1;x2=2;x3=3;
	M=q;
	RandomBnd(a1,M);
	RandomBnd(a2,M);
	RandomBnd(a3,M);
	RandomBnd(a4,M);
	RandomBnd(a5,M);
	A(1,1)=a1;A(1,2)=a2;A(1,3)=a3;A(1,4)=a4;A(1,5)=a5;
	
	b1=PowerMod(g,a1,p);
	b2=PowerMod(g,a2,p);
	b3=PowerMod(g,a3,p);
	b4=PowerMod(g,a4,p);
	b5=PowerMod(g,a5,p);
	B(1,1)=b1;B(1,2)=b2;B(1,3)=b3;B(1,4)=b4;B(1,5)=b5;
	
		for(int i=1;i<n+1;i++)
	{
		
	 
	do{
		RandomBnd(s1,6);
		RandomBnd(s2,6);
		RandomBnd(s3,6);
	}while(s1==0||s2==0||s3==0);//1~5����� 
	
	//cout<<"j="<<s1<<" "<<s2<<" "<<s3<<endl;
	//cout<<A(1,temp1)<<endl;
	k[i-1]=AddMod(operator*(A(1,s1),x1),operator+(operator*(A(1,s2),x2),operator*(A(1,s3),x3)),M);
	cout<<"x="<<k[i-1]<<endl;
	gk[i-1]=MulMod(power(B(1,s1),x1),operator*(power(B(1,s2),x2),power(B(1,s3),x3)),p);
	cout<<"X="<<gk[i-1]<<endl;
	}
}

void gen_ua(ZZ p,ZZ q,ZZ& u,ZZ& a)//Zp������u,Zq������a 
{
	int a_len;
	a_len=NumBits(q); 
	//���u a 
	do{
		RandomBnd(a,q);//a in Zq
	}while(NumBits(a)!=a_len);
	
	
//	RandomBnd(u,p);//u in Zp	
//�˴��޸�u�����ɣ���u=h^((p-1)/q) mod p����Rand��g�����ɷ�����ͬ,����˵u��Ӧ�������ַ��� ?

	ZZ h;
	RandomBnd(h,p);
	u=PowerMod(h,operator/(p-1,q),p); 
}
