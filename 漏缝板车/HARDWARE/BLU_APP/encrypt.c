#include "encrypt.h"




uint8_t MCU_UID[12];			/*唯一设备ID*/
uint8_t KEY_UID[12];			/*加密密匙*/
uint8_t Password[12];			/*加密密匙*/

/*读取MCU唯一设备ID*/
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

/*读取工程加密结果*/
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

/*加密算法*/
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


/*密匙配对*/
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

/*程序保护*/
void Program_Protection(void)
{
	bool flag;
	
	Read_MCUUID();				//读取MCU唯一设备ID
	Read_KEYUID();				//读取工程加密密匙
	Encryption_Algorithmic();	//生成工程加密密匙
	flag = Project_Decrypt();	//密匙配对
	
	if(!flag)
		while(1);				//密匙配对失败，程序卡在这里，建议设置陷阱
}					















