# zad1


Protokół komunikacyjny: sterowanie kamerami


Do dyspozycji mamy:
4 kamery o adresach : 0x01 0x02 0x03 0x04

Funkcje :
*ON / OFF(kod funkcji : 0x10) przyjmujaca wartosc 0 lub 1
*MOVE_HORIZONTAL(kod funkcji : 0x20) przyjmujaca argument z zakresu 0 - 200
*MOVE_VERTICAL(kod funkcji : 0x30) przyjmujaca argument z zakresu 0 - 125


Sposob uzycia:
1. Podaj informacje co chcesz zrobic. Np. zad1.exe 0x01 0x10 1
2. W odpowiedzi otrzymasz ramke. Np. 0x01 0x10 1
3. Uruchom program w trybie do odczytu i podaj ramke.
	Np.zad1.exe - r 0x01 0x10 1 0xED
4. Otrzymasz informacje o wyniku.
	Np. 0x01 0x10 1 0xED 0x00


Budowa ramki:[adres_kamery][kod_funkcji][argument_funkcji]
	Przyklad:zad1.exe 0x01 0x30 25 
	Czyli:kamera nr 1,funkcja MOVE_VERTICAL,kat

Wynik:[adres_kamery][kod_funkcji][argument_funkcji][suma_kontrolna][kod_bledu]
	Przyklad:0x01 0x30 25 0xB5 0x00
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