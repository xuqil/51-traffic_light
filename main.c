#include<reg52.h>  
#define	uint unsigned int
#define uchar unsigned char

sbit    n_r = P1^2;				//北面  红灯
sbit    n_y = P1^1;				//北面  黄灯
sbit    n_g = P1^0;				//北面	绿灯

sbit    s_r = P2^0;				//南面	红灯
sbit    s_y = P2^1;				//南面	黄灯
sbit	s_g = P2^2;				//南面	绿灯

sbit    w_r = P2^5;				//西面	红灯
sbit    w_y = P2^6;				//西面	黄灯
sbit    w_g = P2^7;				//西面	绿灯

sbit    e_r = P3^7;				//东面  红灯
sbit    e_y = P3^6;				//东面	黄灯
sbit    e_g = P3^5;				//东面  绿灯

sbit	smg_1 = P2^3;			//数码管1
sbit	smg_2 = P2^4;		    //数码管2
sbit	key = P3^2;				//开始暂停按键

#define	SMG_XS	P0				//数码管数据显示（共阴数码管）
uchar code Data[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
//定义数组  数码管   0    1    2    3    4    5    6    7    8    9    -
uchar time_cl = 0;				//定义时间控制标志位  取0
uchar time_num = 0;				//计数初始值
uchar flag_master = 1;			//主干标志位
uchar flag_slave = 0;			//主干黄灯标志位
uchar flag_m_s = 0;				//支路标志位
uchar flag_s_m = 0;				//支路黄灯标志位

void delay(uint time);			//扫描延时函数
void south_north();				//主干通30s（南北路线）
void east_west();				//支路通30s（东西路线）								
void master_to_slave();		    //主干转到支路5s(主干黄灯)
void slave_to_master();		    //支路转到主干5s（支路黄灯）
void Time0_init();				//定时器0初始化
void light_init();				//灯光初始化
void master_or_slaver(char flag_num);	//红绿灯
void yellow_light(char flag_num);		//黄灯
void Int0Init();				//外部中断0初始化

void main()
{
	light_init();
	Int0Init();
	Time0_init();
	SMG_XS=Data[10];
	delay(300);
	 while(1)
	 {
	 }
}

void delay(uint time)			//扫描延时函数
{
	uint x,y;
	for(x = time; x > 0; x --)
	for(y = 110; y > 0; y --);		
}

//主干通30s（南北路线）
void south_north()
{
    n_r = 1;
	n_y = 1;
	n_g = 0;

	s_r = 1;
	s_y = 1;
	s_g = 0;

	e_r = 0;
	e_y = 1;
	e_g = 1;

	w_r = 0;
	w_y = 1;
	w_g = 1;
}

//支路通20s（东西路线）
void east_west()
{
	e_r = 1;
	e_y = 1;
	e_g = 0;

	w_r = 1;
	w_y = 1;
	w_g = 0;

    n_r = 0;
	n_y = 1;
	n_g = 1;

	s_r = 0;
	s_y = 1;
	s_g = 1;

}

//主干转到支路5s(主干黄灯)
void master_to_slave()
{
	e_r = 1;
	e_y = 1;
	e_g = 1;

	w_r = 1;
	w_y = 1;
	w_g = 1;

    n_r = 1;
	n_y = 0;
	n_g = 1;

	s_r = 1;
	s_y = 0;
	s_g = 1;
}

//支路转到主干5s（支路黄灯）
void slave_to_master()
{
	e_r = 1;
	e_y = 0;
	e_g = 1;

	w_r = 1;
	w_y = 0;
	w_g = 1;

    n_r = 1;
	n_y = 1;
	n_g = 1;

	s_r = 1;
	s_y = 1;
	s_g = 1;
}

//灯光初始化（所有灯亮黄灯，数码管显示--）
void light_init()
{
	e_r = 1;
	e_y = 0;
	e_g = 1;

	w_r = 1;
	w_y = 0;
	w_g = 1;

    n_r = 1;
	n_y = 0;
	n_g = 1;

	s_r = 1;
	s_y = 0;
	s_g = 1;

	
	smg_1 = 0;
	smg_2 = 1;
	SMG_XS = Data[10];
	delay(3);
	smg_1 = 1;

	smg_1 = 1;
	smg_2 = 0;
	SMG_XS = Data[10];
	delay(3);
	smg_2 = 1;

	delay(3);
	smg_1 = 0;
	smg_2 = 0;

}

void Time0_init()				    //定时器0初始化函数
{
	TMOD = 0x01;					//定时器0为方式1
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;		//12M晶振 定时时间50ms
	ET0 = 1;						//开定时器0中断
	EA = 1;						    //开总中断
	//TR0 = 1;						//启动定时器0
}

void Int0Init()
{
	IT0 = 1;						//跳变沿出发方式（下降沿）
	EX0 = 1;						//打开INT0的中断允许
	EA = 1;
}

void master_or_slaver(char flag_num)
{
	smg_1 = 0;
	SMG_XS = Data[flag_num/10];
	delay(3);
	smg_1 = 1; 

	smg_2 = 0;
	SMG_XS = Data[flag_num%10];
	delay(3); 	
	smg_2 = 1;
}

void yellow_light(char flag_num)
{
	smg_1 = 0;
	SMG_XS = Data[flag_num/10];
	delay(3);
	smg_1 = 1; 

	smg_2 = 0;
	SMG_XS = Data[flag_num%10];
	delay(3); 	
	smg_2 = 1;
}

void Int0() interrupt 0
{
	delay(3);
	if(key == 0)
	{
		TR0 =~ TR0;						//启动和暂停定时器0
		delay(3);
	}
}

void Time0() interrupt 1			//定时器0中断服务函数
{	
	static char flag_num1 = 29;		//主干倒计时
	static char flag_num2 = 4;	    //黄灯倒计时
	static char flag_num3 = 19;	    //支路倒计时

	TH0 = (65536-50000)/256;		//重新载装初值,设置50ms中断一次
	TL0 = (65536-50000)%256;
	time_num ++;					//时间基数加1
	if(flag_master)    				// 主干通
	{	 
		south_north();	
		master_or_slaver(flag_num1);
		if(time_num == 20)			//1s
		{ 	
			flag_num1 --;
			time_num = 0;
			if(flag_num1 < 0)
			{
				time_num = 0;
				flag_num1 = 29;
				flag_master = 0;
				flag_m_s = 1;
			}
		 }
	}
	if(flag_m_s)					//主干转支路
	{		 
		 master_to_slave();
		 yellow_light(flag_num2);
		 if(time_num == 20)
		 {
			 flag_num2 --;
			 time_num = 0;
			if(flag_num2 < 0)
			{
				time_num = 0;
				flag_m_s = 0;
				flag_slave = 1;	
				flag_num2 = 4;
			}
		 }
	}
	if(flag_slave)					//支路通	
	{		 
		east_west();
		master_or_slaver(flag_num3);
		if(time_num == 20)			//1s
		 {
		 	time_num = 0;
		 	flag_num3 --;
			 if(flag_num3 < 0)
			 {
			 	time_num = 0;
				flag_num3 = 19;
				flag_slave = 0;
				flag_s_m = 1;
			 }
		 }
	}
	if(flag_s_m)					//支路转主干
	{		 
		slave_to_master();
		 yellow_light(flag_num2);
		 if(time_num == 20)
		 {
		 	flag_num2 --;
			time_num = 0;
			if(flag_num2 < 0)
			{
			 	time_num = 0;
				flag_s_m = 0;
				flag_master = 1;
				flag_num2 = 4;
			}
		 }
	}
}
