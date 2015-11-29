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
	/**
    Se carga un proceso en espacio de memoria

    @pagina la esctructura de pagina que se desea cargar de memoria real a Disco.

    @return void
    */
	void guardarEnAreaSwap(struct Pagina pagina);

	/**
    Se saca un proceso de Disco con el numero de pagia que se le manda.

    @nombrePro el nombre de proceso que se busca para sacar de Disco

    @numeroPag el numero de la pagina que se desea sacar

    @return pagina Sacada regresa la pagina que se saco de Disco.
    */
	struct Pagina * sacarDeAreaSwap(string nombrePro, int numeroPag);

	/**
    Se libera un proceso de Disco y se registra en un vector de referencia.

    @paginasLiberadasSwap direccion para hacer referencia al vector de las paginas liberadas, en este se guardan
    las paginas que fueron liberadas.

    @nombrePro nombre de los procesos que se busca liberar.

    @return void
    */
	void liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombrePro);



private:
	//Metodos privados de la clase
    struct Espacio tablaPaginas[512]; //Esta es la tabla que simula la tabla de paginas de Disco
    string areaSwap[4096]; //Esta tabla simula la memoria de Disco.
    int paginasLibres; //Contador de el numero de paginas disponibles.
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
		//Prueba de funcionalidad de guardado en tabla.
		cout<<"Memoria Swap - Se guardo la pagina " << tablaPaginas[posicion].pagina.numeroPagina<<" del proceso "<<tablaPaginas[posicion].pagina.nombreProceso<<" en el marco de pagina "<<posicion
	  <<" en la dirección " << posicion*8 << " de S en memoria swap "<<endl;
		//Se notifica que la posicion actual no se encuentra vacia
		tablaPaginas[posicion].estaVacio = false;
		paginasLibres--;
		//Se escribe en el primer y ultimo espacio de memoria indicando cuales estan ocupados.
		areaSwap[posicion*8] = "Pagina: " + pagina.nombreProceso;
		areaSwap[(posicion*8)+7] = "FIN de pagina: " + pagina.nombreProceso;

	}
}

struct Pagina * Disco::sacarDeAreaSwap(string nombrePro, int numeroPag)
{
	int posicion = 0;
	bool seEncontro = false;
	struct Pagina *paginaSacada;
	//Se hace un ciclo que busca la pagina que tenga el nombre y numero que se pidieron
	for (int i=0;i<512;i++){

        if ((tablaPaginas[i].pagina.nombreProceso == nombrePro) &&
		(tablaPaginas[i].pagina.numeroPagina == numeroPag) &&
		!tablaPaginas[i].estaVacio
		){
				seEncontro = true;
				paginaSacada = &tablaPaginas[i].pagina;
				cout << paginaSacada->numeroPagina << endl;
				tablaPaginas[i].estaVacio = true;
				//Prueba de funcionalidad De liberar tabla de paginas.
				cout<<"Memoria Swap - Se libero el marco de pagina "<<i<<" que contenia la pagina "<<tablaPaginas[i].pagina.numeroPagina<<" del proceso "
				<<nombrePro<<endl;
				paginasLibres++;
				//Se borra el texto de las posiciones finales e iniciales en memoria
				areaSwap[i*8] = "";
				areaSwap[(i*8)+7] = "";
				return paginaSacada;
		}
	}

	if (!seEncontro){
		cout << "Esa página no la tiene ese proceso!" << endl;
		paginaSacada = NULL;
	}
	return paginaSacada;

/*
	while((tablaPaginas[posicion].pagina.nombreProceso != nombrePro) &&
		(tablaPaginas[posicion].pagina.numeroPagina != numeroPag) &&
		&& posicion<512)
	{
		posicion++;
	}
	//Si se salio del ciclo por salirse del rango, no se encontro el proceso, por lo tanto se le notifica al usuario.

	if(tablaPaginas[posicion].estaVacio || posicion==512)
		cout<<"No se encontro el Proceso"<<endl;
	else //Si se encuentra el proceso, se hace el proceso de sacarlo
	{
		//Se notifica que el espacio de la tabla esta vacio para que se pueda sobrescribir sobre el.
		tablaPaginas[posicion].estaVacio = true;
		//Prueba de funcionalidad De liberar tabla de paginas.
		cout<<"Memoria Disco - Se libero la posicion: "<<posicion<<" en el marco de paginas."<<endl;
		paginasLibres++;
		//Se borra el texto de las posiciones finales e iniciales en memoria
		areaSwap[posicion*8] = "";
		areaSwap[(posicion*8)+7] = "";
		//Prueba de funcionalidad al liberar rango de memoria.
	}
*/

}

void Disco::liberarProceso(vector <struct Pagina> &paginasLiberadasSwap, string nombrePro)
{
	//Se inicializa el vector de paginas liberadas
	vector <struct Pagina> *vectorPaginas = &paginasLiberadasSwap;
	//Se inicializa un booleano que nos servira para indicarnos si se encontraron paginas con el nombre del proceso.
	for(int posicion=0; posicion<512; posicion++)
	{
		if (tablaPaginas[posicion].pagina.nombreProceso == nombrePro)
		{
			//Se notifica que el espacio de la tabla esta vacio para que se pueda sobrescribir sobre el.
			tablaPaginas[posicion].estaVacio = true;
			//Prueba de funcionalidad De liberar tabla de paginas.
            cout<<"Memoria Disco - Se libero la posicion: "<<posicion<<" en el marco de paginas."<<endl;
			paginasLibres++;
			//Se borra el texto de las posiciones finales e iniciales en memoria
			areaSwap[posicion*8] = "";
			areaSwap[(posicion*8)+7] = "";
			//Agregamos la pagina que liberamos al vector
			vectorPaginas->push_back(tablaPaginas[posicion].pagina);

		}
	}
}

#endif // DISCO_H_INCLUDED DISCO_H_INCLUDED
