#pragma once

#ifndef nodo_h
#define nodo_h

typedef enum {
	LIBRE,
	PROHIBIDO,
	PENALIZADO,
	INICIO,
	FIN,
	RECORRIDO,
	REPLANIFICADO
}tNodo;

class nodo {
private:
	int _x;
	int _y;
	double _heuristica;
	double _acumulador;
	double _distancia;
	int _indicePadre;
public:
	nodo() {};
	nodo(int x, int y) {
		_x = x;
		_y = y;
		_acumulador = 0; //Inicializamos el acumulador a 0
	}
	nodo(int x, int y, double heuristica, double acumulador, int indicePadre) {
		_x = x;
		_y = y;
		_heuristica = heuristica;
		_acumulador = acumulador;
		_distancia = _heuristica + _acumulador;
		_indicePadre = indicePadre;
	}
	int getX() const {
		return _x;
	}
	int getY() const {
		return _y;
	}
	int getPadre() const {
		return _indicePadre;
	}
	double getDistancia() const {
		return _distancia;
	}
	double getAcumulador() const {
		return _acumulador;
	}

	//Sobrecarga de operadores
	bool operator != (nodo miNodo) const {
		return _x != miNodo.getX() || _y != miNodo.getY();
	}
	bool operator == (nodo miNodo) const {
		return _x == miNodo.getX() && _y == miNodo.getY();
	}
};

#endif // nodo