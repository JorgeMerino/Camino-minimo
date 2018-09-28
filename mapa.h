#pragma once

#ifndef mapa_h
#define mapa_h

#include <iostream>
#include <iomanip> //Para setfill y setw
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include "nodo.h"
#include "replanificacion.h"
#include "vista.h"

using namespace std;

const string CENTINELA = "XXX"; //Establecemos un centinela para el fin del archivo

vector <vector<int>> cargarMapa(nodo &ini, nodo &fin, int &valorLibres, int &valorPenalizadas, char *nombreArchivo, Replanificacion &replanificacion);
template <class T>
void mostrarMapa(vector <T> const &mapa, double coste);

//Abre el mapa desde un fichero dado
vector <vector<int>> cargarMapa(nodo &ini, nodo &fin, int &valorLibres, int &valorPenalizadas, char *nombreArchivo, Replanificacion &replanificacion) {
	vector <vector<int>> mapa;
	ifstream archivo;
	int dimX, dimY, contInicio = 0, contFin = 0, replanX, replanY;
	string c;	

	archivo.open(nombreArchivo);

	if (archivo.is_open()) {
		archivo >> dimX >> dimY >> valorLibres >> valorPenalizadas >> replanificacion.OK;
		if (dimX < 3 || dimX > 30 || dimY < 3 || dimY > 30) //Excepciones de rango
			throw invalid_argument("Rango no permitido. Debe oscilar entre [3-30][3-30]");

		if(valorLibres < 1 || valorPenalizadas <= valorLibres) //Excepciones de los valores
			throw invalid_argument("Los valores de los nodos no son validos");

		if (replanificacion.OK) {
			archivo >> replanificacion.etapa >> replanX >> replanY;
			if (replanificacion.etapa < 0 || replanX < 0 || replanX >= dimX || replanY < 0 || replanY >= dimY)
				throw invalid_argument("Nodo de replanificacion incorrecto");
			replanificacion.nuevo = nodo(replanX, replanY);
		}

		mapa = vector <vector<int>>(dimX, vector<int>(dimY));
		for (int i = 0; i < dimX; i++) {
			for (int j = 0; j < dimY; j++) {
				archivo >> mapa[i][j];
				if(!archivo) //Si lee del archivo y no hay más informacion, lanza una excepción
					throw invalid_argument("El archivo esta corrompido");

				if (mapa[i][j] == INICIO) {
					ini = nodo(i, j);
					contInicio++;
				}					
				else if (mapa[i][j] == FIN) {
					fin = nodo(i, j);
					contFin++;
				}					
			}
		}

		if (contInicio != 1 || contFin != 1) //Si no hay justo un nodo inicio y un nodo meta, salta una excepción
			throw invalid_argument("El mapa no se pudo cargar. El mapa debe tener un solo nodo inicio y otro meta");
		if (replanificacion.OK && (replanificacion.nuevo == ini || replanificacion.nuevo == fin)) //Si el nodo replanificacion es nodo inicio o meta, salta una excepción
			throw invalid_argument("El nodo de replanificacion no puede ser el nodo inicio ni el nodo meta");

		archivo >> c;
		if (c != CENTINELA) //Si no ha llegado al CENTINELA, lanza una excepción
			throw invalid_argument("El archivo esta corrompido");
	}
	else
		throw invalid_argument("El archivo no existe");

	archivo.close();

	return mapa;
}

//Imprime por pantalla el mapa
template <class T>
void mostrarMapa(vector <T> const &mapa, double coste) {
	int dimX = mapa.size();
	int dimY = mapa[0].size();
	restaurarPunteroConsola(); //En cada impresión restauramos el puntero a la posición (0, 0)
	setColor(BLANCO);
	cout << "Ingenieria del Conocimiento - Practica 1 - Jorge Merino Martinez\n\n";

	cout << setfill(' ') << setw(45) << "-- MAPA --\n\n";
	for (int i = 0; i < dimX; i++) {
		cout << setfill(' ') << setw((80 - dimY * 2) / 2);
		for (int j = 0; j < dimY; j++) {
			if (mapa[i][j] == LIBRE) //Nodo Libre
				setColor(BLANCO);
			else if (mapa[i][j] == PROHIBIDO) //Nodo Prohibido
				setColor(ROJO);
			else if (mapa[i][j] == PENALIZADO) //Nodo con Penalizacion
				setColor(GRIS);
			else if (mapa[i][j] == INICIO) //Nodo de Inicio
				setColor(VERDEOSCURO);
			else if (mapa[i][j] == FIN) //Nodo Final
				setColor(AZUL);
			else if (mapa[i][j] == RECORRIDO) //Nodo de Recorrido
				setColor(VERDE);
			else if (mapa[i][j] == REPLANIFICADO) //Nodo de Recorrido con Replanificación
				setColor(MORADO);
			cout << char(219) << char(219);
		}
		cout << endl;
	}

	setColor(BLANCO);
	cout << endl << setfill(' ') << setw(39) << "COSTE: " << fixed << setprecision(3) << coste << endl;
	
	cout << "\n\n  -- LEYENDA -- \n\n";
	cout << char(219) << char(219) << " - Nodo Libre" << endl;
	setColor(ROJO);
	cout << char(219) << char(219) << " - Nodo Prohibido" << endl;
	setColor(GRIS);
	cout << char(219) << char(219) << " - Nodo con Penalizacion" << endl;
	setColor(VERDEOSCURO);
	cout << char(219) << char(219) << " - Nodo de Inicio" << endl;
	setColor(AZUL);
	cout << char(219) << char(219) << " - Nodo Final" << endl;
	setColor(VERDE);
	cout << char(219) << char(219) << " - Recorrido" << endl;
	setColor(MORADO);
	cout << char(219) << char(219) << " - Recorrido con Replanificacion\n" << endl;
	setColor(BLANCO);

	Sleep(300);
}

#endif // mapa