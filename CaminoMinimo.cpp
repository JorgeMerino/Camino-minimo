#include <iostream>
#include <iomanip> //Para setfill y setw
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "nodo.h"
#include "mapa.h"
#include "replanificacion.h"
#include "heuristica.h"
#include "vista.h"

using namespace std;

template <class T>
void establecerRecorrido(vector <T> &mapa, nodo const &ini, nodo const &fin, int const &valorLibres, int const &valorPenalizadas, Replanificacion const &replanificacion);
template <class T>
vector<nodo> listaVecinos(vector <T> const &mapa, nodo const &miNodo, int indicepadre, nodo fin, int const &valorLibres, int const &valorPenalizadas);
double acumuladoNodo(double acumuladorPadre, bool esDiagonal, int tipo, int const &valorLibres, int const &valorPenalizadas);
template <class T>
bool prohibido(vector <T> const &mapa, nodo const &miNodo);
nodo calcularMinimo(vector<nodo> const &abierta, int &indice);
vector<nodo> obtenerRecorrido(vector<nodo> const &cerrada);
template <class T>
void mostrarRecorrido(vector <T> &mapa, vector<nodo> const &recorrido, nodo const &ini, nodo const &fin, int const &valorLibres, int const &valorPenalizadas, Replanificacion const &replanificacion);

int main(int argc, char *argv[]) {
	nodo ini, fin;
	vector <vector<int>> mapa;
	Replanificacion replanificacion;
	int valorLibres, valorPenalizadas;
	char *nombreArchivo = "mapa1.txt";

	ajustarVentana();
	try {
		//argv[0] es el nombre del programa
		if (argc == 2)
			nombreArchivo = argv[1];
		else if (argc != 1) {
			cout << "\nOrden incorrecta. Pruebe con 'ejecutable.exe nombre_de_archivo.txt'";
			return 1;
		}

		mapa = cargarMapa(ini, fin, valorLibres, valorPenalizadas, nombreArchivo, replanificacion);

		system("cls");
		mostrarMapa(mapa, 0);
		system("pause");
		system("cls");
		establecerRecorrido(mapa, ini, fin, valorLibres, valorPenalizadas, replanificacion);
		system("pause");
	}
	catch (invalid_argument e) {
		cout << e.what() << endl;
		system("pause");
	}

	return 0;
}

//Aplicamos el algoritmo A estrella, para averiguar el camino con menor coste
template <class T>
void establecerRecorrido(vector <T> &mapa, nodo const &ini, nodo const &fin, int const &valorLibres, int const &valorPenalizadas, Replanificacion const &replanificacion) {
	heuristica heur(ini, fin);
	vector<nodo> abierta;
	vector<nodo> cerrada;
	int indice, i;
	bool estaEnAbierta;
	//Declaramos el nodo raiz, al que llamaremos mínimo, y lo insertamos en abierta
	nodo minimo (ini.getX(), ini.getY(), heuristica(ini, fin).getHeur(), ini.getAcumulador(), -1);
	abierta.push_back(minimo);
		
	try {
		//Mientras el minimo de abierta (o lo que es lo mismo, el ultimo de cerrada) no sea el nodo fin, continua
		while (minimo != fin) {
			//Si abierta, en una nueva iteración se queda vacía es que no hay solución posible, ya que no podremos
			//seguir añadiendo nodos en cerrada, y como abaca de entrar, el último de cerrada sabemos que no es el nodo meta
			if (abierta.empty())
				throw invalid_argument("Abierta se encuentra vacia");

			//Calculamos el nodo mínimo de abierta
			minimo = calcularMinimo(abierta, indice);

			//Lo quitamos de abierta y lo metemos en cerrada
			abierta.erase(abierta.begin() + indice);
			cerrada.push_back(minimo);

			//Tratamos cada vecino del nodo "minimo"
			for (nodo vecino : listaVecinos(mapa, minimo, cerrada.size() - 1, fin, valorLibres, valorPenalizadas)) {
				//Descartamos los vecinos prohibidos y los que contenga la lista cerrada
				if (!prohibido(mapa, vecino) && cerrada.end() == find(cerrada.begin(), cerrada.end(), vecino)) {
					//Comprobamos si está en abierta, de ser así miramos que el Acumulador (distancia Recorrida), sea menor
					//y lo actualizamos. Si no es menor se queda como está. Y si no está en abierta se añade.
					i = 0;
					estaEnAbierta = false;
					while (i < abierta.size() && !estaEnAbierta) {
						if (vecino == abierta[i]) {
							estaEnAbierta = true;
							if (vecino.getAcumulador() < abierta[i].getAcumulador())
								abierta[i] = vecino;
						}
						i++;
					}
					if (!estaEnAbierta)
						abierta.push_back(vecino);
				}
			}
		}
		//Obtenemos el recorrido, a partir de la lista cerrada, y lo mostramos
		vector <nodo> recorrido = obtenerRecorrido(cerrada);
		mostrarRecorrido(mapa, recorrido, ini, fin, valorLibres, valorPenalizadas, replanificacion);
	}
	catch (invalid_argument e) {
		mostrarMapa(mapa, 0);
		setColor(ROJO);
		cout << endl << setfill(' ') << setw(55) << "-- NO HAY SOLUCION POSIBLE --\n\n";
		setColor(BLANCO);
	}
}

