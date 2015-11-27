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


bool ValidaOp(string comando)
{
    string operacion;;
    string auxLectura,aux;
    int numIteraciones = 0;
    string comandoEsperado;
    int blankSpaces = 0;
    bool malComando =false;
    stringstream sscomando;
    sscomando <<comando;

    while(sscomando >> operacion && !malComando){
            //cout<<"******OPERACION: "<<operacion<<endl;
            if(operacion == " ")
                blankSpaces++;
            else if (operacion == "P" && numIteraciones == 0)
                comandoEsperado = "P";
            else if (operacion == "A" && numIteraciones == 0)
                comandoEsperado = "A";
            else if (operacion == "L" && numIteraciones == 0)
                comandoEsperado = "L";
            else if (operacion == "F" && numIteraciones == 0)
                comandoEsperado = "F";
            else if (operacion == "E" && numIteraciones == 0)
                comandoEsperado = "E";
            /*else
                malComando = true;*/
        numIteraciones++;
        blankSpaces++;

    }

        if(comandoEsperado == "P" && blankSpaces== 3)
            return true;
        else if(comandoEsperado == "A" && blankSpaces == 4)
            return true;
        else if(comandoEsperado == "L" && blankSpaces == 2)
            return true;
        else if(comandoEsperado == "F" && blankSpaces == 1)
            return true;
        else if(comandoEsperado == "E" && blankSpaces == 1)
            return true;
        else if(comandoEsperado == "P" && blankSpaces != 3)
            {cout<<"ERROR EL COMANDO P TIENE DOS PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "A" && blankSpaces != 4)
            {cout<<"ERROR EL COMANDO A TIENE TRES PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "L" && blankSpaces != 2)
            {cout<<"ERROR EL COMANDO L TIENE UN PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "F" && blankSpaces != 1)
            {cout<<"ERROR EL COMANDO F TIENE DOS PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "E" && blankSpaces != 1)
            {cout<<"ERROR EL COMANDO P TIENE DOS PARAMETROS"<<endl; return false; }
        else
            {cout<<"ERROR con el comando: "<<comando<<endl; return false; }


}



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
    string nombreProceso,op,operacion;
    int bytes,dirVirtual;
    time_t tiempoaux,tiempo;
    int bitLecMod;
    double turnaroundTotal;//8====================================================================D
    bool acceso=false, accesarProceso=false;
    stringstream sscomando;

    //while(getline(ArchEntrada,operacion))
    //while (ArchEntrada>>op)//Lectura de archivo, la primer letra,
    while(getline(ArchEntrada,operacion))
    {//La primer letra determina cual es la operacion a realizar
        sscomando <<operacion;
        if(ValidaOp(operacion))
        {
            //cout<<"operacion AKA READLINE:  "<<operacion<<endl;
            //cout<<"LA VERDAD: Valida Operacion:   "<<ValidaOp(operacion)<<endl;
            sscomando >> op;
            //cout<<op<<endl;

        }
        else{
            op="ERROR";
            sscomando.str(" ");
        }

        if(op == "P")
        {//Operacion de iniciar un nuevo proceso
            sscomando>>bytes;//ArchEntrada>>bytes;
            //cout<<"VALOR DE BYTES "<<bytes<<endl;
            //cout<<"VALOR DE BYTES(VALIDADO) "<<isdigit(bytes)<<endl;
            if(bytes>0)//if(isdigit(bytes))
            {
                sscomando>> nombreProceso;//ArchEntrada >> nombreProceso;
                if(bytes>2048 || bytes <= 0)
                    cout<<"Proceso: "<<nombreProceso<<" No cabe en memoria"<<endl;
                else
                {
                    cout<<"COMANDO: "<<op<<" "<<bytes<<" "<<nombreProceso<<endl;
                    RAM.cargarProceso(bytes,nombreProceso,paginasSwappeadas);
                    //Agregacion al vector de procesos
                    Proceso pro;
                    pro.nombreProceso = nombreProceso;
                    localtime(&(pro.tiempollegada));
                    localtime(&tiempo);
                    //cout<<"TIEMPOOOO "<<ctime(&tiempo);
                    //cout<<"TIEMPO AUX "<<ctime(&(pro.tiempollegada))<<endl;
                    //pro.tiempollegada = localtime(&tiempoaux);
                    procesosSesion.push_back(pro);
                }
            }
            else
            {
                cout<<"COMANDO NO VALIDO: "<<operacion<<endl;
                sscomando.str(" ");
            }

        }
        else if(op == "A")
        {//Operacion de acceso a un nuevo proceso
            sscomando >> dirVirtual;//ArchEntrada>>dirVirtual; //direccion virtual a accesar
            //cout<<"DIRECCION VIRTUAL:   "<<dirVirtual<<endl;
            if(dirVirtual>0)//if(isdigit(dirVirtual))
            {
                sscomando >> nombreProceso;//ArchEntrada >> nombreProceso; //nombre del proceso que se quiere accesar
                sscomando >> bitLecMod;//ArchEntrada >> bitLecMod; //modo en que se quiere accesar
                cout<<"COMANDO: "<<op<<" "<<dirVirtual<<" "<<nombreProceso<<" "<< bitLecMod<<endl;
                int pos;
                for(int i=0; i< procesosSesion.size(); i++)
                {
                    if(procesosSesion[i].nombreProceso == nombreProceso){
                        //Si el proceso coincide con el solicitado
                        if(procesosSesion[i].tamano <= dirVirtual){
                            //Si esl tamaño del proceso es mayor a la direccion virtual
                            acceso =true;//Si se acceso =TRUE
                            accesarProceso = RAM.accesarProceso(dirVirtual,nombreProceso,Pag1,Pag2);
                            //Accesar, Lectura o modificacion, numero de paginas de swap
                            pos = i;//Guarda posicion, para asignar despues PageFaults, si hubó
                            break;
                        }
                        else//
                        cout<<"La direccion de memoria no se puede accesar (PAGE OVERFLOW)"<<endl;
                    }
                }
                if(!acceso){
                    //Si no se logro el acceso
                    cout<<"Ese proceso no ha sido declarado"<<endl;
                }
                else
                {//Si se acceso y ocurrio un pageFault
                    if(accesarProceso)
                        procesosSesion[pos].numPageFaults +=1;
                }
            }
            else
            {
                cout<<"COMANDO NO VALIDO: "<<operacion<<endl;
                sscomando.str(" ");
            }
        }
        else if(op == "L")
        {//Operacion de liberacion de memoria, ocupada por un proceso
            acceso =false;
            sscomando >> nombreProceso;//ArchEntrada >> nombreProceso;
            cout<<"COMANDO: "<<op<<" "<<nombreProceso<<endl;
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso)
                    {
                        RAM.liberarProceso(nombreProceso,paginasLiberadasEnSwap,paginasLiberadasEnMemoria);
                        localtime(&tiempoaux);
                        procesosSesion[i].tiempoSalida = tiempoaux;
                        //cout<<"TIEMPO AUX "<<ctime(&tiempoaux)<<endl;
                        localtime(&tiempo);
                        //cout<<"TIEMPOOOO "<<ctime(&tiempo);
                        acceso =true;
                        break;
                    }
            }
            if(!acceso){
                cout<<"Proceso: "<< nombreProceso <<", no se ha declarado"<<endl;
            }
        }
        else if(op == "F")
        {//Fin de un secuencia de instrucciones, despliega un brief de lo realizado
            cout<<op<<" RESUMEN DE INTRUCCIONES"<<endl;
            cout<<"******************************"<<endl;
            double contProcesosTerminados=0;
            double turnaroundProceso;
            //Finaliza una secuencia de isnturcciones
            for(int i=0; i < procesosSesion.size();i++)
            {cout<<"PROCESO "<<i<<endl;
            cout<<procesosSesion[i].tiempoSalida<<endl;
                if(procesosSesion[i].tiempoSalida != NULL)
                {
                    contProcesosTerminados++;

                    //cout<<ctime(&(procesosSesion[i].tiempoSalida))<<" TIEMPOS "<<ctime(&(procesosSesion[i].tiempollegada))<<endl;

                    turnaroundProceso = difftime(procesosSesion[i].tiempoSalida,procesosSesion[i].tiempollegada);
                    //cout<<"TIME DIFF "<< turnaroundProceso<<endl;

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
            cout<<op<<" FIN DE PROGRAMA"<<endl;
        }
        else if(op == "ERROR")
        {//Mensaje de error al recibir un COMANDO no registrado como valido
            cout<<"*Operacion invalida \" "<<operacion<<"\""<<endl;
        }
    }

    ArchEntrada.close();


    return 0;
}
