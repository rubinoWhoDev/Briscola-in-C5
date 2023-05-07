#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	Segno briscola;
	Carta cartaChiamata{ 0,0 };
	int punteggioMinimo = 74;
	int primoAGiocare = Random(0, 4);

	punteggioMinimo = InizioGioco(giocatori, punteggioMinimo, primoAGiocare);
	briscola = GiroMorto(giocatori, punteggioMinimo, primoAGiocare, cartaChiamata);
	for (int i = 0; i < 7; i++)
		GiroStardard(giocatori, primoAGiocare, briscola, punteggioMinimo, cartaChiamata);

	delete[] giocatori;
	
	return 0;
}