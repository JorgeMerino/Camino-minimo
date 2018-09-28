#pragma once

#ifndef vista_h
#define vista_h

#include <windows.h>

const int ANCHO = 800, ALTO = 650;

typedef enum {
	NEGRO,
	AZULOSCURO,
	VERDEOSCURO,
	AGUAMARINA,
	GRANATE,
	MORADO,
	DORADO,
	BLANCOOSCURO,
	GRIS,
	AZUL,
	VERDE,
	TURQUESA,
	ROJO,
	ROSA,
	AMARILLO,
	BLANCO
}color;

void ajustarVentana();
void restaurarPunteroConsola();
void setColor(color const &colorTexto, color colorFondo = NEGRO);

//Le da un tamaño fijo a la consola, además de una posición inicial
void ajustarVentana() {
	HWND console = GetConsoleWindow();
	RECT r;
	//Almacena las dimensiones de la consola en r
	GetWindowRect(console, &r);
	//Mueve la ventana y la redimensiona
	MoveWindow(console, r.left, r.top, ANCHO, ALTO, TRUE);
}

/*Restaura el puntero de la consola a la posición (0, 0). De este modo
evitamos el parpadeo de la pantalla si estuviesemos usando system("cls")*/
void restaurarPunteroConsola() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 0, 0 };
	SetConsoleCursorPosition(hConsole, c);
}

//Modifica el color que se muestra por consola
void setColor(color const &colorTexto, color colorFondo) {
	int color = colorTexto + colorFondo * 16;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

#endif // vista