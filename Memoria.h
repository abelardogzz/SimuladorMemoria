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
    ejecuta swapIn y swapOut si es necesario, y almacena donde se guardo la pagina accesada en paginaNuevaMemoria
    y la pagina sacada en paginaSacada

    @dirVirtual la direccion virtual a acceder del proceso

    @nombreProceso el nombre del proceso a acceder

    @&paginaSacada parametro por referencia que almacenara, solamente si fue necesario, la pagina que tuvo
    que sacar de memoria para meter la pagina que se esta accediendo

    @&paginaNuevaMemoria parametro por referencia que almacenara, solamente si fue necesario, la pagina que meterse
    en la memoria (en el dado caso que no estaba en memoria real, y estaba en disco)

    @return void
  */
  bool accesarProceso(int dirVirtual,string nombreProceso,struct Pagina &paginaSacada,struct Pagina &paginaNuevaMemoria);
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
  /**
    Se obtiene el número total de Swap Ins de la sesion

    @return int
  */
  int getTotalSwapIns();
  /**
    Se obtiene el número total de Swap Outs de la sesion

    @return int
  */
  int getTotalSwapOuts();
  /**
    Se resettea el número total de Swap Ins de la sesion

    @return void
  */
  void resetTotalSwapIns();
  /**
    Se resettea el número total de Swap Outs de la sesion

    @return void
  */
  void resetTotalSwapOuts();


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
    void swapIn(string nombreProceso,int numPagina, int dirVirtual);

    /**
      Se mueve de la memoria real al area swap la(s) pagina(s) que se requiere(n) sacar para meter el proceso

      @nombreProceso el nombre del proceso con el cual guardaremos la(s) pagina(s)

      @bytes el total de bytes que guardaremos en memoria

      @numSwapsNecesarios entero que nos servira para saber cuantas paginas tenemos que sacar

      @&paginasSwappeadas vector que almacenara las paginas que se movieron de la memoria real al area swap

      @return void
    */
    void swapOut(int numSwapsNecesarios,vector <struct Pagina> *paginasSwappeadas);

    /**
      Método que se encarga de meter las paginas necesarias dado un número de bytes para un proceso

      @bytesProceso el total de bytes que guardaremos en memoria

      @nombreProceso el nombre del proceso

      @return void
    */
    void meterPaginasDeProceso(int bytesProceso,string nombreProceso,vector <struct Pagina> &marcosDePaginaAsignados);



};

Memoria::Memoria()
{
  //Se inicializan las variables
  this->totalSwapIns = 0;
  this->totalSwapOuts = 0;
  this->paginasLibres = 256;
}


void Memoria::cargarProceso(int bytesProceso,string nombreProceso,vector <struct Pagina> &paginasSwappeadas)
{
  //struct Pagina pagina;
  vector <struct Pagina> *vecPaginasSwappeadas = &paginasSwappeadas;
  //Calculo la cantidad de paginas que requerira la asignacion de ese proceso
  int numPaginasRequeridas = ceil(bytesProceso/8.0);
  //Si hay espacio en memoria real, asigno las paginas
  //cout << "Paginas requeridas " << numPaginasRequeridas << endl;
  if  (numPaginasRequeridas <= this->paginasLibres){

  }
  //Si no hay espacio disponible, hay que swappear-out
  else{
    //Determino cuantas paginas quitaré
    int numPaginasAQuitar = ceil(numPaginasRequeridas - paginasLibres);
    //cout << "Num Paginas a Quitar :" << numPaginasAQuitar <<endl;
    this->swapOut(numPaginasAQuitar,vecPaginasSwappeadas);
    //vectorPaginasSwappeadas->push_back(tabla[posicion].pagina);
    // struct Pagina paginaIni;
    // struct Pagina paginaFini;

  }
  //A continuacion se meten las paginas del proceso
  vector <struct Pagina> marcosDePaginaAsignados;
  this->meterPaginasDeProceso(bytesProceso,nombreProceso,marcosDePaginaAsignados);
  int inicio=marcosDePaginaAsignados[0].marcoPagina;
  int final=-1;
  cout << "Memoria - Se asignaron los siguientes marcos de memoria al proceso " + nombreProceso << endl;

  //Este for imprime que marcos de pagina se asignaron
  for(std::vector<int>::size_type i = 0; i != marcosDePaginaAsignados.size(); i++) {
    struct Pagina marco = marcosDePaginaAsignados[i];
    if (inicio+1==marcosDePaginaAsignados[i].marcoPagina){
      final = marcosDePaginaAsignados[i].marcoPagina;
    }
    else if (inicio+1!=marcosDePaginaAsignados[i].marcoPagina && final==-1){
      cout << "[" + to_string(inicio) + "-";
    }
    //Esto significa que ahora se cambio de marco, y se imprime el final pasado y se cambia
    //el inicio ahora
    else if (inicio+1!=marcosDePaginaAsignados[i].marcoPagina && final!=-1){
      cout <<  to_string(final) + "]";
      inicio = marcosDePaginaAsignados[i].marcoPagina;
      cout << "[" + to_string(inicio) + "-";
      final = -1;
    }

    if (i == marcosDePaginaAsignados.size()-1){
      final = marcosDePaginaAsignados[i].marcoPagina;
      cout <<  to_string(final) + "]";
    }

    inicio = marcosDePaginaAsignados[i].marcoPagina;
  }
  cout << endl;



}

