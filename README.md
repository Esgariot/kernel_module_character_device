# Oprogramowanie Systemowe - projekt
#### Robert Marciniak

### Temat projektu - _9. Sterownik wyświetlający informacje o tablicach systemowych (GDT, IDT lub inne, plus tablica wektorów przerwań 0x80 plus system calls) w systemie Linux_

#### Co zrobiłem w tym projekcie
1. Moduł kernela, który tworzy urządzenie znakowe. Można z tego urządzenia pisać i czytać. Rzeczy pisane interpertuje jako PID procesu, który ma sprawdzić, a podczas odczytu zwraca zawartość rejestru CR3 tego procesu.
2. Program do komunikacji z urządzeniem
3. Moduł wypisujący zawartość GDT do logów komunikatów kernela.

