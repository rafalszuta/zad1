
/*
*  Protokól komunikacyjny: sterowanie kamerami
*  Rafal Szuta(C)
*/


#include <string>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


 int camAddress = 0;
 int functionCode = 0;
 int argument = 0;
 int error = 0x00;
 int userSum = 0;
 int checkSum = 0xFF - camAddress - functionCode - argument;

#define ON_OFF 0 | 0x00
#define DOWN_RANGE 0x00
#define HOR_RANGE 0xC8
#define VER_RANGE 0x7D
#define FIRST_ARG_NUMBER 4
#define SECOND_ARG_NUMBER 6

#define SUCCESS 0x00
#define COMMAND_ERR 0x01
#define CAMERA_ERR 0x02
#define FUNCTION_ERR 0x03
#define ARGUMENT_ERR 0x04
#define OUT_OF_RANGE_ERR 0x05
#define CHECKSUM_ERR 0x06

#define CAMERA1 0x01
#define CAMERA2 0x02
#define CAMERA3 0x03
#define CAMERA4 0x04

#define FUN_ON_OFF 0xA
#define FUN_MOVE_HOR 0x14
#define FUN_MOVE_VER 0x1E


void Print(int camAddress, int functionCode, int argument, int checkSum, int error)   //informacja końcowa
{
	printf("\n0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", camAddress, functionCode, argument, checkSum, error);
}

void Print2(int camAddress, int functionCode, int argument, int checkSum)    //wydruk ramki
{
	printf("\n0x%02X 0x%02X 0x%02X 0x%02X\n", camAddress, functionCode, argument, checkSum);
}

void Parse(const char *camAddress, int * functionCode)
{
	*functionCode = strtol(camAddress, NULL, 10);
	if (*functionCode == 0)
	{
		*functionCode = strtol(camAddress, NULL, 16);
	}
}

void Check(int camAddress, int functionCode, int argument, int *error)   
{
	if (camAddress == CAMERA1 || camAddress == CAMERA2 || camAddress == CAMERA3 || camAddress == CAMERA4)  
	{
		if (functionCode == FUN_ON_OFF || functionCode == FUN_MOVE_HOR || functionCode == FUN_MOVE_VER)
		{
			if (functionCode == FUN_ON_OFF && argument == ON_OFF)   
			{
				*error = SUCCESS;
			}

			if (functionCode == FUN_ON_OFF && argument != ON_OFF)
			{
				*error = ARGUMENT_ERR;
			}

			if (functionCode == FUN_MOVE_HOR && (argument < HOR_RANGE) && (argument >  DOWN_RANGE))   
			{
				*error = SUCCESS;
			}

			if (functionCode == FUN_MOVE_HOR && (argument > HOR_RANGE || argument == DOWN_RANGE))
			{
				*error = OUT_OF_RANGE_ERR;
			}

			if (functionCode == FUN_MOVE_VER && (argument < VER_RANGE) && (argument>  DOWN_RANGE)) 
			{
				*error = SUCCESS;
			}

			if (functionCode == FUN_MOVE_VER && (argument > VER_RANGE || argument == DOWN_RANGE))
			{
				*error = OUT_OF_RANGE_ERR;
			}
		}
		else
		{
			*error = FUNCTION_ERR;
		}
	}
	else
	{
		*error = CAMERA_ERR;
	}
}




