# Serwer gry sieciowej

# Wymagania

  - CMake v3.5+ (sudo apt-get install cmake)
  - kompilator c++ (niezbędny standard c++14)
  - boost (sudo apt-get install libboost-all-dev)
 
# Build	
	$ cd build
	$ cmake ..
	$ make
	$ ./GameServer_run <port>
	$ ./GameServer_test #unit tests

# Struktura plików:
 - Bullet.h – klasa opisująca pocisk w grze
 - Client.h – klasa opisująca klienta w komunikacji sieciowej
 - common.h – plik zawierający parametry gry i ustawienia komunikacji sieciowej
 - Game.h – klasa obsługująca logikę gry
 - Player.h – klasa opisująca gracza w grze
 - Server.h – klasa obsługująca serwer i łączność z klientami
 - Encryptor.h - klasa obsługująca szyfrowanie i liczenie CRC

# Sposób działania:

Serwer po uruchomieniu otwiera port nasłuchujący klientów. Po nawiązaniu połączenia TCP przez klienta, serwer tworzy oddzielny wątek obsługujący to połączenie (Server::handleConnection()). Jeżeli serwer nie otrzyma pakietu aktywności po TCP w ciągu 10 sekund rozłącza klienta. Serwer, po uruchomieniu, nasłuchuje po UDP. Klient po inicjalizacji, może przesyłać swoje ruchy (obsłużone w 61 linijce w Server.cpp). Oddzielny wątek serwera (Server::sendGameState) wysyła okresowo stany gry każdemu klientowi. Gra aktualizuje się na podstawie mierzonego czasu przed każdym wysłaniem i zmianą stanu gry.
