#ifndef DISCO_H_INCLUDED
#define DISCO_H_INCLUDED
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <queue>
#include <string>

class Disco
{
public:
	//Constructor
	Disco();
	//Metodos publicos de la clase
	void guardarEnAreaSwap(struct Pagina pagina);
	struct Pagina sacarDeAreaSwap(string nombrePro, int numeroPag);
	void liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombrePro);



private:
	//Metodos privados de la clase
	//Se hacen matrices de Memoria de Disco y Paginas.
    struct Espacio tablaPaginas[512];
    string areaSwap[4096];
    int paginasLibres;
};

Disco::Disco ()
{
	//Se inicializan los espacios libres de Paginas
	paginasLibres = 512;
}

void Disco::guardarEnAreaSwap(struct Pagina pagina)
{
	//Inicia contador de posicion en la tabla de paginas
	int posicion = 0;
	//Se hace un ciclo que busca el primer espacio vacio en la tabla de paginas
	while(!tablaPaginas[posicion].estaVacio && paginasLibres > 0)
	{
		posicion++;
	}
	//Si no se encuentra un espacio vacio, se le notifica al usuario
	if(paginasLibres <= 0)
		cout<<"Memoria de Disco Duro llena"<<endl;
	else //Si se encuentra espacio vacio, se realizan las siguientes acciones
	{
		//Se insertan los datos de la pagina en la tabla
		tablaPaginas[posicion].pagina = pagina;
		//Se cambia el marco de pagina a la posicion actual en la pagina de memoria
		tablaPaginas[posicion].pagina.marcoPagina = posicion;
		//Se notifica que la posicion actual no se encuentra vacia
		tablaPaginas[posicion].estaVacio = false;
		paginasLibres--;
		//Se escribe en el primer y ultimo espacio de memoria indicando cuales estan ocupados.
		areaSwap[posicion*8] = "Pagina: " + pagina.nombreProceso;
		areaSwap[(posicion*8)+7] = "FIN de pagina: " + pagina.nombreProceso;
	}
}

struct Pagina Disco::sacarDeAreaSwap(string nombrePro, int numeroPag)
{
	int posicion = 0;
	//Se hace un ciclo que busca la pagina que tenga el nombre y numero que se pidieron
	while((tablaPaginas[posicion].pagina.nombreProceso != nombrePro) &&
		(tablaPaginas[posicion].pagina.numeroPagina != numeroPag)
		&& posicion<512)
	{
		posicion++;
	}
	//Si se salio del ciclo por salirse del rango, no se encontro el proceso, por lo tanto se le notifica al usuario.
	if(!tablaPaginas[posicion-1].estaVacio)
		cout<<"No se encontro el Proceso"<<endl;
	else //Si se encuentra el proceso, se hace el proceso de sacarlo
	{
		//Se notifica que el espacio de la tabla esta vacio para que se pueda sobrescribir sobre el.
		tablaPaginas[posicion].estaVacio = true;
		paginasLibres++;
		//Se borra el texto de las posiciones finales e iniciales en memoria
		areaSwap[posicion*8] = "";
		areaSwap[(posicion*8)+7] = "";
	}

}

void Disco::liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombrePro)
{
	//Se inicializa el vector de paginas liberadas
	vector <struct Pagina> *vectorPaginas = &paginasLiberadasSwap;
	//Se inicializa un booleano que nos servira para indicarnos si se encontraron paginas con el nombre del proceso.
	bool seEncontro = false;
	for(int posicion=0; posicion<512; posicion++)
	{
		if (tablaPaginas[posicion].pagina.nombreProceso == nombrePro)
		{
			//Se notifica que se encontro minimo 1 pagina con el nombre de proceso
			seEncontro = true;
			//Se notifica que el espacio de la tabla esta vacio para que se pueda sobrescribir sobre el.
			tablaPaginas[posicion].estaVacio = true;
			paginasLibres++;
			//Se borra el texto de las posiciones finales e iniciales en memoria
			areaSwap[posicion*8] = "";
			areaSwap[(posicion*8)+7] = "";
			//Agregamos la pagina que liberamos al vector
			vectorPaginas->push_back(tablaPaginas[posicion].pagina);
		}
	}
	//Si no se encontraron resultados, se notifica.
	if(!seEncontro)
		cout<<"No se encontro el Proceso"<<endl;
}

#endif // DISCO_H_INCLUDED
