#pragma once

#ifndef replanificacion_h
#define replanificacion_h

#include "nodo.h"

typedef struct {
	bool OK;
	int etapa; //La etapa comienza en cero 
	nodo nuevo;
}Replanificacion;

#endif // replanificacion