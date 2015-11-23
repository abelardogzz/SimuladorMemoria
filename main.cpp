#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
using namespace std;
#include "Memoria.h"
/**
 *Archivo prueba
P 2048 3
A 1 1 0
L 1
F
E
 */
/**
 *Estructura Pagina que define las caracteristicas de una pagina
 *las cuales son utilizadas por los procesos
 *
*/
struct Pagina {
        string nombreProceso;
        int marcoPagina, bitReferencia, bitModificacion,
            numeroPagina, bitResidencia;
        };
/**
 *Estructura que define las caracteristicas de un proceso
 *con el proposito de llevar un registro de cuando entra y sale
 *cada proceso en memoria
*/
struct Proceso{
        string nombreProceso;
        time_t tiempollegada;
        time_t tiempoSalida;
        int numPageFaults;
    };
int main()
{
    //Vectores de control de procesos
    vector <Pagina> paginasSwappeadas; //Contador de cantidad de Page Faults
    vector <Proceso> procesosSesion;
    vector <Pagina> paginasLiberadasEnSwap, paginasLiberadasEnMemoria;

    //Variables que definen el comportamiento de la memoria, RAM
    //Memoria RAM = new Memoria();

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
            if(bytes>2048)
                cout<<"Proceso: "<<nombreProceso<<" No cabe en memoria"<<endl;
            else
            {
                cout<<"Tamaño de proceso: "<<bytes<<" ID "<<nombreProceso<<endl;
                //RAM.cargaProceso(int bytes,string processID, vector paginasSwappeadas);
                //Agregacion al vector de procesos
                Proceso pro;
                pro.nombreProceso = nombreProceso;
                pro.tiempollegada = time(&tiempoaux);
                procesosSesion.push_back(pro);
            }
        }
        else if(op == "A")
        {//Operacion de acceso a un nuevo proceso
            ArchEntrada>>dirVirtual;
            ArchEntrada >> nombreProceso;
            ArchEntrada >> bitLecMod;
            //cout<<"DirVirtual: "<<dirVirtual<<" ID: "<<nombreProceso<<" Lectura/Modificacion: "<< bitLecMod<<endl;
            //acceso = RAM.accesoProceso(int dirVirtual, string nombreProceso, int bitLecMod);
            if(acceso)
            {
                for(int i=0;i<procesosSesion.size();i++)
                {
                    if(procesosSesion[i].nombreProceso == nombreProceso)
                        procesosSesion[i].numPageFaults ++;
                }
            }
            acceso =false;

        }
        else if(op == "L")
        {//Operacion de liberacion de memoria, ocupada por un proceso
            ArchEntrada >> nombreProceso;
            //cout<<"Libera proceso: "<<nombreProceso<<endl;
            //RAM.liberaProceso(string nombreProceso, vector paginasLiberadasEnSwap, vector paginasLiberadasEnMemoria);
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso)
                    {
                        procesosSesion[i].tiempoSalida = time(&tiempoaux);
                    }
            }


        }
        else if(op == "F")
        {//Fin de un secuencia de instrucciones, despliega un brief de lo realizado
            cout<<"RESUMEN DE INTRUCCIONES"<<endl;
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
            //cout<<"Total Swapp-in's: "<< RAM.getSwapin()<<endl;
            //cout<<"Total Swapp-out's: "<< RAM.getSwapout()<<endl;
        }
        else if(op == "E")
        {//Terminacion del programa
            cout<<"FIN DE PROGRAMA"<<endl;
        }
        else
        {//Mensaje de error al recibir un comando no registrado como valido
            cout<<"*Operacion invalida"<<endl;
            cout<<"Posibles comandos: \nP(iniciar un proceso), \nA(acessar un proceso, \nL(liberar un proceso reigstrado)"<<endl;
        }
    }

    ArchEntrada.close();


    return 0;
}
