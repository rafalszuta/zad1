# zad1


Protokół komunikacyjny: sterowanie kamerami

Do dyspozycji mamy:
- 4 kamery o adresach: 0x01 0x02 0x03 0x04
- 3 funkcje:
  *ON/OFF (kod funkcji: 0x10) przyjmująca wartość 0 lub 1
  *MOVE_HORIZONTAL (kod funkcji: 0x20) przyjmująca argument z zakresu 0-200
  *MOVE_VERTICAL (kod funkcji: 0x30) przyjmująca argument z zakresu 0-125

Suma kontrolna obliczana jest wg zależności: checkSum = 0xFF - camAddress - functionCode - argument
(jeżeli checkSum wychodzi poza zakres to wówczas checkSum = 0xFF).

Jeśli użytkownik popełni błąd wpisująć komendy to wychwytywane są następujące błędy:
- 0x00	EVERYTHING_OK
- 0x01	WRONG_COMMAND
- 0x02	WRONG_CAMERA_ADDRESS
- 0x03	WRONG_FUNCTION_CODE
- 0x04	WRONG_ARGUMENT
- 0x05    OUT_OF_RANGE_POSITION

Opis ramki:

Wysyłana ramka: adres_kamery kod_funkcji argument_funkcji
Przykład: zad1.exe 0x01 0x30 25 //odwołanie do kamery nr 1, funkcja MOVE_VERTICAL , kąt_obrotu


Odbierana ramka: adres_kamery kod_funkcji argument_funkcji, kod_błędu, suma_kontrolna
Przykład: zad1.exe 0x01 0x30 25 0x00 0xB5 //odwołanie do kamery nr 1, funkcja MOVE_VERTICAL , kąt_obrotu, kod_błędu, suma_kontrolna
