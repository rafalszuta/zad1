/*
*  Protokól komunikacyjny: sterowanie kamerami
*  Rafal Szuta(C)
*  Update 1.2
*/


#include <string>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


void Print(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e)   //informacja końcowa
{
	printf("\n0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", a, b, c, d, e);
}

void Print2(unsigned int a, unsigned int b, unsigned int c, unsigned int d)    //wydruk ramki
{
	printf("\n0x%02X 0x%02X 0x%02X 0x%02X\n", a, b, c, d);
}

void Parse(const char *a, unsigned int *b)
{
	char * pEnd;
	*b = strtol(a, &pEnd, 10);
	if (*b == 0)
	{
		//printf("byla liczba hex\n");
		*b = strtol(a, &pEnd, 16);
		//printf("Liczba: %X\n",*b);
	}

	else
	{
		//("była liczba 10\n");
		//	printf("Liczba: %X\n", *b);
	}



}

void Check(unsigned int a, unsigned int b, unsigned int c, unsigned int *d)   //validation

{
	/*
	camAddress=a;
	functionCode =b;
	argument =c
	error=d;*/


	if (a == 0x01 || a == 0x02 || a == 0x03 || a == 0x04)   //validating camera address
	{
		if (b == 0xA || b == 0x14 || b == 0x1E)   //validating function code
		{
			if (b == 0xA && (c == 0 || c == 0x01))   //ON/OFF function
			{
				*d = 0x00;
			}

			if (b == 0xA && c != 0x00 && c != 0x01)
			{
				*d = 0x04;
			}

			if (b == 0x14 && (c < 0xC8) && (c >  0x00))   //MOVE_HORIZONTAL function
			{
				*d = 0x00;
			}

			if (b == 0x14 && (c > 0xC8 || c == 0))
			{
				*d = 0x05;
			}

			if (b == 0x1E && (c < 0x7D) && (c>  0x00))  //MOVE_VERTICAL function
			{
				*d = 0x00;
			}

			if (b == 0x1E && (c > 0x7D || c == 0x00))
			{
				*d = 0x05;
			}
		}

		else
		{
			*d = 0x03;
		}
	}
	else
	{
		*d = 0x02;
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


	if ((argc < 4 || argc>4) && argc != 6)
	{
		Instruction();
	}
	else
	{
		if (argc == 4)
		{
			unsigned int error = 0x00;    //errror code
			unsigned int camAddress = 0;
			unsigned int functionCode = 0;
			unsigned int argument = 0;

			Parse(argv[1], &camAddress);
			Parse(argv[2], &functionCode);
			Parse(argv[3], &argument);

	

			unsigned int checkSum = 0xFF - camAddress - functionCode - argument;

			if (checkSum > 0xFF)   //if checkSum is out of range
			{
				checkSum = 0xFF;
			}

			Check(camAddress, functionCode, argument, &error);


			if (error == 0x00)
			{

				Print2(camAddress, functionCode, argument, checkSum);
			}
			else
			{
				printf("Podales bledna komende!\n");
				system("Pause");
				Instruction();
			}
		}

		string text = "";     //read only -r
		text.append(string(argv[1]));

		if (argc == 6 && text == "-r")
		{
			unsigned int error = 0x00;    //errror code
			unsigned int camAddress = 0;
			unsigned int functionCode = 0;
			unsigned int argument = 9;
			unsigned int userSum = 0;//checkSum writed by user
			

			Parse(argv[2], &camAddress);
			Parse(argv[3], &functionCode);
			Parse(argv[4], &argument);
			Parse(argv[5], &userSum);

			unsigned int checkSum = 0xFF - camAddress - functionCode - argument;
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
				error = 0x06;
				Print(camAddress, functionCode, argument, checkSum, error);
			}
		}
	}
}
