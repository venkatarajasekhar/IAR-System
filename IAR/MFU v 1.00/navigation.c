/*******************************************************************************
********************************************************************************
**                                                                            **
**          ���������� ������� ������ ���������� FSATREX IT600                **
**                                                                            **
********************************************************************************
*******************************************************************************/

//******************������������ �����******************************************

#include "navigation.h"
#include "main.h"
#include <string.h>
#include <ctype.h>
//*************������������� ���������� ����������******************************

char Timestamp[6][11]; //����� ������ ���������
char Latitude[6][9]; //������ 
char Longitude[6][10]; //�������
char Speed[6][6]; //��������
char Date[6][7]; //���� 

uint8_t Navi_ResetCounter; //������� ��������� ������ ����������

               /*������� ������ � ������� ������� ��� ����������*/
const char NAVI_Comand1[] = "$PSTMSETPAR,1201,0040*1D\r\n"; //�������� ������ ��������� RMC
const char NAVI_Comand2[] = "$PSTMSAVEPAR*58\r\n";  //��������� ��������� � ����������������� ������



//**************������� ��� ������ � �����������********************************
void NAVI_Configuration(void)   //������������� �������������� ���������
{
  
 IWDG_ReloadCounter(); //����� �������� ����������� ������� 
 delay_ms(60); //�������� 60�� ����� ������� ������
 GPIO_SetBits(NAVIGATOR , NAVI_STANDBAY); //��������� ����� �������� ����������
 GPIO_SetBits(NAVIGATOR , NAVI_RESET); //������� ��������� �� ��������� ������
 delay_ms(5000);
 
 SendString_InUnit(NAVI_Comand1 , Navigator); //�������� ������� "������ RMC"
 delay_ms(10); //�������� 10��

 if (strstr(Navi_RxBuffer , "$PSTMSETPAROK")!=NULL) //���� ����� �������������
 {
  SendString_InUnit(NAVI_Comand2 , Navigator); //�������� ������� ���������� ���������� 
 }
 
 STATUS.NavigatorStatus = ACTIVE; //������ ���������� �������

 IWDG_ReloadCounter(); //����� �������� ����������� ������� 
 
 Navi_ResetCounter = 0; //����� ��������
}
//================================================================================
void NAVI_Reset(void) //������� ������ ����������
{
  STATUS.NavigatorStatus = INACTIVE; //������ ���������� ���������
  
  GPIO_ResetBits(NAVIGATOR , NAVI_RESET);//���������� ���������
  delay_ms(500);
  GPIO_SetBits(NAVIGATOR , NAVI_RESET); //������� ��������� �� ��������� ������
  delay_ms(1000);
  STATUS.NavigatorStatus = ACTIVE; //������ ���������� �������
  Navi_ResetCounter = 0; //����� ��������
}
//======================================================================================
void ReadCoordinates(void) //������� ���������� ���������
{
char *s; //��������� ��������� ����������
uint8_t i; //�������

s = strstr(Navi_RxBuffer , "$GPRMC"); //���� � ������ ��������� ��������� RMC
if(s!=NULL)                               //���� ������ � ������ ������������
{
  
 // if(strchr(Navi_RxBuffer , 'A')!=NULL) //���������� ������� �� ������������ ������
  
   //
   
  // s++;
   s = s+7; //������������� ����� ��������� ���������� �� ������ �������
   
  
       /*������� �������� �������*/
   for(i=0;i<11;i++) Timestamp[0][i] = 0x00; 
   for(i=0;i<6;i++) Speed[0][i] = 0x00;
   for(i=0;i<9;i++) Latitude[0][i] = 0x00;
   for(i=0;i<10;i++) Longitude[0][i] = 0x00;
   for(i=0;i<7;i++) Date[0][i] = 0x00; 
   
   for(i=0;i<10;i++) Timestamp[0][i] = *s++; //��������� ������ ������� 
  
   s++; //������������� ����� �� ������ ���������� ��������� 
   
   if(*s == 'A') //���� ���������� �������
   {
   STATUS.CoordinatesStatus = 'A'; //������������� ������ �������� ��������� 
   
   s=s+2;                                //������������� ����� ��������� ���������� �� ������ ������   
   for(i=0;i<8;i++) Latitude[0][i] = *s++; //��������� ������ ������
   
            /*��������� ������� ������*/
   for(i=0;i<4;i++) if(isdigit(Latitude[0][i]) != 1) STATUS.CoordinatesStatus = 'V';
   if(Latitude[0][4] != '.') STATUS.CoordinatesStatus = 'V';
   for(i=5;i<8;i++) if(isdigit(Latitude[0][i]) != 1) STATUS.CoordinatesStatus = 'V';
   
   s=s+3; //������������� ����� ��������� ���������� �� ������ �������
   for(i=0;i<9;i++) Longitude[0][i] = *s++; //��������� ������ ������� 
   
   /*��������� ������� �������*/
   for(i=0;i<5;i++) if(isdigit(Longitude[0][i]) != 1) STATUS.CoordinatesStatus = 'V';
   if(Longitude[0][5] != '.') STATUS.CoordinatesStatus = 'V';
   for(i=6;i<9;i++) if(isdigit(Longitude[0][i]) != 1) STATUS.CoordinatesStatus = 'V';
   
   
   
   s=s+3; //������������ ����� �� ������ ��������
   
   for(i=0;i<5;i++) //��������� ������ ��������
   {
    Speed[0][i] = *s++;
    if(*s == ',') break;
     
   }
   for(i=0;i<6;i++)
   {s++; //������������ ����� �� ������ ����
   if(*s == ',') break;}
   s++;
   for(i=0;i<6;i++) Date[0][i] = *s++; //��������� ������ ����
   for(i=0;i<6;i++) if(isdigit(Date[0][i]) != 1) STATUS.CoordinatesStatus = 'V';
  
   }
   else if(*s == 'V')  STATUS.CoordinatesStatus = 'V'; //���� ���������� �� ������� ������������� ��������������� ������ 
 
  
} 
}



/***************************����� �����****************************************/