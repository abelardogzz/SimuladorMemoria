#include <iostream>
#include <fstream>
#include <string>
using namespace std;
/**
 *Archivo prueba
P 2048 3
A 1 1 0
L 1
F
E
 */
#include "Disco.h"
int main()
{
    struct Proceso{
        string ProcesoID;

    };
    ifstream ArchEntrada;
    ArchEntrada.open("Instrucciones.txt");
    string operacion;
    string procesID,op;
    int bytes,dirVirtual;
    int pos,bitLecMod;

    //while (getline(ArchEntrada,operacion))//Lectura de archivo linea por linea
    while (ArchEntrada>>op)//Lectura de archivo linea por linea
    {
        operacion="ZONAAA #$ 1";
        pos = operacion.find(" ");
        //op= operacion.substr(0,pos).c_str();
        operacion.erase(0,pos);
        if(op == "P")
        {
            pos = operacion.find(" ");
            //bytes = (operacion.substr(0,pos));
            ArchEntrada>>bytes;
            ArchEntrada >> procesID;
            if(bytes>2048)
            {
                cout<<"Proceso: "<<procesID<<" No cabe en memoria"<<endl;

            }
            else
                cout<<"Tamaño de proceso: "<<bytes<<" ID "<<procesID<<endl;
            operacion.erase(0,pos);

        }
        else if(op == "A")
        {
            ArchEntrada>>dirVirtual;
            ArchEntrada >> procesID;
            ArchEntrada >> bitLecMod;
            cout<<"DirVirtual: "<<dirVirtual<<" ID: "<<procesID<<" Lectura/Modificacion: "<< bitLecMod<<endl;
        }
        else if(op == "L")
        {
            ArchEntrada >> procesID;
            cout<<"Libera proceso: "<<procesID<<endl;
        }
        else if(op == "F")
        {
            cout<<"RESUMEN DE INTRUCCIONES"<<endl;
            //Finaliza una secuencia de isnturcciones
        }
        else if(op == "E")
        {
            cout<<"FIN DE PROGRAMA, BYE."<<endl;
        }
        else
        {
            cout<<"*******Operacion invalida*******"<<endl;
        }
//        cout<<"HOLAMUDNO"<<endl;


    }
//    cout<<"HOLAMUDNO+++++"<<endl;

    ArchEntrada.close();


    return 0;
}
