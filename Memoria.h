#ifndef MEMORIA_H_INCLUDED
#define MEMORIA_H_INCLUDED

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cmath>
#include "Disco.h"

using namespace std;

class Memoria
{

public:
  //Constructor
  Memoria();

  //Metodos pubicos de la clase
  /**
    Se carga un proceso en la memoria.

    @bytesProceso el tama;o del proceso en bytes

    @nombreProceso el nombre del proceso

    @&paginasSwappeadas parametro por referencia que almacenara las paginas que se sacaron debido a la carga de este proceso

    @return void
  */
  void cargarProceso(int bytesProceso,string nombreProceso,vector <struct Pagina> &paginasSwappeadas);
  /**
    Se accede a un proceso que esta activo en memoria. Si no esta cargado en RAM,
    ejecuta swapOut si es necesario, y almacena donde se guardo la pagina accesada en paginaNuevaMemoria
    y la pagina sacada en paginaSacada

    @dirVirtual la direccion virtual a acceder del proceso

    @nombreProceso el nombre del proceso a acceder

    @&paginaSacada parametro por referencia que almacenara, solamente si fue necesario, la pagina que tuvo
    que sacar de memoria para meter la pagina que se esta accediendo

    @&paginaNuevaMemoria parametro por referencia que almacenara, solamente si fue necesario, la pagina que meterse
    en la memoria (en el dado caso que no estaba en memoria real, y estaba en disco)

    @return void
  */
  void accesarProceso(int dirVirtual,string nombreProceso,struct Pagina &paginaSacada,struct Pagina &paginaNuevaMemoria);
  /**
    Se liberan todas las paginas del proceso en cuestion

    @nombreProceso el nombre del proceso cuyas paginas seran liberadas

    @&paginasLiberadasDisco parametro por referencia que, solo si fue necesario, regresara un vector
    que contendra las paginas que se liberaron en disco

    @&paginasLiberadasMemoria parametro por referencia que regresara un vector que contendra las paginas que se liberaron
    en memoria

    @return void
  */
  void liberarProceso(string nombreProceso,vector <struct Pagina> &paginasLiberadasDisco, vector <struct Pagina> &paginasLiberadasMemoria);



private:
    //Atributos privados de la clase
    queue <struct Pagina> queuePaginas; //Este simulador de memoria utiliza un FIFO para sacar paginas
    struct Espacio tablaPaginas[256]; //En esta tabla se simula la tabla de paginas
    string memoria[2048];//Esta es la "memoria"
    int paginasLibres;//La cantidad de paginas libres en el sistema
    Disco disco; //La memoria tiene acceso al disco, en caso de que necesite hacer swaps
    int totalSwapIns;//Contador que nos dice la cantidad total de swap ins de la sesion
    int totalSwapOuts;//Contador que nos dice la cantidad total de swap outs de la sesion

    //Métodos privados de clase

    /**
      Se trae a memoria real la pagina que esta en area swap, y se regresa esta pagina

      @nombreProceso el nombre del proceso del cual se requiere traer la pagina

      @numPagina numero de pagina del proceso que queremos traer de memoria

      @return Pagina
    */
    struct Pagina swapIn(string nombreProceso,int numPagina);

    /**
      Se mueve de la memoria real al area swap la(s) pagina(s) que se requiere(n) sacar para meter el proceso

      @nombreProceso el nombre del proceso con el cual guardaremos la(s) pagina(s)

      @bytes el total de bytes que guardaremos en memoria

      @numSwapsNecesarios entero que nos servira para saber cuantas paginas tenemos que sacar

      @&paginasSwappeadas vector que almacenara las paginas que se movieron de la memoria real al area swap

      @return void
    */
    void swapOut(string nombreProceso,int bytes,int numSwapsNecesarios,vector <struct Pagina> &paginasSwappeadas);

    /**
      Método que se encarga de meter las paginas necesarias dado un número de bytes para un proceso

      @bytesProceso el total de bytes que guardaremos en memoria

      @nombreProceso el nombre del proceso

      @return void
    */
    void meterPaginasDeProceso(int bytesProceso,string nombreProceso);



};

