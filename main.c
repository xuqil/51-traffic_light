#include<reg52.h>  

#define	uint unsigned int
#define uchar unsigned char

/***************定义  红黄绿灯**************/
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
sbit	smg_1 = P2^3;
sbit	smg_2 = P2^4;

/***************定义  数码管*****************/
#define	SMG_XS	P0				//数码管数据显示

uchar code Data[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77};
//定义数组  数码管   0    1    2    3    4    5    6    7    8    9    A
uchar time_cl = 0;				//定义时间控制标志位  取0
uchar time_num = 0;				//计数初始值
uchar flag_master = 1;
uchar flag_slave = 0;
uchar flag_m_s = 0;
uchar flag_s_m = 0;

void delay(uint time);			//扫描延时函数
void south_north();				//主干通30s（南北路线）
void east_west();				//支路通30s（东西路线）								
void master_to_slave();		    //主干转到支路5s(主干黄灯)
void slave_to_master();		    //支路转到主干5s（支路黄灯）
void Time0_init();				//定时器0初始化

void main()
{
	smg_1 = 1;
	smg_2 = 1;
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
	for(x=time;x>0;x--)
	for(y=110;y>0;y--);		
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

//支路通30s（东西路线）
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

void Time0_init()				    //定时器0初始化函数
{
	TMOD = 0x01;					//定时器0为方式1
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;		//12M晶振 定时时间50ms
	ET0 = 1;						//开定时器0中断
	EA = 1;						    //开总中断
	TR0 = 1;						//启动定时器0
}



void Time0() interrupt 1					//定时器0中断服务函数
{
	static char flag_num2 = 4;
	static char flag_num1 = 29;

	TH0 = (65536-50000)/256;				//重新载装初值,设置50ms中断一次
	TL0 = (65536-50000)%256;
	time_num ++;							//时间基数加1
	if(flag_master)    						// 主干
	{	 
		south_north();	
	 	smg_1 = 0;
		SMG_XS = Data[flag_num1/10];
		delay(3);
		smg_1 = 1; 

		smg_2 = 0;
		SMG_XS = Data[flag_num1%10];
		delay(3); 	
		smg_2 = 1;

		if(time_num == 20)//1s
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
	if(flag_m_s)
	{		 
		master_to_slave();
		smg_1 = 0;
		SMG_XS = Data[flag_num2/10];
		delay(3);
		smg_1 = 1; 
	
		smg_2 = 0;
		SMG_XS = Data[flag_num2%10];
		delay(3); 	
		smg_2 = 1;

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
	if(flag_slave)
	{		 
		east_west();
		smg_1 = 0;
		SMG_XS = Data[flag_num1/10];
		delay(3);
		smg_1 = 1; 

		smg_2 = 0;
		SMG_XS = Data[flag_num1%10];
		delay(3); 	
		smg_2 = 1;

		  if(time_num == 20)//1s
		 {
		 	time_num = 0;
		 	flag_num1 --;
			 if(flag_num1 == 0)
			 {
			 	time_num = 0;
				flag_num1 = 29;
				flag_slave = 0;
				flag_s_m = 1;
			 }
		 }
	}
	if(flag_s_m)
	{		 
		slave_to_master();
		smg_1 = 0;
		SMG_XS = Data[flag_num2/10];
		delay(3);
		smg_1 = 1; 
	
		smg_2 = 0;
		SMG_XS = Data[flag_num2%10];
		delay(3); 	
		smg_2 = 1;

		 if(time_num == 20)
		 {
		 	flag_num2 --;
			time_num = 0;
			if(flag_num2 == 0)
			{
			 	time_num = 0;
				flag_s_m = 0;
				flag_master = 1;
				flag_num2 = 4;
			}
		 }
	}

}
