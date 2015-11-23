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
*Estructura Espacio que define los espacios de memoria de la tabla de paginas
*
*/

struct Espacio
{
	struct Pagina pagina;
	bool estaVacio = true;
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
