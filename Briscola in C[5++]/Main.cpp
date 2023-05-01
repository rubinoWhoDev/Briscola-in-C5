#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	Carte mazzo(40);

	int punteggioMinimo = 74;
	int primoAGiocare = Random(0, 4);

	inizializzaGiocatori(giocatori);

	for (int i = 0; i < 5; i++) {
		giocatori[i]->Pesca(mazzo, 8);
		giocatori[i]->OrdinaCarte();
		giocatori[i]->stampaNome();
		giocatori[i]->stampaMano();
		cout << endl;
	}


	delete[] giocatori;
	return 0;
}