//Establece los nodos vecinos de un nodo dado
template <class T>
vector<nodo> listaVecinos(vector <T> const &mapa, nodo const &miNodo, int indicepadre, nodo fin, int const &valorLibres, int const &valorPenalizadas) {
	vector<nodo> listaNodos;
	int dimX = mapa.size();
	int dimY = mapa[0].size();
	int x = miNodo.getX();
	int y = miNodo.getY();

	//Horizontales
	if (x > 0)
		listaNodos.push_back(nodo(x - 1, y, heuristica(nodo(x - 1, y), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), false, mapa[x - 1][y], valorLibres, valorPenalizadas), indicepadre));
	if (x < dimX - 1)
		listaNodos.push_back(nodo(x + 1, y, heuristica(nodo(x + 1, y), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), false, mapa[x + 1][y], valorLibres, valorPenalizadas), indicepadre));
	//Verticales
	if (y > 0)
		listaNodos.push_back(nodo(x, y - 1, heuristica(nodo(x, y - 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), false, mapa[x][y - 1], valorLibres, valorPenalizadas), indicepadre));
	if (y < dimY - 1)
		listaNodos.push_back(nodo(x, y + 1, heuristica(nodo(x, y + 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), false, mapa[x][y + 1], valorLibres, valorPenalizadas), indicepadre));
	//Diagonales
	if (x > 0 && y > 0)
		listaNodos.push_back(nodo(x - 1, y - 1, heuristica(nodo(x - 1, y - 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), true, mapa[x - 1][y - 1], valorLibres, valorPenalizadas), indicepadre));
	if (x > 0 && y < dimY - 1)
		listaNodos.push_back(nodo(x - 1, y + 1, heuristica(nodo(x - 1, y + 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), true, mapa[x - 1][y + 1], valorLibres, valorPenalizadas), indicepadre));
	if (x < dimX - 1 && y > 0)
		listaNodos.push_back(nodo(x + 1, y - 1, heuristica(nodo(x + 1, y - 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), true, mapa[x + 1][y - 1], valorLibres, valorPenalizadas), indicepadre));
	if (x < dimX - 1 && y < dimY - 1)
		listaNodos.push_back(nodo(x + 1, y + 1, heuristica(nodo(x + 1, y + 1), fin).getHeur(), acumuladoNodo(miNodo.getAcumulador(), true, mapa[x + 1][y + 1], valorLibres, valorPenalizadas), indicepadre));

	return listaNodos;
}

