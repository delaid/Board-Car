#include "encrypt.h"




uint8_t MCU_UID[12];			/*Ψһ�豸ID*/
uint8_t KEY_UID[12];			/*�����ܳ�*/
uint8_t Password[12];			/*�����ܳ�*/

/*��ȡMCUΨһ�豸ID*/
void Read_MCUUID(void)
{
	uint8_t *pt;
	uint8_t  i;
	
	pt = (uint8_t *)MCU_UID_BASE_ADD;
	for(i=0;i<12;i++)
	{
		MCU_UID[i] = *pt;
		pt++;
	}
}

/*��ȡ���̼��ܽ��*/
void Read_KEYUID(void)
{
	uint8_t *pt;
	uint8_t  i;
	
	pt = (uint8_t *)KEY_UID_BASE_ADD;
	for(i=0;i<12;i++)
	{
		KEY_UID[i] = *pt;
		pt++;
	}
}

/*�����㷨*/
void Encryption_Algorithmic(void)
{
	uint8_t i;
	
	for(i=0;i<12;i++)
	{
		Password[i]  = MCU_UID[i];
	}
	
	PASSWORD_01();
	PASSWORD_02();
	PASSWORD_03();
	PASSWORD_04();
	PASSWORD_05();
	PASSWORD_06();
	PASSWORD_07();
	PASSWORD_08();
	PASSWORD_09();
	PASSWORD_10();
	PASSWORD_11();
	PASSWORD_12();
}


/*�ܳ����*/
bool Project_Decrypt(void)
{
	uint8_t i;
	
	for(i=0;i<12;i++)
	{
		if(Password[i] != KEY_UID[i])
		{
			return false;
		}
	}
	
	return true;
}

/*���򱣻�*/
void Program_Protection(void)
{
	bool flag;
	
	Read_MCUUID();				//��ȡMCUΨһ�豸ID
	Read_KEYUID();				//��ȡ���̼����ܳ�
	Encryption_Algorithmic();	//���ɹ��̼����ܳ�
	flag = Project_Decrypt();	//�ܳ����
	
	if(!flag)
		while(1);				//�ܳ����ʧ�ܣ����������������������
}					















