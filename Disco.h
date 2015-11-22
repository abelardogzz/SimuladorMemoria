#ifndef DISCO_H_INCLUDED
#define DISCO_H_INCLUDED
#include <iostream>
#include <queue>
#include <vector>
#include <string>

struct Pagina
{
    string string nombreProceso;
    int marcoPagina, bitReferencia, bitModificacion, numeroPagina, bitResidencia;
};

class Disco
{
public:
	void guardarEnAreaSwap(Pagina pag);
	Pagina sacarDeAreaSwap();
	void liberarProceso(vector <Pagina> &paginasLiberadasSwap, string nombre);

private:
    Pagina tabla[]=new[512];
    string areaSwap[]=new[4096];
};


#endif // DISCO_H_INCLUDED
