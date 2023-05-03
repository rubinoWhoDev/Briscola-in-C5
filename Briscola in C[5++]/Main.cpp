#include <iostream>
#include "Carte.h"
#include "Briscola.h"

using namespace std;

int main() {
	Giocatore** giocatori = nullptr;
	int punteggioMinimo = 74;
	int primoAGiocare = Random(0, 4);

	inizializzaGiocatori(giocatori);

	while (punteggioMinimo == 74) {
		Carte mazzo(40);

		for (int i = 0; i < 5; i++) {
			giocatori[i]->Pesca(mazzo, 8);
			giocatori[i]->OrdinaCarte();
			giocatori[i]->setChiamante(true);
			/*
			giocatori[i]->stampaNome();
			giocatori[i]->stampaMano();
			*/
			cout << endl;
		}

		if (TuttiLisci(giocatori)) {
			cout << "Un giocatore ha in mano tutti lisci." << endl;
			StampaManoGiocatori(giocatori);
			continue;
		}
		punteggioMinimo = GiroChiamanti(giocatori, punteggioMinimo);
		for (int i = 0; i < 5; i++)
			giocatori[i]->getMano().~Carte();
	}
	

	delete[] giocatori;
	
	return 0;
}