void Instruction()
{
	printf("\n------------------------------Instrukcja!------------------------------\n\n");
	printf("UWAGA!\n");
	printf("Wartosci mozesz wpisywać zarowno w zapisie hex jak i dec!\n\n");
	printf("Pamietaj jednak, ze jesli chcesz wpisac wartosc hex poprzedz ja sekwencja <0x> np. 0x01\n\n");
	printf("Do dyspozycji mamy:\n\n");
	printf("4 kamery o adresach : 1(0x01) 2(0x02) 3(0x03) 4(0x04)\n\n");
	printf("Funkcje :\n*ON / OFF(kod funkcji : 10(0xA) przyjmujaca wartosc 0 lub 1\n");
	printf("*MOVE_HORIZONTAL(kod funkcji : 20(0x14)) przyjmujaca argument z zakresu 0 - 200(0x00-0xC8*)\n");
	printf("*MOVE_VERTICAL(kod funkcji : 30(0x1E)) przyjmujaca argument z zakresu 0 - 125(0x00-0x7D)\n\n");
	printf("Sposob uzycia:\n");
	printf("1. Podaj informacje co chcesz zrobic. Np. zad1.exe 0x01 0xA 0x01 lub zad1.exe 1 10 1\n");
	printf("2. W odpowiedzi otrzymasz ramke. Np. 0x01 0x0A 0x01 0xF3\n");
	printf("3. Uruchom program w trybie do odczytu i podaj ramke.\n");
	printf("\tNp.zad1.exe -r 0x01 0x0A 0x01 0xF3 lub zad1.exe -r 1 10 1 243 \n");
	printf("4. Otrzymasz informacje o wyniku.\n\tNp. 0x01 0x0A 0x01 0xF3 0x00\n\n");
	printf("Budowa ramki:[adres_kamery][kod_funkcji][argument_funkcji]\n");
	printf("Przyklad:zad1.exe 1 30 80\n\tCzyli:kamera nr 1,funkcja MOVE_VERTICAL,kat 80 stopni\n\n");
	printf("Wynik:[adres_kamery][kod_funkcji][argument_funkcji]");
	printf("[suma_kontrolna][kod_bledu]\n");
	printf("Przyklad:1 30 80 0x90 0x00\n\t");
	printf("Czyli:kamera nr 1,funkcja MOVE_VERTICAL,kat,");
	printf("suma_kontrolna,kod_bledu\n\n");
	printf("KODY BLEDOW:\n");
	printf("0x00 EVERYTHING_OK\t\t0x01 WRONG_COMMAND\n");
	printf("0x02 WRONG_CAMERA_ADDRESS\t0x03 WRONG_FUNCTION_CODE\n");
	printf("0x04 WRONG_ARGUMENT\t\t0x05 OUT_OF_RANGE_POSITION\n");
	printf("0x06 WRONG_CHECKSUM\n\n");
	printf("UWAGA!\n");
	printf("Uzycie polecenia -h lub --help zawsze otworzy pomoc!!!\n\n\n");
}


int main(int argc, char * argv[])
{
	for (int i = 0; i < argc; i++)     //open help
	{
		string textHelp = "";
		textHelp.append(string(argv[i]));

		if (textHelp == "--help" || textHelp == "-h")
		{
			Instruction();
		}
	}

	if ((argc < FIRST_ARG_NUMBER || argc>FIRST_ARG_NUMBER) && argc != SECOND_ARG_NUMBER)
	{
		Instruction();
	}
	else
	{
		if (argc == FIRST_ARG_NUMBER)
		{
			Parse(argv[1], &camAddress);
			Parse(argv[2], &functionCode);
			Parse(argv[3], &argument);

			if (checkSum > 0xFF)   //if checkSum is out of range
			{
				checkSum = 0xFF;
			}

			Check(camAddress, functionCode, argument, &error);

			if (error == SUCCESS)
			{
				Print2(camAddress, functionCode, argument, checkSum);
			}
			else
			{
				printf("Podales bledna komende!\n");
				Instruction();
			}
		}

		string text = "";     //read only -r
		text.append(string(argv[1]));

		if (argc == SECOND_ARG_NUMBER && text == "-r")
		{
			Parse(argv[2], &camAddress);
			Parse(argv[3], &functionCode);
			Parse(argv[4], &argument);
			Parse(argv[5], &userSum);

			if (checkSum > 0xFF)   //if checkSum is out of range
			{
				checkSum = 0xFF;
			}

			if (checkSum == userSum)
			{
				Check(camAddress, functionCode, argument, &error);
				Print(camAddress, functionCode, argument, checkSum, error);
			}
			else
			{
				error = CHECKSUM_ERR;
				Print(camAddress, functionCode, argument, checkSum, error);
			}
		}
	}
}
