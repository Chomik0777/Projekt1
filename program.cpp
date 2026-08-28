#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

int losuj(int minV, int maxV)
{
    return minV + rand() % (maxV - minV + 1);
}

long long silnia(int n)
{
    long long w = 1;
    for (int i = 2; i <= n; i++) w *= i;
    return w;
}

long long potega(int a, int b)
{
    long long w = 1;
    for (int i = 0; i < b; i++) w *= a;
    return w;
}

string lowerAscii(string s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = char(s[i] - 'A' + 'a');
    }
    return s;
}

string wybierzTryb(const string& wpis)
{
    string t = lowerAscii(wpis);
    if (t == "latwy") return "Latwy";
    if (t == "sredni" || t == "Sbredni") return "Sredni";
    if (t == "trudny") return "Trudny";
    return "Latwy"; // domyślnie
}

int main()
{
    srand((unsigned)time(NULL));

    string imie;
    cout << "Imie?\n Odpowiedz: ";
    getline(cin, imie);
    if (imie.empty()) imie = "Gracz";

    string wpisTrudnosc;
    cout << "Okej, " << imie << "! Jaka trudnosc chcesz? (Latwy/Sredni/Trudny)\n Odpowiedz: ";
    getline(cin, wpisTrudnosc);

    string tryb = wybierzTryb(wpisTrudnosc);

    cout << "\n" << imie << ", runda rozpocznie sie za 10 sekund. Przygotuj sie!\n\n";
    for (int i = 10; i >= 1; i--)
    {
        cout << "Start za: " << i << "...\n";
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "\nRunda sie rozpoczyna!\n";

    // Zakres liczb zależnie od trudności
    int minL = 1, maxL = 20;
    if (tryb == "Sredni") { minL = 1; maxL = 50; }
    if (tryb == "Trudny") { minL = 1; maxL = 12; }

    // Jakie działania są dostępne
    // 0:+ 1:- 2:x 3:: 4:^ 5:!
    int opMax = 1;
    if (tryb == "Sredni") opMax = 3;
    if (tryb == "Trudny") opMax = 5;

    int punkty = 0;
    const int CZAS_RUNDY = 30;

    auto start = chrono::steady_clock::now();

    while (true)
    {
        auto teraz = chrono::steady_clock::now();
        int sekundy = (int)chrono::duration_cast<chrono::seconds>(teraz - start).count();
        if (sekundy >= CZAS_RUNDY) break;

        int op = losuj(0, opMax);

        int a = losuj(minL, maxL);
        int b = losuj(minL, maxL);

        string znak;
        long long poprawna = 0;

        if (op == 0) // +
        {
            znak = "+";
            poprawna = (long long)a + (long long)b;
        }
        else if (op == 1) // -
        {
            znak = "-";
            poprawna = (long long)a - (long long)b;
        }
        else if (op == 2) // x
        {
            znak = "x";
            poprawna = (long long)a * (long long)b;
        }
        else if (op == 3) // : (dzielenie z całkowitym wynikiem)
        {
            znak = ":";
            b = losuj(1, 12);
            int k = losuj(1, 10);
            a = b * k;
            poprawna = a / b;
        }
        else if (op == 4) // ^
        {
            znak = "^";
            a = losuj(2, 6);
            b = losuj(2, 4);
            poprawna = potega(a, b);
        }
        else // 5: !
        {
            znak = "!";
            a = losuj(3, 8);
            poprawna = silnia(a);
        }

        cout << "\nIle to jest ";
        if (op == 5)
            cout << a << znak;
        else
            cout << a << " " << znak << " " << b;
        cout << "?\n Odpowiedz: ";

        long long odp;
        cin >> odp;

        if (odp == poprawna)
        {
            punkty++;
            cout << "Dobrze! +1 punkt\n";
        }
        else
        {
            cout << "Nieprawidlowa odpowiedz. Jest to " << poprawna << ", a nie " << odp << ".\n";
        }

        // wyczyść bufor wejścia (żeby kolejne pytanie działało poprawnie)
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\nKoniec rundy!\n";
    cout << "Trudnosc: " << tryb << "\n";
    cout << "Punkty: " << punkty << "\n\n";

    
    
    // Zapis wyniku do pliku (osobno dla trudności)
    string nazwaWynik = "Wyniki_" + tryb + ".txt";
    ofstream wynik(nazwaWynik.c_str(), ios::app);
    if (wynik.is_open())
    {
        wynik << imie << ";" << tryb << ";" << punkty << ";" << "27/08/2026" << "\n";
        wynik.close();
        cout << "Zapisano wynik do: " << nazwaWynik << "\n";
    }
    else
    {
        cout << "Nie udalo sie zapisac wyniku do pliku.\n";
    }

    // Zapis ustawień (prosto)
    ofstream ustaw("UstawieniaGracza.txt");
    if (ustaw.is_open())
    {
        ustaw << "Trudnosc=" << tryb << "\n";
        ustaw << "Level=1\n";
        ustaw.close();
    }

    cout << "\nNacisnij Enter, aby zakonczyc...";
    cin.get();

}