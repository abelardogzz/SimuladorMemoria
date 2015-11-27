#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
using namespace std;
#include "Estructuras.h"
#include "Memoria.h"
#include "Disco.h"



int main()
{
    //Vectores de control de procesos
    vector <struct Pagina> paginasSwappeadas; //Contador de cantidad de Page Faults
    vector <struct Proceso> procesosSesion;
    vector <struct Pagina> paginasLiberadasEnSwap, paginasLiberadasEnMemoria;
    struct Pagina Pag1,Pag2;

    //Variables que definen el comportamiento de la memoria, RAM
    Memoria RAM ;

    ifstream ArchEntrada;//Archivo de Entrada, define las instrucciones
    ArchEntrada.open("Instrucciones.txt");
    //Declaraciones de caracteriticas de los procesos
    string nombreProceso,op;
    int bytes,dirVirtual;
    time_t tiempoaux;
    int bitLecMod;
    double turnaroundTotal;//8====================================================================D
    bool acceso=false;

    while (ArchEntrada>>op)//Lectura de archivo, la primer letra,
    {//La primer letra determina cual es la operacion a realizar

        if(op == "P")
        {//Operacion de iniciar un nuevo proceso
            ArchEntrada>>bytes;
            ArchEntrada >> nombreProceso;
            if(bytes>2048 || bytes <= 0)
                cout<<"Proceso: "<<nombreProceso<<" No cabe en memoria"<<endl;
            else
            {
                cout<<"Comando: "<<op<<" "<<bytes<<" "<<nombreProceso<<endl;
                RAM.cargarProceso(bytes,nombreProceso,paginasSwappeadas);
                //Agregacion al vector de procesos
                Proceso pro;
                pro.nombreProceso = nombreProceso;
                pro.tiempollegada = time(&tiempoaux);
                procesosSesion.push_back(pro);
            }
        }
        else if(op == "A")
        {//Operacion de acceso a un nuevo proceso
            ArchEntrada>>dirVirtual; //direccion virtual a accesar
            ArchEntrada >> nombreProceso; //nombre del proceso que se quiere accesar
            ArchEntrada >> bitLecMod; //modo en que se quiere accesar
            cout<<"Comando: "<<op<<" "<<dirVirtual<<" "<<nombreProceso<<" "<< bitLecMod<<endl;
            for(int i=0; i< procesosSesion.size(); i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso){
                    if(procesosSesion[i].tamano <= dirVirtual){
                        acceso =true;
                        procesosSesion[i].numPageFaults ++;
                        RAM.accesarProceso(dirVirtual,nombreProceso,Pag1,Pag2);

                        //Accesar, Lectura o modificacion, numero de paginas de swap
                    }
                    else
                    cout<<"La direccion de memoria no se puede accesar (PAGE OVERFLOW)"<<endl;
                }
            }
            if(!acceso)
                cout<<"Ese proceso no ha sido declarado"<<endl;
        }
        else if(op == "L")
        {//Operacion de liberacion de memoria, ocupada por un proceso
            acceso =false;
            ArchEntrada >> nombreProceso;
            cout<<"Comando: "<<op<<" "<<nombreProceso<<endl;
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso)
                    {
                        RAM.liberarProceso(nombreProceso,paginasLiberadasEnSwap,paginasLiberadasEnMemoria);
                        procesosSesion[i].tiempoSalida = time(&tiempoaux);
                        acceso =true;
                    }
            }
            if(!acceso)
                cout<<"Proceso: "<< nombreProceso <<", no se ha declarado"<<endl;
        }
        else if(op == "F")
        {//Fin de un secuencia de instrucciones, despliega un brief de lo realizado
            cout<<op<<"\nRESUMEN DE INTRUCCIONES"<<endl;
            cout<<"******************************"<<endl;
            int contProcesosTerminados=0,turnaroundProceso;
            //Finaliza una secuencia de isnturcciones
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].tiempoSalida != NULL)
                {
                    contProcesosTerminados++;
                    cout<<procesosSesion[i].tiempoSalida<<" TIEMPOS "<<procesosSesion[i].tiempollegada<<endl;
                    turnaroundProceso = difftime(procesosSesion[i].tiempoSalida,procesosSesion[i].tiempollegada);

                    cout<<"Turnaround: "<< ((turnaroundProceso)) << " segundos ";
                    cout<<" Proceso: "<< procesosSesion[i].nombreProceso;
                    cout<<" PageFaults: "<<procesosSesion[i].numPageFaults<<endl;
                    turnaroundTotal += turnaroundProceso;

                }
            }
            cout<<"Turnaround Promedio: "<<(turnaroundTotal / contProcesosTerminados)<<endl;

            cout<<"Total Swapp-in's: "<< RAM.getTotalSwapIns()<<endl;
            cout<<"Total Swapp-out's: "<< RAM.getTotalSwapOuts()<<endl;
            RAM.resetTotalSwapIns();
            RAM.resetTotalSwapOuts();
            cout<<"******************************"<<endl;

        }
        else if(op == "E")
        {//Terminacion del programa
            cout<<op<<"\nFIN DE PROGRAMA"<<endl;
        }
        else
        {//Mensaje de error al recibir un comando no registrado como valido
            cout<<"*Operacion invalida \" "<<op<<"\""<<endl;

        }
    }

    ArchEntrada.close();


    return 0;
}
