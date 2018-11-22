#include<reg52.h>  

#define	uint unsigned int
#define uchar unsigned char

/***************����  ����̵�**************/
sbit    n_r = P1^2;				//����  ���
sbit    n_y = P1^1;				//����  �Ƶ�
sbit    n_g = P1^0;				//����	�̵�

sbit    s_r = P2^0;				//����	���
sbit    s_y = P2^1;				//����	�Ƶ�
sbit	s_g = P2^2;				//����	�̵�

sbit    w_r = P2^5;				//����	���
sbit    w_y = P2^6;				//����	�Ƶ�
sbit    w_g = P2^7;				//����	�̵�

sbit    e_r = P3^7;				//����  ���
sbit    e_y = P3^6;				//����	�Ƶ�
sbit    e_g = P3^5;				//����  �̵�
sbit	smg_1 = P2^3;
sbit	smg_2 = P2^4;

/***************����  �����*****************/
#define	SMG_XS	P0				//�����������ʾ

uchar code Data[]={0x18,0x7B,0x89,0x49,0x6A,0x4C,0x0C,0x79,0x08,0x48,0xEF};
//��������  �����   0    1    2    3    4    5    6    7    8    9    -
uchar time_cl = 0;				//����ʱ����Ʊ�־λ  ȡ0
uchar time_num = 0;				//������ʼֵ
uchar flag_master = 1;
uchar flag_slave = 0;
uchar flag_m_s = 0;
uchar flag_s_m = 0;

void delay(uint time);			//ɨ����ʱ����
void south_north();				//����ͨ30s���ϱ�·�ߣ�
void east_west();				//֧·ͨ30s������·�ߣ�								
void master_to_slave();		    //����ת��֧·5s(���ɻƵ�)
void slave_to_master();		    //֧·ת������5s��֧·�Ƶƣ�
void Time0_init();				//��ʱ��0��ʼ��

void main()
{	smg_1 = 1;
	smg_2 = 1;
	Time0_init();
 while(1)
 {
 	;
 }
}


void delay(uint time)			//ɨ����ʱ����
{
	uint x,y;
	for(x=time;x>0;x--)
	for(y=110;y>0;y--);		
}

//����ͨ30s���ϱ�·�ߣ�
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

//֧·ͨ30s������·�ߣ�
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

//����ת��֧·5s(���ɻƵ�)
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

//֧·ת������5s��֧·�Ƶƣ�
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

void Time0_init()				//��ʱ��0��ʼ������
{
	TMOD = 0x01;					//��ʱ��0Ϊ��ʽ1
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;		//12M���� ��ʱʱ��50ms
	ET0 = 1;						//����ʱ��0�ж�
	EA = 1;						//�����ж�
	TR0 = 1;						//������ʱ��0
}



void Time0() interrupt 1						//��ʱ��0�жϷ�����
{
	//static uchar time = 0;
	static uchar flag_num = 0;
//	uchar m_s_time = 0;
//	uchar s_m_time = 0;
//	uchar slave_time = 0;

	TH0 = (65536-50000)/256;						//������װ��ֵ,����50ms�ж�һ��
	TL0 = (65536-50000)%256;
	time_num ++;									//ʱ�������1
	if(flag_master) // ����
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