bool Memoria::accesarProceso(int dirVirtual,string nombreProceso,struct Pagina &paginaSacada,struct Pagina &paginaNuevaMemoria)
{
  int paginaDelProceso = dirVirtual/8;
  int offsetDePagina = dirVirtual%8;
  bool encontro = false;
  struct Pagina *paginaCreada = &paginaNuevaMemoria;
  struct Pagina *paginaSwappeada = &paginaSacada;
  for (int i=0;i<256;i++){

    //Se busca el proceso y su pagina, para saber en donde esta alocado
    if (!this->tablaPaginas[i].estaVacio
      && this->tablaPaginas[i].pagina.nombreProceso == nombreProceso
      && this->tablaPaginas[i].pagina.numeroPagina == paginaDelProceso ){
          encontro = true;
          cout << "Direccion virtual del proceso "+ nombreProceso +" es: "+to_string(dirVirtual)+" y su direccion real es: "+
          to_string(this->tablaPaginas[i].pagina.marcoPagina*8+offsetDePagina) << endl;
          return false;
      }

  }

  if (!encontro){
    //Se intenta hacer el swap in
    //cout << "¡Esa dirección virtual no corresponde a este proceso!" << endl;
    this->swapIn(nombreProceso,paginaDelProceso,dirVirtual);
    return true;
  }

}

void Memoria::liberarProceso(string nombreProceso,vector <struct Pagina> &paginasLiberadasDisco, vector <struct Pagina> &paginasLiberadasMemoria)
{
  // vector <struct Pagina>  *vectorPaginasLiberadas= &paginasLiberadasMemoria;
  vector <struct Pagina>  vectorPaginasLiberadas;
  vector <struct Pagina>  vectorPaginasLiberadasDisco;
  //Se hace un ciclo en donde se buscan todas las paginas del proceso
  for (int i=0;i<256;i++){
    //Se busca el proceso y su pagina, para saber en donde esta alocado
    if (!this->tablaPaginas[i].estaVacio && this->tablaPaginas[i].pagina.nombreProceso == nombreProceso){
          this->tablaPaginas[i].estaVacio = true;
          vectorPaginasLiberadas.push_back(this->tablaPaginas[i].pagina);
          this->paginasLibres++;
      }
  }
  if (vectorPaginasLiberadas.size()!=0){
    int inicio=vectorPaginasLiberadas[0].marcoPagina;
    int final=-1;
    cout << "Memoria - Se liberaron los siguientes marcos de memoria real que eran ocupados por el proceso " + nombreProceso << endl;
    //Este for imprime que marcos de pagina se asignaron
    for(std::vector<int>::size_type i = 0; i != vectorPaginasLiberadas.size(); i++) {
      struct Pagina marco = vectorPaginasLiberadas[i];
      if (inicio+1==vectorPaginasLiberadas[i].marcoPagina){
        final = vectorPaginasLiberadas[i].marcoPagina;
      }
      else if (inicio+1!=vectorPaginasLiberadas[i].marcoPagina && final==-1){
        cout << "[" + to_string(inicio) + "-";
      }
      else if (inicio+1!=vectorPaginasLiberadas[i].marcoPagina && final!=-1){
        cout <<  to_string(final) + "]";
        inicio = vectorPaginasLiberadas[i].marcoPagina;
        cout << "[" + to_string(inicio) + "-";
        final = -1;
      }

      if (i == vectorPaginasLiberadas.size()-1){
        final = vectorPaginasLiberadas[i].marcoPagina;
        cout <<  to_string(final) + "]";
      }

      inicio = vectorPaginasLiberadas[i].marcoPagina;
    }
    cout << endl;

  }
  disco.liberarProceso(vectorPaginasLiberadasDisco,nombreProceso);


}

