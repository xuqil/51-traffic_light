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

uchar code Data[]={0x18,0x7B,0x89,0x49,0x6A,0x4C,0x0C,0x79,0x08,0x48,0xEF};
//定义数组  数码管   0    1    2    3    4    5    6    7    8    9    -
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
{	smg_1 = 1;
	smg_2 = 1;
	Time0_init();
 while(1)
 {
 	;
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

void Time0_init()				//定时器0初始化函数
{
	TMOD = 0x01;					//定时器0为方式1
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;		//12M晶振 定时时间50ms
	ET0 = 1;						//开定时器0中断
	EA = 1;						//开总中断
	TR0 = 1;						//启动定时器0
}



void Time0() interrupt 1						//定时器0中断服务函数
{
	//static uchar time = 0;
	static uchar flag_num = 0;
//	uchar m_s_time = 0;
//	uchar s_m_time = 0;
//	uchar slave_time = 0;

	TH0 = (65536-50000)/256;						//重新载装初值,设置50ms中断一次
	TL0 = (65536-50000)%256;
	time_num ++;									//时间基数加1
	if(flag_master) // 主干
	{		 
		 south_north();
		 if(time_num == 20)//1s
		 {
		 	time_num = 0;
		 	flag_num ++;
			if(flag_num == 6)
			{
				time_num = 0;
				flag_num = 0;
				flag_master = 0;
				flag_m_s = 1;
			}
		 }
	}
	if(flag_m_s)
	{		 
		 master_to_slave();
		 if(time_num == 100)
		 {
		 	time_num = 0;
			flag_m_s = 0;
			flag_slave = 1;
		 }
	}
	if(flag_slave)
	{		 
		 east_west();
		  if(time_num == 20)//1s
		 {
		 	time_num = 0;
		 	flag_num ++;
			 if(flag_num == 60)
			 {
			 	time_num = 0;
				flag_num = 0;
				flag_slave = 0;
				flag_s_m = 1;
			 }
		 }
	}
	if(flag_s_m)
	{		 
		 slave_to_master();
		 if(time_num == 100)
		 {
		 	time_num = 0;
			flag_s_m = 0;
			flag_master = 1;
		 }
	}

}
