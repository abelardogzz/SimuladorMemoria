#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include <stdio.h>
using namespace std;
#include "Estructuras.h"
#include "Memoria.h"
#include "Disco.h"


/**
    Valida la estructura de un comando

    @comando el comando completo que se recibio de la linea

    @return bool TRUE si cuenta con la estructura de un comando correcto, FALSE no es un comando incorrecto
  */
bool ValidaOp(string comando)
{
    string operacion;
    int numIteraciones = 0;
    string comandoEsperado;
    int blankSpaces = 0;
    bool malComando =false;
    stringstream sscomando;//Variable para crear un stream de datos
    sscomando <<comando; //Se vacia el comando recibido en el stream

    while(sscomando >> operacion && !malComando){//Mientras tenga datos en el stream
            //Se leen uno a uno, tomando un conteo de los argumentos mandados
            if(operacion == " ")
                blankSpaces++;
            else if (operacion == "P" && numIteraciones == 0) //Si es una P se espera cargar un proceso
                comandoEsperado = "P";
            else if (operacion == "A" && numIteraciones == 0) //Si es una A se espera accesar un proceso
                comandoEsperado = "A";
            else if (operacion == "L" && numIteraciones == 0) //Si es una L se espera liberar un proceso
                comandoEsperado = "L";
            else if (operacion == "F" && numIteraciones == 0) //Si es una F se espera resumen de sesion
                comandoEsperado = "F";
            else if (operacion == "E" && numIteraciones == 0) //Si es una E se espera terminar programa
                comandoEsperado = "E";

        numIteraciones++;
        blankSpaces++;

    }
        //Condicion que decide si fue correcto o no  la cantidad de argumentos, contando la opcion
        if(comandoEsperado == "P" && blankSpaces== 3)//Si se espera una P, tiene 3 argumentos
            return true;
        else if(comandoEsperado == "A" && blankSpaces == 4)//Si se espera una A tiene 4 argumentos
            return true;
        else if(comandoEsperado == "L" && blankSpaces == 2)//Si se espera una L tiene 2 argumentos
            return true;
        else if(comandoEsperado == "F" && blankSpaces == 1)//Si se espera una F tiene 1 argumento
            return true;
        else if(comandoEsperado == "E" && blankSpaces == 1)//Si se espera una E tiene 1 argumento
            return true;
        else if(comandoEsperado == "P" && blankSpaces != 3)             //En caso contrario, se regresa un mensaje
            {cout<<"ERROR EL COMANDO P TIENE DOS PARAMETROS"<<endl; return false; }//notificando la forma correcta
        else if(comandoEsperado == "A" && blankSpaces != 4)
            {cout<<"ERROR EL COMANDO A TIENE TRES PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "L" && blankSpaces != 2)
            {cout<<"ERROR EL COMANDO L TIENE UN PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "F" && blankSpaces != 1)
            {cout<<"ERROR EL COMANDO F TIENE DOS PARAMETROS"<<endl; return false; }
        else if(comandoEsperado == "E" && blankSpaces != 1)
            {cout<<"ERROR EL COMANDO P TIENE DOS PARAMETROS"<<endl; return false; }
        else
            {/*cout<<"ERROR con el comando: "<<comando<<endl;*/ return false; }//Si no es ninguna, solo regresa false
            //El resto del programa se administra en main()
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
    long double turnaroundTotal;
    bool acceso=false, accesarProceso=false, repetido =false;
    stringstream sscomando;


    //while(getline(ArchEntrada,operacion))
    //while (ArchEntrada>>op)//Lectura de archivo, la primer letra,
    while(getline(ArchEntrada,operacion))
    {//La primer letra determina cual es la operacion a realizar
        sscomando <<operacion;
        if(ValidaOp(operacion))
        {
            sscomando >> op;
        }
        else{
            op="ERROR";
            sscomando.str("");
            sscomando.clear();
        }

        if(op == "P")
        {//Operacion de iniciar un nuevo proceso
            sscomando>>bytes;//ArchEntrada>>bytes;
            if(bytes>0)//if(isdigit(bytes))
            {
                repetido = false;
                sscomando>> nombreProceso;//ArchEntrada >> nombreProceso;
                for(int i=0;i<procesosSesion.size();i++)
                {
                    if(procesosSesion[i].nombreProceso == nombreProceso)
                        repetido = true;
                }
                if(bytes>2048 || bytes <= 0)
                    cout<<endl<<"Proceso: "<<nombreProceso<<" No cabe en memoria"<<endl;
                else if(!repetido)
                {
                    cout<<endl<<"COMANDO: "<<op<<" "<<bytes<<" "<<nombreProceso<<endl;
                    RAM.cargarProceso(bytes,nombreProceso,paginasSwappeadas);
                    //Agregacion al vector de procesos
                    Proceso pro;
                    pro.nombreProceso = nombreProceso;
                    pro.tiempollegada = clock();
                    procesosSesion.push_back(pro);
                }
                else
                    cout<<endl<<"COMANDO NO VALIDO (REPETIDO):"<< operacion<<endl;
                    sscomando.str("");
                    sscomando.clear();
            }
            else
            {
                cout<<endl<<"COMANDO NO VALIDO: "<<operacion<<endl;
                sscomando.str("");
                sscomando.clear();
            }

        }
        else if(op == "A")
        {//Operacion de acceso a un nuevo proceso
            acceso =false;
            sscomando >> dirVirtual;//ArchEntrada>>dirVirtual; //direccion virtual a accesar
            if(dirVirtual>0)//if(isdigit(dirVirtual))
            {
                sscomando >> nombreProceso;//ArchEntrada >> nombreProceso; //nombre del proceso que se quiere accesar
                sscomando >> bitLecMod;//ArchEntrada >> bitLecMod; //modo en que se quiere accesar
                cout<<endl<<"COMANDO: "<<op<<" "<<dirVirtual<<" "<<nombreProceso<<" "<< bitLecMod<<endl;
                int pos;
                for(int i=0; i< procesosSesion.size(); i++)
                {
                    if(procesosSesion[i].nombreProceso == nombreProceso){
                        //Si el proceso coincide con el solicitado
                        if(procesosSesion[i].tamano <= dirVirtual && procesosSesion[i].tamano>=0){
                            //Si esl tama�o del proceso es mayor a la direccion virtual
                            acceso =true;//Si se acceso =TRUE
                            accesarProceso = RAM.accesarProceso(dirVirtual,nombreProceso,Pag1,Pag2);
                            //Accesar, Lectura o modificacion, numero de paginas de swap
                            pos = i;//Guarda posicion, para asignar despues PageFaults, si hub�
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
            {//Si la direccion Virtual no
                cout<<"COMANDO NO VALIDO: "<<operacion<<endl;
                sscomando.str("");
                sscomando.clear();
            }

        }
        else if(op == "L")
        {//Operacion de liberacion de memoria, ocupada por un proceso
            acceso =false;
            sscomando >> nombreProceso;//ArchEntrada >> nombreProceso;
            cout<<endl<<"COMANDO: "<<op<<" "<<nombreProceso<<endl;
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].nombreProceso == nombreProceso && procesosSesion[i].tiempoSalida == NULL)
                    {
                        RAM.liberarProceso(nombreProceso,paginasLiberadasEnSwap,paginasLiberadasEnMemoria);
                        procesosSesion[i].tiempoSalida = clock();
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
            cout<<endl<<op<<" RESUMEN DE INTRUCCIONES"<<endl;
            cout<<"******************************"<<endl;
            double contProcesosTerminados=0; //Contador de procesos liberados, para Turnaround Promedio
            long double turnaroundProceso; //turnaround de cada proceso
            //Finaliza una secuencia de isnturcciones
            for(int i=0; i < procesosSesion.size();i++)
            {
                if(procesosSesion[i].tiempoSalida != NULL)
                {//Si ya se habia liberado(tiene tiempoSalida)
                    contProcesosTerminados++;
                    turnaroundProceso = difftime(procesosSesion[i].tiempoSalida,procesosSesion[i].tiempollegada);

                    cout<<"Turnaround: "<< ((turnaroundProceso)) << " segundos ";
                    cout<<" Proceso: "<< procesosSesion[i].nombreProceso;
                    cout<<" PageFaults: "<<procesosSesion[i].numPageFaults<<endl;
                    turnaroundTotal += turnaroundProceso;
                }
                else
                {//Si no se ha liberado( NO tiene tiempoSalida)
                    //Se le fija un tiempoSalida
                    procesosSesion[i].tiempoSalida = clock();
                    nombreProceso = procesosSesion[i].nombreProceso;
                    //Se manda liberar
                    RAM.liberarProceso(nombreProceso,paginasLiberadasEnSwap,paginasLiberadasEnMemoria);
                    //Se toma en cuenta en el resumen de sesion
                    contProcesosTerminados++;
                    turnaroundProceso = difftime(procesosSesion[i].tiempoSalida,procesosSesion[i].tiempollegada);
                    cout<<"Turnaround: "<< ((turnaroundProceso)) << " segundos ";
                    cout<<" Proceso: "<< procesosSesion[i].nombreProceso;
                    cout<<" PageFaults: "<<procesosSesion[i].numPageFaults<<endl;
                    turnaroundTotal += turnaroundProceso;
                }
            }
            //Limpia el vector de proceso que se habian definido
            procesosSesion.clear();
            //Resto de datos de la sesion
            if(contProcesosTerminados != 0)
            {//Si al menos hubo proceso
                cout<<"Turnaround Promedio: "<<(turnaroundTotal / contProcesosTerminados)<<endl;
                cout<<"Total Swapp-in's: "<< RAM.getTotalSwapIns()<<endl;
                cout<<"Total Swapp-out's: "<< RAM.getTotalSwapOuts()<<endl;
                RAM.resetTotalSwapIns();
                RAM.resetTotalSwapOuts();
                cout<<"******************************"<<endl;
            }
            else
            {
                cout<<"NO SE CARGO NINGUN PROCESO"<<endl;
                cout<<"******************************"<<endl;
            }

        }
        else if(op == "E")
        {//Terminacion del programa
            cout<<endl<<op<<" FIN DE PROGRAMA"<<endl;
        }
        else if(op == "ERROR")
        {//Mensaje de error al recibir un COMANDO no registrado como valido
            cout<<endl<<" Operacion invalida: "<<operacion<<" "<<endl;
            sscomando.str("");
            sscomando.clear();
        }


    }

    ArchEntrada.close();


    return 0;
}
