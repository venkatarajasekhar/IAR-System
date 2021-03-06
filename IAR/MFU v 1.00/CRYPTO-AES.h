//******************************************************************************
//            ������������ ���� ���������� ������� ������ ����������
//******************************************************************************

//*****************������������ �����*******************************************

#include "stm32f10x.h"
//***************����������������***********************************************
#define Nb 4 //����� 32-������ ���� ������������ �� ������� �����
#define Nk 4 //����� 32-������ ���� ������������ � �������� �����
#define Nr 10 //����� �������
#define N Nb*(Nr+1) //����������� ���� ������������ �����

#define ENCRYPT 0 //�������� 
#define DECRYPT 1 //����������

#define BPOLY (uint8_t) 0x11B
#define MPOLY (uint8_t) 0x101

#define ROTL(x) (((x)>>7) | ((x)<<1))
#define ROTL8(x) (((x)<<8) | ((x)>>24))
#define ROTL16(x) (((x)<<16) | ((x)>>16))
#define ROTL24(x) (((x)<<24) | ((x)>>8))
//**************����������� ����������� �����***********************************

//************* �������������  ���������� ��������� �� ������ ������ ***********

//*************���������� ����������� �������***********************************
void Encrypt( uint8_t *keyblok, uint8_t *buff, uint8_t *result); //������� ��������������
void Decrypt( uint8_t *keyblok, uint8_t *buff, uint8_t *result); //������� ���������������

void AddRoundKey(uint32_t *s, uint32_t *key , uint8_t nround ); //�������� ����������� ��� � ��������� ������
void SubBytes(uint8_t *s, uint8_t direct); //������� ������ ����� � �������
void ShiftRows(uint8_t *s, uint8_t direct); //������� ������� �����
void MixColums(uint32_t *s, uint8_t lenght, uint8_t direct); //������ ������������� ������ � �������

void KeyExpansion(uint8_t *key , uint8_t direct); //������� ���������� �����

uint8_t product(uint32_t x, uint32_t y, uint8_t mod); //��������������� ������� �������������� ������ � �������
uint8_t bmul(uint8_t a, uint8_t b, uint8_t mod); //��������� ��������� � ���� GF(2^m)
void shiftrow(uint8_t *row , uint8_t n , uint8_t direct); //������� ������ ������
uint32_t pack(uint8_t *b); //�������������� 8-������ ��������� ������� � 32-������ �����
void unpack(uint32_t a , uint8_t *b ); //�������������� 32-������� ����� � 8-������ ��������
uint32_t SubDWord(uint32_t a); //������� ������ ������� �������
uint8_t ByteSub(uint8_t x); //������� ������ �����
uint8_t InvByteSub(uint8_t x); //������� �������� ������ �����
uint8_t xtime(uint8_t a, uint8_t mod); //������� �������� �����

/***************************����� �����****************************************/
