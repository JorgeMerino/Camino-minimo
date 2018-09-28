#pragma once

#ifndef heuristica_h
#define heuristica_h

#include <math.h>
#include "nodo.h"

//La heuristica es la distancia estimada que queda para llegar hasta el final
//El m�todo que se ha utilizado para su c�lculo es la Distancia Eucl�dea
class heuristica {
private:
	double _heur;
public:
	heuristica() {};
	//Calcula la Distancia Eucl�dea entre un nodo dado y un nodo meta
	heuristica(nodo act, nodo fin) {
		int catX = abs(act.getX() - fin.getX());
		int catY = abs(act.getY() - fin.getY());
		_heur = sqrt(pow(catX, 2) + pow(catY,2));
	}
	double getHeur() {
		return _heur;
	}
};

#endif // heuristica