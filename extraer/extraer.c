#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "extraer.h"

int main (int argc, char *argv[]) {
    char *errmsg;
    int index, n;

    if (argc != 3) {
        sprintf(errmsg, "El número de argumentos introducidos (%d) no es valido", argc);
        E_DESCO(errmsg);
    } else {
	index = atoi(argv[2]);
        n = extrae_fichero(argv[1],  index);
	if (n == 0) {
//	    printf("¡EXTRACCIÓN COMPLETADA!\n");
	} else if (n == 1) {
	    E_POS("Error 1: Fallo  al introducir el índice del archivo.\n");
	} else if (n == 2) {
	    E_OPEN("Error 2: Fallo en la apertura del fichero '.myzip'.\n");
	} else if (n == 3) {
            E_DIR1("Error 3: Fallo al crear el directorio.\n");
        } else if (n == 2) {
            E_DIR2("Error 4: Fallo en la apertura del directorio.\n");
        }else if (n == 99) {
	    E_DESCO("Error 99: Error desconocido.\n");
	}
    }

    return(0);
}
