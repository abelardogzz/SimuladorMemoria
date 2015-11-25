#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
using namespace std;
#include "Estructuras.h"
#include "Memoria.h"



int main()
{
    //Vectores de control de procesos
    vector <struct Pagina> paginasSwappeadas; //Contador de cantidad de Page Faults
    vector <struct Proceso> procesosSesion;
    vector <struct Pagina> paginasLiberadasEnSwap, paginasLiberadasEnMemoria;

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
                cout<<op<<" proceso: "<<bytes<<" ID: "<<nombreProceso<<endl;
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
            ArchEntrada>>dirVirtual; //direccion virtual a accesar
            ArchEntrada >> nombreProceso; //nombre del proceso que se quiere accesar
            ArchEntrada >> bitLecMod; //modo en que se quiere accesar
            cout<<op<<" DirVirtual: "<<dirVirtual<<" ID: "<<nombreProceso<<" Lectura/Modificacion: "<< bitLecMod<<endl;
            for(int i=0; i< procesosSesion.size(); i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso){
                    if(procesosSesion[i].tamano <= dirVirtual){
                        //acceso = RAM.accesoProceso(int dirVirtual, string nombreProceso, int bitLecMod);
                    }
                    else
                    cout<<"La direccion de memoria no se puede accesar (PAGE OVERFLOW)"<<endl;
                }
            }

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
            cout<<op<<" Libera proceso: "<<nombreProceso<<endl;
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
            cout<<op<<"\nRESUMEN DE INTRUCCIONES"<<endl;
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
            cout<<op<<"\nFIN DE PROGRAMA"<<endl;
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
