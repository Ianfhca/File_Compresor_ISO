
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "insertar.h"

int main (int argc, char *argv[]) {
    char *errmsg;
    int index, n;

    if (argc != 4) {
        sprintf(errmsg, "El número de argumentos introducidos (%d) no es valido", argc);
        E_DESCO(errmsg);
    } else {
	index = atoi(argv[3]);
        n = inserta_fichero(argv[1], argv[2], index);
	if (n == 0) {
	    return(index);
	} else if (n == -1) {
	    E_OPEN1("Error -1: Fallo al abrir el fichero 'f_dat'.\n");
	} else if (n == -2) {
	    E_OPEN2("Error -2: Fallo en la apertura del fichero 'f_mypackzip'\n");
	} else if (n == -3) {
	    E_POS1("Error -3: Fallo al introducir el indice del archivo.\n");
	} else if (n == -4) {
            E_POS2("Error -4: El índice introducido hace referencia a una cabecera que ya contiene datos validos.\n");
        } else if (n == -5) {
            E_OPENDIR1("Error -5: Fallo en la apertura del directorio.\n");
        } else if (n == -99) {
            E_DESCO("Error -99: Error desconocido.\n");
        }
    }

    return(-1);
}
