#ifndef DISCO_H_INCLUDED
#define DISCO_H_INCLUDED
#include <iostream>
#include <queue>
#include <vector>
#include <string>

struct Pagina
{
    string nombreProceso;
    int marcoPagina, bitReferencia, bitModificacion, numeroPagina, bitResidencia;
};

struct Espacio
{
	struct Pagina pagina;
	bool estaVacio = true;
};

class Disco
{
public:
	void guardarEnAreaSwap(struct Pagina pagina)
	{
		int posicion = 0;
		bool hayEspacioVacio = false;
		while(!tabla[posicion].estaVacio && posicion<512)
		{
			posicion++;
		}
		if(!tabla[posicion-1].estaVacio)
			cout<<"Memoria de Disco Duro llena"<<endl;
		else
		{
			tabla[posicion].pagina = pagina;
			tabla[posicion].estaVacio = false;
			areaSwap[posicion*8] = "Pagina: " + pagina.nombreProceso;
			areaSwap[(posicion*8)+7] = "FIN de pagina: " + pagina.nombreProceso;
		}
	}

	struct Pagina sacarDeAreaSwap()
	{}

	void liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombreProceso)
	{}

private:
    struct Espacio tabla[512];
    string areaSwap[4096];
};


#endif // DISCO_H_INCLUDED
