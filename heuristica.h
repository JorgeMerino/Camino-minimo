#pragma once

#ifndef heuristica_h
#define heuristica_h

#include <math.h>
#include "nodo.h"

//La heuristica es la distancia estimada que queda para llegar hasta el final
//El método que se ha utilizado para su cálculo es la Distancia Euclídea
class heuristica {
private:
	double _heur;
public:
	heuristica() {};
	//Calcula la Distancia Euclídea entre un nodo dado y un nodo meta
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