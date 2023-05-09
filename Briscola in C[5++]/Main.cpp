#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	Segno briscola;
	Carta cartaChiamata{ 0,0 };
	char scelta;

	do {
		system("cls");
		int punteggioMinimo = 74;
		int primoAGiocare = Random(0, 4);
		punteggioMinimo = InizioGioco(giocatori, punteggioMinimo, primoAGiocare);
		briscola = GiroMorto(giocatori, punteggioMinimo, primoAGiocare, cartaChiamata);
		for (int i = 0; i < 7; i++)
			GiroStardard(giocatori, primoAGiocare, briscola, punteggioMinimo, cartaChiamata);
		StampaVincitori(giocatori, punteggioMinimo);
		cout << endl << "Giocare ancora? (n per uscire) " << flush;
		cin >> scelta;
		for (int i = 0; i < 5; i++)
			giocatori[i]->CancellaCarte();
		delete[] giocatori;
	} while (scelta != 'n' && scelta != 'N');

	
	return 0;
}