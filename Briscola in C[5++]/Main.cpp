#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	//Segno briscola;
	int punteggioMinimo = 74;
	int primoAGiocare = Random(0, 4);

	punteggioMinimo = InizioGioco(giocatori, punteggioMinimo, primoAGiocare);
	/*briscola =*/ GiroMorto(giocatori, punteggioMinimo, primoAGiocare);

	delete[] giocatori;
	
	return 0;
}