Memoria::Memoria()
{
  //Se inicializan las variables
  this->totalSwapIns = 0;
  this->totalSwapOuts = 0;
  this->paginasLibres = 256;
  //this->disco = new Disco();
}


void Memoria::cargarProceso(int bytesProceso,string nombreProceso,vector <struct Pagina> &paginasSwappeadas)
{
  //struct Pagina pagina;
  vector <struct Pagina> *vecPaginasSwappeadas = &paginasSwappeadas;

  //Calculo la cantidad de paginas que requerira la asignacion de ese proceso
  int numPaginasRequeridas = bytesProceso/8;

  //Si hay espacio en memoria real, asigno las paginas
  if  (numPaginasRequeridas <= this->paginasLibres){
    this->meterPaginasDeProceso(bytesProceso,nombreProceso);
  }
  //Si no hay espacio disponible, hay que swappear-out
  else{
    //Determino cuantas paginas quitaré
    int numPaginasAQuitar = ceil(numPaginasRequeridas - paginasLibres);
    //this->swapOut();
    //vectorPaginasSwappeadas->push_back(tabla[posicion].pagina);
  }

}

void Memoria::swapOut(string nombreProceso,int bytes,int numSwapsNecesarios,vector <struct Pagina> &paginasSwappeadas){

  vector <struct Pagina> *vecPaginasSwappeadas = &paginasSwappeadas;
  //Hago los swap outs necesarios, para liberar memoria
  for (int i=0;i<numSwapsNecesarios;i++){

      //this->tablaPaginas[i].
      struct Pagina paginaASacar = this->queuePaginas.front();
      this->queuePaginas.pop();
      this->tablaPaginas[paginaASacar.marcoPagina].estaVacio = true;
      vecPaginasSwappeadas->push_back(paginaASacar);

      //Se limpia la memoria
      this->memoria[paginaASacar.marcoPagina*8] = "";
      this->memoria[(paginaASacar.marcoPagina*8)+7] = "";

      this->paginasLibres++;
      this->totalSwapOuts++;
      //this->disco.guardarEnAreaSwap(paginaASacar);
  }
  //Una vez liberada la memoria, ahora si se meten las paginas del proceso
  this->meterPaginasDeProceso(bytes,nombreProceso);


}

void Memoria::meterPaginasDeProceso(int bytesProceso,string nombreProceso){
  //Calculo la cantidad de paginas que requerira la asignacion de ese proceso
  int numPaginasRequeridas = bytes/8;

    //Por cada pagina, hare una iteracion para buscar posiciones libres en la tabla de paginas
    for (int numPagina = 0; numPagina < numPaginasRequeridas; numPagina++){
      for(int posicion=0; posicion<256; posicion+8)
      {
        if (this->tablaPaginas[posicion].estaVacio)
        {
          //Le asigno a esa pagina los valores iniciales
          this->tablaPaginas[posicion].pagina.numeroPagina = numPagina;
          this->tablaPaginas[posicion].pagina.marcoPagina = posicion;
          this->tablaPaginas[posicion].pagina.nombreProceso = nombreProceso;
          this->tablaPaginas[posicion].pagina.bitReferencia = 0;
          this->tablaPaginas[posicion].pagina.bitModificacion = 0 ;
          this->tablaPaginas[posicion].pagina.bitResidencia = 1;

          //Ya no está vacío ese marco
          this->tablaPaginas[posicion].estaVacio = false;

          //En memoria le pongo strings, solo para facilitar el debugging
          this->memoria[posicion*8] = "Proceso "+nombreProceso;
          this->memoria[(posicion*8)+7] = "Fin de proceso "+nombreProceso;

          //Meto en la queue esta pagina, pues es la que sacaremos si es que se ocupa hacer un swap out
          this->queuePaginas.push(tablaPaginas[posicion].pagina);
          this->paginasLibres--;
          break;
        }
      }
    }
}

#endif // MEMORIA_H_INCLUDED
