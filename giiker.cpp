#include<iostream>
#include<cstring>
#include<ctime>
#include<cmath>
#include<conio.h>
#include<map>
#include<Windows.h>
#include "stdlib.h"
#include<bits/stdc++.h>
#define DATALEN 10000
using namespace std;

class ScreenBuff{
public:
	HANDLE hOutput;
	HANDLE hOutBuf;
	COORD coord={0,0};
	DWORD bytes;
	bool isOn;
	char data[DATALEN];
	ScreenBuff(){
		bytes = 0;
		hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
		hOutBuf = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, 
			CONSOLE_TEXTMODE_BUFFER, 
			NULL
			);
		CONSOLE_CURSOR_INFO cci;
		cci.bVisible=0;
		cci.dwSize=1;
		SetConsoleCursorInfo(hOutput, &cci);
		SetConsoleCursorInfo(hOutBuf, &cci);
	}
	void on()
	{//开启双缓冲后，在输出后必须调用update才能显示 
		isOn = true;
		SetConsoleActiveScreenBuffer(hOutBuf);
	}
	void off()
	{//关闭双缓冲 
		isOn = false;
		SetConsoleActiveScreenBuffer(hOutput);
	}
	void update()
	{//更新屏幕 
		if(isOn)
		{
			ReadConsoleOutputCharacterA(hOutput, data, DATALEN, coord, &bytes);
			WriteConsoleOutputCharacterA(hOutBuf, data, DATALEN, coord, &bytes);
		}
	}
	void clear()//清空屏幕 
	{
		CONSOLE_SCREEN_BUFFER_INFO cinfo;//用于储存控制台缓冲区信息,在这里主要获取控制台缓冲区大小
		DWORD recnum;
		GetConsoleScreenBufferInfo(hOutput, &cinfo);//cinfo.dwSize储存的是缓冲区大小//cinfo.dwSize.X * cinfo.dwSize.Y 即需填充的字符数
		FillConsoleOutputCharacterW(hOutput,  L' ', cinfo.dwSize.X * cinfo.dwSize.Y, (COORD){0, 0}, &recnum);//从{0,0}处开始填充' '字符,成功填充个数为recnum
		FillConsoleOutputAttribute(hOutput, 0, cinfo.dwSize.X * cinfo.dwSize.Y, (COORD){0, 0}, &recnum);//设置输出颜色,如果不是单一颜色,可能会有清除字符而背景色没有清除的现象
		SetConsoleCursorPosition(hOutput, (COORD){0, 0});//将光标设为{0,0}
		update();	
	}
}scrbuf;

map<int,int>f;
map<int,int>FS;
int n,a[50][50],tot,l,l2;
int dx[5]={0,-1,0,1,0},dy[5]={0,0,1,0,-1};

struct node
{
	int x,y;
};
string its(int kk)
{
	string s11="",s22="";
	while(kk>0)
	{
		s11+=(kk%10)+'0';
		kk/=10;
	}
	kk=s11.size();
	for(int i=kk-1;i>=0;i--)
		s22+=s11[i];
	return s22;
}
bool check()
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			if(a[i][j]!=(i-1)*n+j && (i-1)*n+j!=n*n)
				return false;
	return true;
}
int len(int x)
{
	if(x==0)	return 1;
	int num=0;
	while(x!=0)
		num++,x/=10;
	return num;
}
void print()
{
	string sp="";
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=n;j++)
		{
			if(a[i][j]==0)
				sp+="  ";
			else
				sp+=its(a[i][j])+" ";
			for(int k=len(a[i][j]);k<l;k++)
				sp+=" ";
		}
	sp+="\n";
	}
	scrbuf.clear();
	printf("Press WADS to control.\n%s",sp.c_str());
	scrbuf.update();
	return ;
}
void make()
{
	srand((unsigned)time(NULL));
	memset(a,0,sizeof(a));
	int rd=0,r=0,cnt=0;
	
	scrbuf.clear();
	printf("  Enter a number (3~10),it will depend the size.\n");
	scrbuf.update();
	cin>>n;
	
	scrbuf.clear();
	printf("  Entry a number.If the number is 0,the seed will be randomly generated.\n");
	scrbuf.update();
	cin>>rd;
	
	if(rd==0)
		rd=rand()%10086+13;
	r=rd%100;
	
	for(int i=1;i<=114514;i++)
	{
		FS[i]=abs((i*rd%192-rd%23)%5);
		if(FS[i]==0)
			FS[i]=(rd%4+1)%5;
		if(rd>200)
			rd-=r;
		else rd+=r;
	}
	
	l=len(n*n);
	int szs[101],t=0;
	for(int i=1;i<=n*n;i++) 
		szs[i]=i;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
			a[i][j]=szs[++t];
	
	a[n][n]=0;
	node rxy,ttmp;
	rxy.x=n,rxy.y=n;
	
	for(int i=1;i<=114514;i++)
	{
		ttmp.x=rxy.x+dx[FS[++cnt]],ttmp.y=rxy.y+dy[FS[cnt]];
		if(ttmp.x>0 && ttmp.x<=n && ttmp.y>0 && ttmp.y<=n)
			swap(a[rxy.x][rxy.y],a[ttmp.x][ttmp.y]),swap(rxy,ttmp);
	}
	return;
}

int main()
{
	scrbuf.on();
	f['s']=1,f['a']=2;f['w']=3,f['d']=4;
	while(1)
	{
		make();
		node nw;
		for(int i=1;i<=n;i++)
			for(int j=1;j<=n;j++)
				if(a[i][j]==0)
					nw.x=i,nw.y=j;
		while(1)
		{
			print();
			char fc=getch();
			if(fc==27)
			{
				scrbuf.clear();
				printf("Press a letter :\n [r]:return the game.\n [a]:Restart the game.\n [e]:Exit the game.\n");
				scrbuf.update();
				
				string fc3;
				fc3=getch();
				
				if(fc3[0]=='r') 
					continue;
				if(fc3[0]=='a') 
					break;
				if(fc3[0]=='e') 
					return 0;
				
				print();
				continue;
			}
			if(fc!='w' && fc!='a' && fc!='s' && fc!='d' ) 
				continue;
			node tmp;
			tmp.x=nw.x+dx[f[(int)fc]];
			tmp.y=nw.y+dy[f[(int)fc]];
			if(tmp.x>0 && tmp.x<=n && tmp.y>0 && tmp.y<=n)
			{
				swap(a[nw.x][nw.y],a[tmp.x][tmp.y]),swap(nw,tmp);
				tot++;
				print();
				if(check())
				{
					//scrbuf.clear();
					printf("You win the game!\n");
					printf("The total steps is %d!\n",tot);
					printf("Enter a letter: [a]:Restart the game  [Letters other than a]:Exit the game.\n");
					scrbuf.update();
					tot=0;
					char fc1;
					fc1=getch();
					if(fc1=='a') 
						break;
					else 
						return 0;
				}
			}
		}
	}
	return 0;
}
//version:1.1.1
