#include<iostream>
#include<fstream>
#include<NTL/ZZ.h>
#include<string> 
NTL_CLIENT

using namespace std;
char* trans10to16(char* str16,ZZ num) //�����޸�Ϊ����ֻ��Ҫnum��its easy 
// int main(void)
{
//	char str16[1000];//ֱ��ʹ��ntl������������洢Ϊ�ַ���
//���Ǹ�������� 
	char temp;
	ZZ d,round,coff;
	int r,len,num_int;
	coff=num;
//	cout<<coff<<endl;
	if(operator<(coff,ZZ(0))) num=-num;
	

//	ifstream inf;
//	inf.open("p 2048 q 256 g.txt");
//	inf>>num;
//	inf.close();	
//	cout<<num<<endl; 

//while(cin>>num)
//{
	int i=0;
	for(i=0;i<1000;i++)//str16��ʼ�� 
	{
		str16[i]='\0';
	}
	i=0;
do
{
	r=DivRem(d,num,16);
	num=d;
	//���r 
	if(r<10) str16[i]=r+'0'; 
	else if(r==10) str16[i]='A';
	else if(r==11) str16[i]='B';
	else if(r==12) str16[i]='C';
	else if(r==13) str16[i]='D';
	else if(r==14) str16[i]='E';
	else if(r==15) str16[i]='F';
	
	i++;
}while(d!=0);
	
	if(operator<(coff,ZZ(0))) str16[i]='-';


	len=0;
	while(str16[len]!='\0')
	{
		len++;
	}
//	cout<<"len="<<len<<endl;
//	cout<<str16[0]<<" "<<str16[len-1]<<endl;
	//str16[len-1]='\0';
//	cout<<"str61="<<str16<<endl;
	
	//����ʮλ����֪��ԭ��ĩβ��λ��������� (�ѽ����str16�������ڲ�Ϊ���ֵ���ʶ�����и�ֵ��,ʮ���������ĺ�һλ��һ����'\0'��������������� 
	round=len/2;
//	cout<<round<<endl;
	
	for(i=0;i<round;i++)
	{
		temp=str16[i];
		str16[i]=str16[len-1-i];
		str16[len-1-i]=temp;
	}
	//cout<<"str16="<<str16<<endl;
return str16;
//}
}
