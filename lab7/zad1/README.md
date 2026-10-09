# Prosty serwer HTTP i Klient

## Prosty serwer HTTP

Napisz serwer odpowiadający na zapytania. Serwer będzie zawierał zmienną LICZNIK_ZAPYTAŃ, której początkowa wartość to 0. Na początku program powinien przygotować się na przyjęcie połączenia TCP na wybranym porcie, np. 9000 i wszystkich dostępnych interfejsach sieciowych. W nieskończonej pętli program powinien:
- czekać na połączenie,
- przyjąć nadchodzące połączenie i odczytać przesłane dane z poleceniem,
- Jeżeli polecenie zaczyna się od "POST" lub "GET" należy zwiększyć LICZNIK_ZAPYTAŃ o jeden. Następnie należy odesłać tekst złożony z nagłówka i tekstu odpowiedzi. Nagłówek to:
  `HTTP/1.1 200 OK\r\nServer: Zajeciowy serwer SO\r\nContent-Type: text/plain; charset=utf-8\r\nConnection: close\r\nCache-Control: no-store\r\nContent-Length: %d\r\n\r\n`
  gdzie w miejsce "%d" należy wpisać liczbę bajtów z których składa się tekst odpowiedzi. Tekst odpowiedzi to: 
  `Liczba pobrań strony: %d`, gdzie w miejsce "%d" należy wpisać wartość zmiennej LICZNIK_ZAPYTAŃ.
- Jeżeli polecenie zaczyna się od "ZADANIE" to LICZNIK_ZAPYTAŃ zostaje zwiększony o wartość która wpisana jest w poleceniu za słowem ZADANIE. Następnie należy odesłać tekst odpowiedzi jak powyżej ale z pominięciem nagłówka HTTP.
- zamknąć połączenie

Z tak działającym serwerem można połączyć się dowolną przeglądarką internetową wpisując w polu adresu IP serwera oraz oddzielony dwukropkiem numer portu, np. 127.0.0.1:9000.

## Klient

Napisz drugi program nawiązujący połączenie TCP. Klient przyjmuje jako swoje argumenty: adres IPv4 serwera, numer portu oraz LICZBĘ. Program będzie łączył się z opisanym wyżej serwerem na wybranym porcie. Po nawiązaniu połączenia program powinien wysłać tekst: "ZADANIE LICZBA", gdzie LICZBA to wartość liczbowa wpisana jako trzeci argument programu. Następnie program powinien odebrać odpowiedź, wyświetlić ją na standardowym wyjściu i zakończyć pracę.