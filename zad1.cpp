

/*
*  Protokół komunikacyjny: sterowanie kamerami
*  Rafal Szuta(C)
*  Update 1
*/




#include <string>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

using namespace std;


void Print(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e)   //informacja końcowa
{
	printf("\n0x%02X 0x%02X %d 0x%02X 0x%02X\n", a, b, c, d, e);
}

void Print2(unsigned int a, unsigned int b, unsigned int c, unsigned int d)    //wydruk ramki
{
	printf("\n0x%02X 0x%02X %d 0x%02X\n", a, b, c, d);
}


void Instruction()
{
	printf("\n------------------------------Instrukcja!------------------------------\n\n");
	printf("Do dyspozycji mamy:\n\n");
	printf("4 kamery o adresach : 0x01 0x02 0x03 0x04\n\n");
	printf("Funkcje :\n*ON / OFF(kod funkcji : 0x10) przyjmujaca wartosc 0 lub 1\n");
	printf("*MOVE_HORIZONTAL(kod funkcji : 0x20) przyjmujaca argument z zakresu 0 - 200\n");
	printf("*MOVE_VERTICAL(kod funkcji : 0x30) przyjmujaca argument z zakresu 0 - 125\n\n");
	printf("Sposob uzycia:\n");
	printf("1. Podaj informacje co chcesz zrobic. Np. zad1.exe 0x01 0x10 1\n");
	printf("2. W odpowiedzi otrzymasz ramke. Np. 0x01 0x10 1\n");
	printf("3. Uruchom program w trybie do odczytu i podaj ramke.\n");
	printf("\tNp.zad1.exe - r 0x01 0x10 1 0xED\n");
	printf("4. Otrzymasz informacje o wyniku.\n\tNp. 0x01 0x10 1 0xED 0x00\n\n");

	printf("Budowa ramki:[adres_kamery][kod_funkcji][argument_funkcji]\n");
	printf("Przyklad:zad1.exe 0x01 0x30 25\n\tCzyli:kamera nr 1,funkcja MOVE_VERTICAL,kat\n\n");
	printf("Wynik:[adres_kamery][kod_funkcji][argument_funkcji]");
	printf("[suma_kontrolna][kod_bledu]\n");
	printf("Przyklad:0x01 0x30 25 0xB5 0x00\n\t");
	printf("Czyli:kamera nr 1,funkcja MOVE_VERTICAL,kat,");
	printf("suma_kontrolna,kod_bledu\n\n");


	printf("KODY BLEDOW:\n");
	printf("0x00 EVERYTHING_OK\t\t0x01 WRONG_COMMAND\n");
	printf("0x02 WRONG_CAMERA_ADDRESS\t0x03 WRONG_FUNCTION_CODE\n");
	printf("0x04 WRONG_ARGUMENT\t\t0x05 OUT_OF_RANGE_POSITION\n");
	printf("0x06 WRONG_CHECKSUM\n\n");


	printf("UWAGA!\n");
	printf("Wartosci możesz wpisywać zarówno w zapisie hex jak i dec!\n\n");
	printf("Uzycie polecenia -h lub --help zawsze otworzy powyzsza pomoc!!!\n\n\n");

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


	if ((argc < 4 || argc>4) && argc != 6)
	{
		Instruction();

	}
	else
	{

		if (argc == 4)
		{
			unsigned int error = 0;    //errror code 
			unsigned int camAddress = strtol(argv[1], NULL, 16);
			unsigned int functionCode = strtol(argv[2], NULL, 16);
			unsigned int argument = atoi(argv[3]);

			unsigned int checkSum = 0xFF - camAddress - functionCode - argument;

			if (checkSum > 0xFF)   //if checkSum is out of range 
			{
				checkSum = 0xFF;
			}


			Print2(strtol(argv[1], NULL, 16), strtol(argv[2], NULL, 16), atoi(argv[3]), checkSum);


		}

		string text = "";     //read only -r
		text.append(string(argv[1]));

		if (argc == 6 && text == "-r")
		{

			unsigned int error;    //errror code 
			unsigned int camAddress = strtol(argv[2], NULL, 16);
			unsigned int functionCode = strtol(argv[3], NULL, 16);
			unsigned int argument = atoi(argv[4]);
			unsigned int userSum = strtol(argv[5], NULL, 16); //checkSum writed by user

			unsigned int checkSum = 0xFF - camAddress - functionCode - argument;

			if (checkSum > 0xFF)   //if checkSum is out of range 
			{
				checkSum = 0xFF;
			}



			if (checkSum == userSum)
			{

				if (camAddress == 0x01 || camAddress == 0x02 || camAddress == 0x03 || camAddress == 0x04)   //validating camera address
				{
					if (functionCode == 0x10 || functionCode == 0x20 || functionCode == 0x30)   //validating function code
					{
						if (functionCode == 0x10 && (argument == 0 || argument == 1))   //ON/OFF function
						{
							error = 0x00;
							Print(camAddress, functionCode, argument, checkSum, error);
						}

						if (functionCode == 0x10 && argument != 0 && argument != 1)
						{
							error = 0x04;
							Print(camAddress, functionCode, argument, checkSum, error);
						}


						if (functionCode == 0x20 && (argument < 200) && (argument > 0))   //MOVE_HORIZONTAL function 
						{
							error = 0x00;
							Print(camAddress, functionCode, argument, checkSum, error);
						}

						if (functionCode == 0x20 && (argument >= 200) || (argument <= 0))
						{
							error = 0x05;
							Print(camAddress, functionCode, argument, checkSum, error);
						}


						if (functionCode == 0x30 && (argument < 125) && (argument > 0))  //MOVE_VERTICAL function
						{
							error = 0x00;
							Print(camAddress, functionCode, argument, checkSum, error);
						}

						if (functionCode == 0x30 && (argument >= 125 || argument < 0))
						{
							error = 0x05;
							Print(camAddress, functionCode, argument, checkSum, error);
						}
					}


					else
					{
						error = 0x03;
						Print(camAddress, functionCode, argument, checkSum, error);
					}

				}
				else
				{
					error = 0x02;
					Print(camAddress, functionCode, argument, checkSum, error);
				}

			}
			else
			{
				error = 0x06;
				Print(camAddress, functionCode, argument, checkSum, error);

			}

		}




	}

}
