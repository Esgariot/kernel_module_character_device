# Oprogramowanie Systemowe - projekt
#### Robert Marciniak 149500

### Temat projektu - _9. Sterownik wyświetlający informacje o tablicach systemowych (GDT, IDT lub inne, plus tablica wektorów przerwań 0x80 plus system calls) w systemie Linux_

#### Co zrobiłem w ramach tematu
1. Moduł kernela, który tworzy urządzenie znakowe. Można z tego urządzenia pisać i czytać. Rzeczy pisane interpertuje jako PID procesu, który ma sprawdzić, a podczas odczytu zwraca zawartość rejestru CR3 tego procesu.
2. Program do komunikacji z urządzeniem
3. Moduł wypisujący zawartość GDT do logów komunikatów kernela (dmesg).


#### 1. Moduł kernela + urządzenie znakowe
1. Tworzy i rejestruje urządzenie "CRThree_char" gdy moduł jest ładowany
2. Rejestruje funkcje do obsługi odczytu i zapisu do buforów
3. Niszczy urządzenie i sprząta po sobie gdy moduł jest odłączany
4. Wypisuje do komunikatów kernela informacje związane z przetwarzaniem

#### 2. Program do komunikacji z urządzeniem
1. Przyjmuje jedyny argument linii poleceń - PID do sprawdzenia
2. Otwiera urządzenie
3. Pisze PID do bufora urządzenia
4. Odczytuje do swojego bufora przetworzony PID  na zawartość CR3
5. Wypisuje na wyjście zwróconą zawartość

#### 3. Moduł wypisujący GDT
1. Podczas ładowania wypisuje do komunikatów kernela zawartość 255 wpisów w GDT

#### 4. Makefile
- make - tworzy CRThreeModule.ko i GDTReaderModule.ko
- make install - ładuje moduł CRThreeModule.ko 
- make remove - usuwa moduł CRThreeModule
- make test - wyświetla dmesg po install i remove
- make test_gdt - wyświetla dmesg po załadowaniu i usunięciu GDTReaderModule
- make clean - czyści folder roboczy