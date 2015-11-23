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

	struct Pagina sacarDeAreaSwap(string nombrePro, int numeroPag)
	{
		int posicion = 0;
		while(!((tabla[posicion].pagina.nombreProceso != nombrePro) && 
			(tabla[posicion].pagina.numeroPagina != numeroPag)) 
			&& posicion<512)
		{
			posicion++;
		}
		if(!tabla[posicion-1].estaVacio)
			cout<<"No se encontro el Proceso"<<endl;
		else
		{
			tabla[posicion].estaVacio = true;
			areaSwap[posicion*8] = "";
			areaSwap[(posicion*8)+7] = "";
		}

	}

	void liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombrePro)
	{
		vector <struct Pagina> *vectorPaginas = &paginasLiberadasSwap;
		bool seEncontro = false;
		for(int posicion=0; posicion<512; posicion++)
		{
			if (tabla[posicion].pagina.nombreProceso == nombrePro)
			{
				seEncontro = true;
				tabla[posicion].estaVacio = true;
				areaSwap[posicion*8] = "";
				areaSwap[(posicion*8)+7] = "";
				vectorPaginas->push_back(tabla[posicion].pagina);
			}
		}
		if(!seEncontro)
			cout<<"No se encontro el Proceso"<<endl;
	}

private:
    struct Espacio tabla[512];
    string areaSwap[4096];
};


#endif // DISCO_H_INCLUDED
