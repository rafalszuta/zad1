

/*  
 *  Protok� komunikacyjny: sterowanie kamerami
 *  Rafa� Szuta(C)
 */




#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

void Print(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e)
{
	printf("\n0x%02X 0x%02X %d 0x%02X 0x%02X\n", a, b, c, d, e);
}


int main(int argumentc, char * argv[])
{
	unsigned int error;    //errror code 
	unsigned int camAddress = strtol(argv[1], NULL, 16);
	unsigned int functionCode = strtol(argv[2], NULL, 16);
	unsigned int argument = atoi(argv[3]);

	unsigned int checkSum = 0xFF - camAddress - functionCode - argument;

	if (checkSum > 0xFF)   //if checkSum is out of range 
	{
		checkSum = 0xFF;
	}



	if (argumentc == 4)
	{
		if (camAddress == 0x01 | camAddress == 0x02 | camAddress == 0x03 | camAddress == 0x04)   //validating camera address
		{
			if (functionCode == 0x10 | functionCode == 0x20 | functionCode == 0x30)   //validating function code
			{ 
				if (functionCode == 0x10 && (argument == 0 | argument == 1))    //ON/OFF function
				{
					error = 0x00;
					Print(camAddress, functionCode, argument, error, checkSum);
				}

				if (functionCode == 0x10 && argument != 0 && argument != 1)
				{
					error = 0x04;
					Print(camAddress, functionCode, argument, error, checkSum);
				}


				if (functionCode == 0x20 && (atoi(argv[3]) < 200) && (atoi(argv[3]) > 0))   //MOVE_HORIZONTAL function 
				{
					error = 0x00;
					Print(camAddress, functionCode, argument, error, checkSum);
				}

				if (functionCode == 0x20 && (atoi(argv[3]) >= 200) | (atoi(argv[3]) <= 0)) 
				{
					error = 0x05;
					Print(camAddress, functionCode, argument, error, checkSum);
				}

				
				if (functionCode == 0x30 && (atoi(argv[3]) < 125) && (atoi(argv[3]) > 0))  //MOVE_VERTICAL function
				{
					error = 0x00;
					Print(camAddress, functionCode, argument, error, checkSum);
				}

				if (functionCode == 0x30 && (atoi(argv[3]) >= 125) | (atoi(argv[3]) < 0))  
				{
					error = 0x05;
					Print(camAddress, functionCode, argument, error, checkSum);
				}
			}


			else
			{
				error = 0x03;
				Print(camAddress, functionCode, argument, error, checkSum);
			}

		}
		else
		{
			error = 0x02;
			Print(camAddress, functionCode, argument, error, checkSum);
		}

	}
	else
	{
		error = 0x01;
		Print(camAddress, functionCode, argument, error, checkSum);
		printf("WRONG!\nYou should write :\nprogramName camAddress functionCode functionArgument\n");
		printf("Example : camera.exe 0x01 0x10 0\n\n");
	}

}