#include<iostream>
#include<cstring>
#include<ctime>
#include<cmath>
#include<ege.h>
#include<map>
#include<Windows.h>
#include "stdlib.h"
#include<bits/stdc++.h>
#define DATALEN 10000
#define ll long long
using namespace std;
using namespace ege;

map<ll,ll>f;
map<ll,ll>FS;
ll n,a[50][50],tot,l,l2;
ll dx[5]={0,-1,0,1,0},dy[5]={0,0,1,0,-1}; //定义变量
ll __seed,__size;
char pri[10000],kz[5000];

struct node
{
	ll x,y;
};

int _read(string s) //键盘输入
{
	char a[100],b;
	int numb=0;
	sprintf(a,"%s\n",s.c_str()); //输出提示
	cleardevice();
	outtextxy(10,10,a);
	while(1)
	{
		b=getch();
		if(b==8)   //如果是Back键就减掉最后一位
			numb=numb/10;
		else if(b>='0' && b<='9')  //如是数字则累加
			numb=numb*10+b-'0';
		else if(b==13)break;  //如果是回车就结束
		sprintf(a,"%s",s.c_str());
		cleardevice();
		outtextxy(10, 10,a);
		if(numb!=0)
			sprintf(a,"%d",numb);
		else sprintf(a," ");
		outtextxy(10, 30,a);
	}
	return numb;
}

string its(ll kk)  //int类型转string类型
{
	string s11="",s22="";
	while(kk>0)
	{
		s11+=(kk%10)+'0';
		kk/=10;
	}
	kk=s11.size();
	for(ll i=kk-1;i>=0;i--)
		s22+=s11[i];
	return s22;
}

bool check()  //检查是否完成游戏
{
	for(ll i=1;i<=n;i++)
		for(ll j=1;j<=n;j++)
			if(a[i][j]!=(i-1)*n+j && (i-1)*n+j!=n*n)
				return false;
	return true;
}

ll len(ll x)  //用于计算输出时数字之间的空格数量
{
	if(x==0)	return 1;
	ll num=0;
	while(x!=0)
		num++,x/=10;
	return num;
}

void print()      //将内容输出到屏幕
{
	string sp;
	sprintf(pri,"Press WADS to control.\n%s",sp.c_str());
	outtextxy(10,10,pri);
	for(ll i=1;i<=n;i++)
	{
		sp="";
		for(ll j=1;j<=n;j++)
		{
			if(a[i][j]==0)
				sp+="  ";
			else
				sp+=its(a[i][j])+" ";
			for(ll k=len(a[i][j]);k<l;k++)
				sp+=" ";
		}
		outtextxy(10,i*15+10,sp.c_str());
	}
	return ;
}

int seed1(int ab)
{
	while(ab<100000000)
		ab*=random(4)+random(100);
	if(ab>=1000000000)
		ab=ab/10;
	return ab;
}

void make()   //生成华容道
{
	memset(a,0,sizeof(a));
	ll rd=0,r=0,cnt=0;
	
	do
	{
		sprintf(pri,"Enter a number (2~11),it will depend the size.\n");   //输入华容道规模
		n=_read(pri);
		__size=n;
	}while(n<2 || n>11); //安全门

	sprintf(pri,"Enter a nine-digit number.If this number is not a nine-digit number,the seed will be randomly generated.\n");
	rd=_read(pri);    //输入种子，若为0则随机生成
	
	if(rd<100000000 || rd>999999999)
		rd=random(999999999);
	rd=seed1(rd);
	r=rd%211+3;
	__seed=rd;
	
	for(int i=1;i<=114514;i++)   //根据种子生成有且仅有[1,2,3,4]数字的FS数组
	{
		FS[i]=abs((i*rd%1921-rd%243)%5);
		if(FS[i]==0)
			FS[i]=(rd%4+1)%5;
		if(rd>200000)
			rd-=r;
		else rd+=r;
		r=rd%985+2;
	}
	
	l=len(n*n);
	ll szs[122],t=0;
	for(ll i=1;i<=n*n;i++) 
		szs[i]=i;
	for(ll i=1;i<=n;i++)
		for(ll j=1;j<=n;j++)
			a[i][j]=szs[++t];  //生成原始华容道
	
	a[n][n]=0;
	node rxy,ttmp;
	rxy.x=n,rxy.y=n;
	
	for(ll i=1;i<=114514;i++)
	{
		ttmp.x=rxy.x+dx[FS[++cnt]],ttmp.y=rxy.y+dy[FS[cnt]];
		if(ttmp.x>0 && ttmp.x<=n && ttmp.y>0 && ttmp.y<=n)
			swap(a[rxy.x][rxy.y],a[ttmp.x][ttmp.y]),swap(rxy,ttmp);      //根据FS数组生成无序的华容道
	}
	return;
}

