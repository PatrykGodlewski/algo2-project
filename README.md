Sposób kompilacji i uruchomienia:
---------------------------------

### Wymagania
- g++ (C++17)
- make

### Uruchomienie
1. Kompilacja
```make```

2. Uruchomienie:
```make run```
lub
```./main <test.txt | test2.txt>```

3. Czyszczenie
```make clean```

Struktura plików:
-----------------
- main.cpp              – główny plik źródłowy z funkcją `main`
- Makefile              - plik do łatwego uruchamiania aplikacji
- test.txt              – plik z przykładowymi danymi wejściowymi
- test2.txt             – plik z przykładowymi danymi wejściowymi
- README.txt            – ten plik

Przykładowe dane wejściowe/wyjściowe:
-------------------------------------
Wejście:
```
-10[-5[100;20[-15;]];30[;8]]
```
```
Wizualizacja struktury drzewa wejsciowego

        ┌── 8
    ┌── 30
-10
    │   ┌── 20
    │   │   └── -15
    └── -5
        └── 100
```

Wyjście:
```
        ┌── 100
    ┌── 30
20
    │   ┌── 8
    │   │   └── -5
    └── -10
        └── -15
```

Uwagi dodatkowe:
----------------
- Plik testowy musi być zapisany według konkretnej zasady ```root[leaf;leaf]```, aby program działał poprawnie, np.: ```-10[-5[100;20[-15;]];30[;8]]```.
- Program bierze pod uwagę tylko pierwszą linię pliku testowego.
