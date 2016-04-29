# zad1


Protokół komunikacyjny: sterowanie kamerami

UWAGA!
Wartosci mozesz wpisywać zarowno w zapisie hex jak i dec!
Pamietaj jednak, ze jesli chcesz wpisac wartosc hex poprzedz ja sekwencja <0x> np. 0x01

Do dyspozycji mamy:
4 kamery o adresach : 1(0x01) 2(0x02) 3(0x03) 4(0x04)

Funkcje :
*Funkcje :\n*ON / OFF(kod funkcji : 10(0xA) przyjmujaca wartosc 0 lub 1
*MOVE_HORIZONTAL(kod funkcji : 20(0x14)) przyjmujaca argument z zakresu 0 - 200(0x00-0xC8)
*MOVE_VERTICAL(kod funkcji : 30(0x1E)) przyjmujaca argument z zakresu 0 - 125(0x00-0x7D)


Sposob uzycia:
1. Podaj informacje co chcesz zrobic. Np. zad1.exe 0x01 0xA 0x01 lub zad1.exe 1 10 1
2. W odpowiedzi otrzymasz ramke. Np. 0x01 0x10 0x01
3. Uruchom program w trybie do odczytu i podaj ramke.
	Np.zad1.exe -r 0x01 0x0A 0x01 0xF3 lub zad1.exe -r 1 10 1 243
4. Otrzymasz informacje o wyniku.
	Np. 0x01 0x0A 0x01 0xF3 0x00


Budowa ramki:[adres_kamery][kod_funkcji][argument_funkcji]
	Przyklad:zad1.exe 1 30 80
	Czyli:kamera nr 1,funkcja MOVE_VERTICAL,kat 80 stopni

Wynik:[adres_kamery][kod_funkcji][argument_funkcji][suma_kontrolna][kod_bledu]
	Przyklad:1 30 80 0x90 0x00
	Czyli:kamera nr 1,funkcja MOVE_VERTICAL,kat,suma_kontrolna,kod_bledu


KODY BLEDOW:
0x00 EVERYTHING_OK
0x01 WRONG_COMMAND
0x02 WRONG_CAMERA_ADDRESS
0x03 WRONG_FUNCTION_CODE
0x04 WRONG_ARGUMENT
0x05 OUT_OF_RANGE_POSITION
0x06 WRONG_CHECKSUM

UWAGA!
Wartosci możesz wpisywać zarówno w zapisie hex jak i dec!
Uzycie polecenia -h lub --help zawsze otworzy powyzsza pomoc!!!