int main()
{
	randomize();//随机数初始化
	initgraph(1040, 680, INIT_ANIMATION);//设置窗口 INIT_ANIMATION为游戏用
	setcaption("Number-Klotski");	//设置窗口标题
	setbkcolor(WHITE);  //设置背景颜色
	setfont(-16, 0,"微软雅黑"); //设置字体大小
	setcolor(EGERGB(0, 0, 0)); //设置字体颜色
	/*scrbuf.on();*/
	f['s']=1,f['a']=2;f['w']=3,f['d']=4;   //wasd键盘控制
	while(1)  //程序主循环
	{
		make();
		node nw;
		for(ll i=1;i<=n;i++)
			for(ll j=1;j<=n;j++)
				if(a[i][j]==0)
					nw.x=i,nw.y=j;
		while(1)        //游戏主循环
		{
			cleardevice();
			print();
			char fc=getch();
			  //读取键盘操作
			if(fc==27)   //若按下Esc，呼出菜单
			{
				sprintf(kz,"Press a key:\n [r]:return the game.\n [a]:Restart the game.\n [e]:Exit the game.\n");
				cleardevice();
				outtextxy(10,10,kz);
				sprintf(kz,"-------------------------\n  The seed is:%d   The size is:%d  \n-------------------------",__seed,__size);
				outtextxy(10,30,kz);
				char fc3;
				fc3=getch();
				
				if(fc3=='r') 
					continue;  	//若按下 r 返回游戏
				if(fc3=='a') 
					break;          //若按下 a 重新开始
				if(fc3=='e') 
					return 0;      //若按下 e 退出程序
				
				print();
				continue;
			}
			
			if(fc!='w' && fc!='a' && fc!='s' && fc!='d' ) 
				continue;     //防止乱按
			
			node tmp;
			tmp.x=nw.x+dx[f[(ll)fc]];
			tmp.y=nw.y+dy[f[(ll)fc]];
			
			if(tmp.x>0 && tmp.x<=n && tmp.y>0 && tmp.y<=n)
			{
				swap(a[nw.x][nw.y],a[tmp.x][tmp.y]),swap(nw,tmp);   //根据按下的按键互换空格和数字
				tot++;
				print();
				if(check())  //检查是否完成排序
				{
					cleardevice();
					char pr[100];
					sprintf(pr,"You win the game!\n");
					outtextxy(10,10,pr);
					sprintf(pr,"The total steps is %d!\n",tot);
					outtextxy(10,30,pr);
					sprintf(pr,"-------------------------\n  The seed is:%d   The size is:%d  \n-------------------------\n",__seed,__size);
					outtextxy(10,50,pr);
					sprintf(pr,"Enter a letter: [a]:Restart the game  [s]:Exit the game.\n");
					outtextxy(10,70,pr);
					
					tot=0;
					char fc1;
					fc1=getch();
					if(fc1=='a')   //若按下 a 则重新开始游戏
						break;
					else 
						return 0;  //若按下其他按键则退出程序
				}
			}
		}
	}
	return 0;
}
//version:1.2.0