//Acumula el coste que le corresponde a cada vecino, basandose en si es diagonal o no y si es penalizano o no
double acumuladoNodo(double acumuladorPadre, bool esDiagonal, int tipo, int const &valorLibres, int const &valorPenalizadas) {
	if (tipo != PENALIZADO && !esDiagonal)
		return acumuladorPadre + valorLibres;
	else if (tipo != PENALIZADO && esDiagonal)
		return acumuladorPadre + sqrt(pow(valorLibres, 2) + pow(valorLibres, 2));
	else if (tipo == PENALIZADO && !esDiagonal)
		return acumuladorPadre + valorPenalizadas;
	else if (tipo == PENALIZADO && esDiagonal)
		return acumuladorPadre + sqrt(pow(valorPenalizadas, 2) + pow(valorPenalizadas, 2));
}

template <class T>
bool prohibido(vector <T> const &mapa, nodo const &miNodo) {
	return mapa[miNodo.getX()][miNodo.getY()] == 1;
}

//Calcula en nodo mínimo de abierta, basandose en la distancia 
nodo calcularMinimo(vector<nodo> const &abierta, int &indice) {
	nodo minimo = abierta[0];
	indice = 0;
	for (int i = 1; i < abierta.size(); i++) {
		if (minimo.getDistancia() > abierta[i].getDistancia()) {
			minimo = abierta[i];
			indice = i;
		}
	}

	return minimo;
}

//Vuelca la lista cerrada en la lista de recorrido, empezando por el último nodo
//y accediendo a los padres de cada uno
vector<nodo> obtenerRecorrido(vector<nodo> const &cerrada) {
	vector <nodo> recorrido, recorridoInverso;
	int indice;

	recorrido.push_back(cerrada[cerrada.size() - 1]);
	indice = cerrada[cerrada.size() - 1].getPadre();

	while (indice > 0) {
		recorrido.push_back(cerrada[indice]);
		indice = cerrada[indice].getPadre();
	}

	//Damos la vuelta al recorrido, para que se pueda leer desde el inicio hasta la meta
	recorridoInverso = vector <nodo>(recorrido.size());
	for (int i = 0; i < recorridoInverso.size(); i++) {
		recorridoInverso[i] = recorrido[recorrido.size() - 1 - i];
	}

	return recorridoInverso;
}

//Para cada nodo del recorrido, actualiza el mapa y lo muestra cada vez que avanza
template <class T>
void mostrarRecorrido(vector <T> &mapa, vector<nodo> const &recorrido, nodo const &ini, nodo const &fin, int const &valorLibres, int const &valorPenalizadas, Replanificacion const &replanificacion) {
	for (int i = 0; i < recorrido.size(); i++) {
		//Si replanificacion tiene la estructura especial, es que está trabajando sobre la replanificacion dada
		if (!replanificacion.OK && replanificacion.etapa == -1 && replanificacion.nuevo == nodo(-1, -1)) {
			mapa[recorrido[i].getX()][recorrido[i].getY()] = REPLANIFICADO;
		}//Sino establece el recorrido normal
		else
			mapa[recorrido[i].getX()][recorrido[i].getY()] = RECORRIDO;
		
		//Si hay replanificación y llega a la etapa, cambia el nodo a prohibido y replanifica (si pertenece a recorrido)
		if (replanificacion.OK && replanificacion.etapa == i){
			//Marca el nuevo nodo replanificado como prohibido
			mapa[replanificacion.nuevo.getX()][replanificacion.nuevo.getY()] = PROHIBIDO;
			//Si el nodo de replanificacion está contenido en recorrido, lo replanifica
			if (recorrido.end() != find(recorrido.begin() + i + 1, recorrido.end(), replanificacion.nuevo)) {
				mostrarMapa(mapa, recorrido[i].getAcumulador()); //Muestra el mapa
				//Llama a establecerRecorrido, con recorrido[i] como nodo inicio y construye una replanificación especial para saber que ya ha sido replanificado
				establecerRecorrido(mapa, recorrido[i], fin, valorLibres, valorPenalizadas, Replanificacion{false, -1, nodo(-1, -1)});
				//Rompe la ejecución actual
				break;
			}
		}

		mostrarMapa(mapa, recorrido[i].getAcumulador());
	}
}