void Memoria::swapOut(int numSwapsNecesarios,vector <struct Pagina> *paginasSwappeadas){

  vector <struct Pagina> *vecPaginasSwappeadas = paginasSwappeadas;
  //Hago los swap outs necesarios, para liberar memoria
  for (int i=0;i<numSwapsNecesarios;i++){

      struct Pagina paginaASacar = this->queuePaginas.front();
      this->queuePaginas.pop();
      this->tablaPaginas[paginaASacar.marcoPagina].estaVacio = true;
      vecPaginasSwappeadas->push_back(paginaASacar);

      //Se limpia la memoria
      this->memoria[paginaASacar.marcoPagina*8] = "";
      this->memoria[(paginaASacar.marcoPagina*8)+7] = "";

      //Se hace el swap out de esa pagina, es decir, se manda a disco
      this->disco.guardarEnAreaSwap(paginaASacar);

      this->paginasLibres++;
      this->totalSwapOuts++;
      //this->disco.guardarEnAreaSwap(paginaASacar);
  }

}

void Memoria::meterPaginasDeProceso(int bytesProceso,string nombreProceso,vector <struct Pagina> &marcosDePaginaAsignados){
  //Calculo la cantidad de paginas que requerira la asignacion de ese proceso
  int numPaginasRequeridas = ceil(bytesProceso/8.0);
  vector <struct Pagina> *marcosDePagina = &marcosDePaginaAsignados;
    //Por cada pagina, hare una iteracion para buscar posiciones libres en la tabla de paginas

    for (int numPagina = 0; numPagina < numPaginasRequeridas; numPagina++){
      for(int posicion=0; posicion<256; posicion++)
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
          this->memoria[posicion*8] = "Pagina "+to_string(numPagina)+" del proceso "+nombreProceso;
          this->memoria[(posicion*8)+7] = "Fin de pagina "+nombreProceso;
          //Se decrementa en uno el contador de paginas libres
          this->paginasLibres--;
          //Meto en la queue esta pagina, pues es la que sacaremos si es que se ocupa hacer un swap out
          this->queuePaginas.push(tablaPaginas[posicion].pagina);

          marcosDePagina->push_back(tablaPaginas[posicion].pagina);

          break;
        }
      }

    }

}

void Memoria::swapIn(string nombreProceso,int numPagina, int dirVirtual){
  //Saco de Area swap la pagina que quiero, y la pongo
  struct Pagina *paginaSwappeadaIn;
  int offsetDePagina = dirVirtual%8;
  vector <struct Pagina> *vecPaginasSwappeadas = new vector <struct Pagina>;

  //Si no hay paginas libres, hay que swappearOut
  if (this->paginasLibres==0){
      cout << "ATENCIÓN: No hay páginas libres para hacer el swapIn. A continuación se hará un swapOut." << endl;
      this->swapOut(1,vecPaginasSwappeadas);
  }
  paginaSwappeadaIn = this->disco.sacarDeAreaSwap(nombreProceso,numPagina);

  //Y la meto manualmente
  if (paginaSwappeadaIn != NULL){
    for(int i=0; i<256; i++){
      if (this->tablaPaginas[i].estaVacio)
      {
        //Le asigno a esa pagina los valores iniciales
        this->tablaPaginas[i].pagina.nombreProceso = paginaSwappeadaIn->nombreProceso;
        this->tablaPaginas[i].pagina.numeroPagina = paginaSwappeadaIn->numeroPagina;
        this->tablaPaginas[i].pagina.marcoPagina = i;
        //this->tablaPaginas[i].pagina. = i;
        //Ya no está vacío ese marco
        this->tablaPaginas[i].estaVacio = false;
        this->paginasLibres--;
        //Meto en la queue esta pagina, pues es la que sacaremos si es que se ocupa hacer un swap out
        this->queuePaginas.push(tablaPaginas[i].pagina);

        cout << "Memoria - " <<"Se guardó la página " << this->tablaPaginas[i].pagina.numeroPagina
        <<" del proceso "<< this->tablaPaginas[i].pagina.nombreProceso << " en el marco de memoria " << i<<endl;

        cout << "Direccion virtual del proceso "+ nombreProceso +" es: "+to_string(dirVirtual)+" y su direccion real es: "+
        to_string(this->tablaPaginas[i].pagina.marcoPagina*8+offsetDePagina) << endl;
        break;
       }
    }
    this->totalSwapIns++;
  }

}
int Memoria::getTotalSwapIns(){
  return this->totalSwapIns;
}

int Memoria::getTotalSwapOuts(){
  return this->totalSwapOuts;
}

void Memoria::resetTotalSwapIns(){
  this->totalSwapIns = 0;
}

void Memoria::resetTotalSwapOuts(){
  this->totalSwapOuts = 0;
}


#endif // MEMORIA_H_INCLUDED
