/*基于NTL库的模指数外包的加密处理程序 just for fun*/ 
#include<NTL/ZZ.h>
#include<NTL/tools.h>
#include<NTL/LLL.h>
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
#include<NTL/vec_ZZ.h>
#include<NTL/vector.h>
NTL_CLIENT

void gen_pq(ZZ& p,ZZ& q,ZZ& g,int p_len,int q_len);
void randpairs(ZZ p,ZZ q,ZZ g,int n,vec_ZZ& k,vec_ZZ& gk); 
void gen_ua(ZZ p,ZZ q,ZZ& u,ZZ& a);
char* trans10to16(char* str16,ZZ num);

int main(void)
{
	struct timeval tstart1,tend1,tstart2,tend2,tstart3,tend3;
	float time1,time2,time3;
	ifstream inf;
	ofstream outf; 
	char str[1000],str16[1000];
	//p q g生成调用 
	ZZ p,q,g;
	int p_len,q_len;
	cout<<"p长度=";
	cin>>p_len;
	cout<<"q长度="; 
	cin>>q_len;
	gen_pq(p,q,g,p_len,q_len);
	
	//或者pqg读入
	
 	//inf.open("p 3072 q 256 g.txt");
 /*	inf.open("p 1024 q 160 g.txt");
 	inf>>p>>q>>g;
 	inf.close();
*/
cout<<"p q g="<<p<<" "<<q<<" "<<g<<endl; 

	//rand调用生成pairs 
	int n;
	vec_ZZ k,gk;
	cout<<"生成盲化数对个数n=(6?)";
	cin>>n;
	k.SetLength(n);
	gk.SetLength(n);
	randpairs(p,q,g,n,k,gk);
	
	cout<<"pairs="<<k<<" "<<gk<<endl;
	//u a 生成调用,ua没有长度限制 ，在Zp Zq 中即可 
	ZZ u,a;
	gen_ua(p,q,u,a);
	cout<<"u a="<<u<<" "<<a<<endl;

/*===========================其他本地计算==============================*/ 

//仅做攻击 
 	ZZ t1,t2,r,y1,y2,x1,x2;
	ZZ temp,temp1,temp2,temp3,temp4,temp5;
	ZZ A,B,C,D,X,Y,Z,A_,B_,C_,D_;
	mat_ZZ L;
	
	L.SetDims(4,4);
	
	//生成ti r 
	int al,be1,be2,j;
		//危险范围内 
	RandomLen(t1,64);
	t2=t1;	
	do{
		RandomBnd(r,ZZ(12));
	}while(r<2);
	be1=NumBits(t1);
	be2=NumBits(t2);	
	al=NumBits(r);	 

	 
	cout<<"bits="<<al<<" "<<be1<<" "<<be2<<endl;
	cout<<"t1="<<t1<<endl<<"t2="<<t2<<endl<<"r="<<r<<endl;
	
	//计算xiyi
	
	//cout<<"k1t1="<<k1t1<<endl;
	ZZ zd;
	XGCD(zd,temp1,temp,operator*(k[1],t1),q);//这里的temp1是没有模q的,z=temp1*k1t1+temp*q=1,即temp1为k1t1模q的逆 
	cout<<"gcd="<<zd<<endl<<"inv="<<temp1<<endl<<"test(1)="<<endl<<MulMod(operator*(k[1],t1)%q,temp1%q,q)<<endl<<MulMod(operator*(k[1],t1),temp1,q)%q<<endl; //correct 
	temp2=SubMod(operator*(k[5],a),k[3],q);//k5a-k3 mod q
	y1=MulMod(temp1%q,temp2%q,q);//cout<<"y1="<<y1<<endl;
	x1=SubMod(a%q,operator*(t1,y1)%q,q);
	
	cout<<"x1y1="<<x1<<" "<<y1<<endl;
	
	//计算x2,y2 
	
	XGCD(zd,temp1,temp,operator*(k[2],t2),q);//同上 XGCD.疑问：这里用k2t2和前边求k1t1时是否也需要先模q? 
	cout<<"gcd="<<zd<<endl<<"inv="<<temp1<<endl<<"test(1)="<<endl<<MulMod(operator*(k[2],t2)%q,temp1%q,q)<<endl<<MulMod(operator*(k[2],t2),temp1,q)%q<<endl;
	cout<<r<<" "<<k[0]<<" "<<k[4]<<endl; 
	temp2=SubMod(r*operator*(k[0],a),k[4],q);//temp2=k6*r*a-k4 mod q	
	
	//temp2=SubMod(k6ra,k4,q)%q;//这里究竟要不要多模一次,但最后求得的x2y2是一样的，不影响 
	y2=MulMod(temp1%q,temp2%q,q);
	
	x2=SubMod(operator*(r,a)%q,operator*(t2,y2)%q,q);//输入参数超过q时要先模q，小于q的可模可不模 
	cout<<"x2="<<x2<<endl;
	cout<<"y2="<<y2<<endl;
	//多项式系数
	A=x1;
	B=y1;
	C=-y2;
	D=-x2;
	X=ZZ(12);
	
	cout<<"ABCD="<<endl<<A<<endl<<B<<endl<<C<<endl<<D<<endl;
	
	do{
		RandomLen(Z,64);
		Y=operator*(X,Z);
	}while(operator>(t2,Z));
	
	cout<<"X="<<X<<endl<<"Y="<<Y<<endl<<"Z="<<Z<<endl;

	XGCD(zd,D_,temp,D,q);
	cout<<"gcd="<<zd<<endl;	
	A_=operator*(D_,A);//乘D的逆 
	B_=operator*(D_,B);
	C_=operator*(D_,C);
	//构造格基
	L(1,1)=ZZ(1); L(1,2)=operator*(A_,X); L(1,3)=operator*(B_,Y); L(1,4)=operator*(C_,Z);
	L(2,1)=ZZ(0); L(2,2)=operator*(q,X);  L(2,3)=ZZ(0);           L(2,4)=ZZ(0);
	L(3,1)=ZZ(0); L(3,2)=ZZ(0);           L(3,3)=operator*(q,Y);  L(3,4)=ZZ(0);
	L(4,1)=ZZ(0); L(4,2)=ZZ(0);           L(4,3)=ZZ(0);           L(4,4)=operator*(q,Z); 
	outf.open("attack_result.txt");
	outf<<"约化前格基为："<<endl;
		outf<<"L(1,1)="<<trans10to16(str16,L(1,1))<<endl;outf<<"L(1,2)="<<trans10to16(str16,L(1,2))<<endl;outf<<"L(1,3)="<<trans10to16(str16,L(1,3))<<endl;outf<<"L(1,4)="<<trans10to16(str16,L(1,4))<<endl;
		outf<<"L(2,1)="<<trans10to16(str16,L(2,1))<<endl;outf<<"L(2,2)="<<trans10to16(str16,L(2,2))<<endl;outf<<"L(2,3)="<<trans10to16(str16,L(2,3))<<endl;outf<<"L(2,4)="<<trans10to16(str16,L(2,4))<<endl;
		outf<<"L(3,1)="<<trans10to16(str16,L(3,1))<<endl;outf<<"L(3,2)="<<trans10to16(str16,L(3,2))<<endl;outf<<"L(3,3)="<<trans10to16(str16,L(3,3))<<endl;outf<<"L(3,4)="<<trans10to16(str16,L(3,4))<<endl;
		outf<<"L(4,1)="<<trans10to16(str16,L(4,1))<<endl;outf<<"L(4,2)="<<trans10to16(str16,L(4,2))<<endl;outf<<"L(4,3)="<<trans10to16(str16,L(4,3))<<endl;outf<<"L(4,4)="<<trans10to16(str16,L(4,4))<<endl;
	outf.close();
	//约化
	gettimeofday(&tstart1,NULL); 
	ZZ det2;
	LLL(det2,L,0); 
	//攻击结果
	ZZ h1,h2,h3,h4,g1,g2,g3,g4,f1,f2,f3,f4; 
	ZZ x,y,z,x0,y0,z0,tempa,tempb,tempres;

//	h1=L(1,1);h2=L(1,2);h3=L(1,3);h4=L(1,4);
//	g1=L(2,1);g2=L(2,2);g3=L(2,3);g4=L(2,4);
//	f1=L(3,1);f2=L(3,2);f3=L(3,3);f4=L(3,4);
	
	temp1=operator-(operator*(L(1,2),L(2,1)),operator*(L(1,1),L(2,2)));
	temp2=operator-(operator*(L(1,2),L(3,3)),operator*(L(1,3),L(3,2)));
	temp3=operator-(operator*(L(1,2),L(3,1)),operator*(L(1,1),L(3,2)));
	temp4=operator-(operator*(L(1,2),L(2,3)),operator*(L(1,3),L(2,2)));
	tempa=operator-(operator*(temp1,temp2),operator*(temp3,temp4));
	
	temp1=operator-(operator*(L(1,2),L(2,4)),operator*(L(1,4),L(2,2)));
	temp2=operator-(operator*(L(1,2),L(3,3)),operator*(L(1,3),L(3,2)));
	temp3=operator-(operator*(L(1,2),L(3,4)),operator*(L(1,4),L(3,2)));
	temp4=operator-(operator*(L(1,2),L(2,3)),operator*(L(1,3),L(2,2)));
	tempb=operator-(operator*(temp1,temp2),operator*(temp3,temp4));

	//temp2=operator-(operator*(operator-(operator*(h2,g4),operator*(h4,g2)),operator-(operator*(h2,f3),operator*(h3,f2))),operator*(operator-(operator*(h2,f4),operator*(h4,f2)),operator-(operator*(h2,g3),operator*(h3,g2)))); 
//	temp1=operator-(operator*(operator-(operator*(,),operator*(,)),operator-(operator*(,),operator*(,))),operator*(operator-(operator*(,),operator*(,)),operator-(operator*(,),operator*(,)))); 
	 tempa=-operator*(tempa,Z);
//	 cout<<"divide(z0)? (should be 1)="<<divide(tempa,tempb)<<endl;
	 z0=operator/(tempa,tempb);
	
	temp1=operator-(operator*(L(1,2),L(2,4)),operator*(L(1,4),L(2,2)));
	temp2=operator-(operator*(L(1,2),L(2,1)),operator*(L(1,1),L(2,2)));
	temp3=operator-(operator*(L(1,2),L(2,3)),operator*(L(1,3),L(2,2)));
	tempa=operator+(operator*(temp1,z0),operator*(temp2,Z));
	tempb=operator*(temp3,Z);
	tempa=-operator*(tempa,Y);
//	cout<<"divide(y0)? (should be 1)="<<divide(tempa,tempb)<<endl;
	y0=operator/(tempa,tempb);
	
	temp1=operator*(operator*(Y,Z),L(1,1));
	temp2=operator*(operator*(L(1,3),y0),Z);
	temp3=operator*(operator*(L(1,4),z0),Y);
	temp4=operator*(operator*(Y,Z),L(1,2));
	tempa=operator+(operator+(temp1,temp2),temp3);
	tempb=temp4;
	tempa=-operator*(tempa,X);
//	cout<<"divide(x0)? (should be 1)="<<divide(tempa,tempb)<<endl;
	x0=operator/(tempa,tempb);
	gettimeofday(&tend1,NULL);
	time1=1000*(tend1.tv_sec-tstart1.tv_sec)+(tend1.tv_usec-tstart1.tv_usec)/1000;
	cout<<"time="<<time1<<"ms"<<endl; 
	//ceshi
//	cout<<"t2="<<t2<<" "<<"z0="<<z0<<endl;
//	cout<<"rt1="<<operator*(r,t1)<<" "<<"y0="<<y0<<endl;
//	cout<<"r="<<r<<" "<<"x0="<<x0<<endl;
	 cout<<"CHECK RESULT:"<<endl;
	if(!compare(x0,r)) cout<<"x0 is CORRECT!"<<endl;else cout<<"x0 is WRONG!"<<endl;
	if(!compare(y0,operator*(r,t1))) cout<<"y0 is CORRECT!"<<endl;else cout<<"y0 is WRONG!"<<endl;
	if(!compare(z0,t2)) cout<<"z0 is CORRECT!"<<endl;else cout<<"z0 is WRONG!"<<endl;
	if(!compare(a,operator+(x1,operator*(t1,y1))%q))cout<<"check a correct"<<endl;else cout<<"check a wrong!"<<endl;
	cout<<endl<<"ATTACK FINISH!"<<endl;

//外包计算部分

//check ad recoveer

//计时 

//输出为文档 in 16

outf.open("attack_result.txt",ios::app);
	outf<<"p="<<endl<<trans10to16(str,p)<<endl;
	outf<<"q="<<endl<<trans10to16(str,q)<<endl;
	outf<<"g="<<endl<<trans10to16(str,g)<<endl;
	outf<<"u="<<endl<<trans10to16(str,u)<<endl;
	outf<<"a="<<endl<<trans10to16(str,a)<<endl;
	outf<<"r="<<endl<<trans10to16(str,r)<<endl;
	outf<<"t1,t2="<<endl<<trans10to16(str,t1)<<endl;
	outf<<"X="<<trans10to16(str,X)<<endl;
	outf<<"Y="<<trans10to16(str,Y)<<endl;
	outf<<"Z="<<trans10to16(str,Z)<<endl;
	outf<<"x1="<<endl<<trans10to16(str,x1)<<endl;
	outf<<"y1="<<endl<<trans10to16(str,y1)<<endl;
	outf<<"x2="<<endl<<trans10to16(str,x2)<<endl;
	outf<<"y2="<<endl<<trans10to16(str,y2)<<endl<<"约化后格基如下："<<endl;
	outf<<"L(1,1)="<<trans10to16(str16,L(1,1))<<endl;outf<<"L(1,2)="<<trans10to16(str16,L(1,2))<<endl;outf<<"L(1,3)="<<trans10to16(str16,L(1,3))<<endl;outf<<"L(1,4)="<<trans10to16(str16,L(1,4))<<endl;
	outf<<"L(2,1)="<<trans10to16(str16,L(2,1))<<endl;outf<<"L(2,2)="<<trans10to16(str16,L(2,2))<<endl;outf<<"L(2,3)="<<trans10to16(str16,L(2,3))<<endl;outf<<"L(2,4)="<<trans10to16(str16,L(2,4))<<endl;
	outf<<"L(3,1)="<<trans10to16(str16,L(3,1))<<endl;outf<<"L(3,2)="<<trans10to16(str16,L(3,2))<<endl;outf<<"L(3,3)="<<trans10to16(str16,L(3,3))<<endl;outf<<"L(3,4)="<<trans10to16(str16,L(3,4))<<endl;
	outf<<"L(4,1)="<<trans10to16(str16,L(4,1))<<endl;outf<<"L(4,2)="<<trans10to16(str16,L(4,2))<<endl;outf<<"L(4,3)="<<trans10to16(str16,L(4,3))<<endl;outf<<"L(4,4)="<<trans10to16(str16,L(4,4))<<endl;
	outf<<"求得解如下："<<endl<<"x0(r)="<<trans10to16(str,x0)<<endl;outf<<"y0(rt1)="<<trans10to16(str,y0)<<endl;outf<<"z0(t2)="<<trans10to16(str,z0)<<endl; 
	outf<<"恢复指数a="<<trans10to16(str,operator+(x1,operator*(t1,y1))%q)<<endl; 
	outf.close(); 
 } 


 
