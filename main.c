#include<reg52.h>  
#define	uint unsigned int
#define uchar unsigned char

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

sbit	smg_1 = P2^3;			//�����1
sbit	smg_2 = P2^4;		    //�����2
sbit	key = P3^2;				//��ʼ��ͣ����

#define	SMG_XS	P0				//�����������ʾ����������ܣ�
uchar code Data[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
//��������  �����   0    1    2    3    4    5    6    7    8    9    -
uchar time_cl = 0;				//����ʱ����Ʊ�־λ  ȡ0
uchar time_num = 0;				//������ʼֵ
uchar flag_master = 1;			//���ɱ�־λ
uchar flag_slave = 0;			//���ɻƵƱ�־λ
uchar flag_m_s = 0;				//֧·��־λ
uchar flag_s_m = 0;				//֧·�ƵƱ�־λ

void delay(uint time);			//ɨ����ʱ����
void south_north();				//����ͨ30s���ϱ�·�ߣ�
void east_west();				//֧·ͨ30s������·�ߣ�								
void master_to_slave();		    //����ת��֧·5s(���ɻƵ�)
void slave_to_master();		    //֧·ת������5s��֧·�Ƶƣ�
void Time0_init();				//��ʱ��0��ʼ��
void light_init();				//�ƹ��ʼ��
void master_or_slaver(char flag_num);	//���̵�
void yellow_light(char flag_num);		//�Ƶ�
void Int0Init();				//�ⲿ�ж�0��ʼ��

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

void delay(uint time)			//ɨ����ʱ����
{
	uint x,y;
	for(x = time; x > 0; x --)
	for(y = 110; y > 0; y --);		
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

//֧·ͨ20s������·�ߣ�
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

//�ƹ��ʼ�������е����Ƶƣ��������ʾ--��
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

void Time0_init()				    //��ʱ��0��ʼ������
{
	TMOD = 0x01;					//��ʱ��0Ϊ��ʽ1
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;		//12M���� ��ʱʱ��50ms
	ET0 = 1;						//����ʱ��0�ж�
	EA = 1;						    //�����ж�
	//TR0 = 1;						//������ʱ��0
}

void Int0Init()
{
	IT0 = 1;						//�����س�����ʽ���½��أ�
	EX0 = 1;						//��INT0���ж�����
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
		TR0 =~ TR0;						//��������ͣ��ʱ��0
		delay(3);
	}
}

void Time0() interrupt 1			//��ʱ��0�жϷ�����
{	
	static char flag_num1 = 29;		//���ɵ���ʱ
	static char flag_num2 = 4;	    //�ƵƵ���ʱ
	static char flag_num3 = 19;	    //֧·����ʱ

	TH0 = (65536-50000)/256;		//������װ��ֵ,����50ms�ж�һ��
	TL0 = (65536-50000)%256;
	time_num ++;					//ʱ�������1
	if(flag_master)    				// ����ͨ
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
	if(flag_m_s)					//����ת֧·
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
	if(flag_slave)					//֧·ͨ	
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
	if(flag_s_m)					//֧·ת����
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
