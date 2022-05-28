#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <ftw.h>

#include "s_header.h"
#include "extraer.h"

extrae_fichero(char * f_mypackzip, int index) {
//
    int fdo, fdd;
    struct s_mypack_headers headers;
//
    char buf[BLOCK_SIZE];
    int file, position, blocks, size, Ret;
    int i = 0;
//
    char fileName[MAX_FILE_NAME], part[MAX_FILE_NAME];
    char *token, *aux;
//directorios
    DIR *fd;
    int dfd = dirfd(fd);
    struct dirent *f_dat;

//COMPROBAR ÍNDICE DE LA CABECERA
    if (index < 0 || index > (NUM_HEADERS-1)) {
        return(1);
    }

//ABRIR FICHERO LECTURA
    fdo = open(f_mypackzip, O_RDONLY);
    if (fdo < 0) {
        return(2);
    }

//SELECCIONAR CABECERA
    file = index * S_HEADER_SIZE;
    lseek(fdo, file, SEEK_SET);

//LEER CABECERA
    read (fdo, &(headers.vHead[index].FileInfo), S_INFO_SIZE);
    if (headers.vHead[index].FileInfo.Type == '\\') {
	return(1);
    }

//ABRIR FICHERO ESCRITURA
    switch(headers.vHead[index].FileInfo.FileType) {
	case 'd':
	    strcpy(fileName, headers.vHead[index].FileInfo.DataFileName);
            token = strtok(fileName, "/");
	    strcpy(part, token);
            while (token !=  NULL) {
		if (fd = opendir(part)){
                    closedir(fd);
                } else {
                    mkdir(part, S_IRWXU|S_IRWXG|S_IRWXO);
		    chmod(part, headers.vHead[index].FileInfo.Permissions);
                }

                token = strtok(NULL, "/");
		if (token != NULL) {
		    strcat(strcat(part, "/"), token);
		}
            }
	break;

	case 'r':
	    strcpy(fileName, headers.vHead[index].FileInfo.DataFileName);
            token = strtok(fileName, "/");
            i = (token != NULL) ? 1 : 0;
            strcpy(part, token);
            while (token !=  NULL) {
                if (fd = opendir(part)){
                    closedir(fd);
                } else {
                    mkdir(part, S_IRWXU|S_IRWXG|S_IROTH);
                }
                token = strtok(NULL, "/");
                if (token != NULL) {
                    strcat(strcat(part, "/"), token);
                }
            }
            if (i == 1) {
                remove(part);
            }
            fdd = open(headers.vHead[index].FileInfo.DataFileName, O_WRONLY|O_CREAT|O_TRUNC, 0600);
            if (fdd < 0) {
                return(99);
            }
	    chmod(headers.vHead[index].FileInfo.DataFileName, headers.vHead[index].FileInfo.Permissions);

//ESCRIBIR DATOS DEL NUEVO FICHERO
    	    lseek(fdo, headers.vHead[index].FileInfo.DatPosition, SEEK_SET);
    	    blocks = headers.vHead[index].FileInfo.NumBlocks;
    	    size = headers.vHead[index].FileInfo.DataSize;
    	    while (blocks > 0) {
        	if (size >= BLOCK_SIZE) {
            	    read(fdo, buf, BLOCK_SIZE);
            	    write(fdd, buf, BLOCK_SIZE);
		} else {
	    	    read(fdo, buf, size);
	    	    write(fdd, buf, size);
		}
        	blocks--;
        	size = size-512;
    	    }

//CERRAR FICHEROS
    	    close(fdd);
	break;

	case 'o':
	    strcpy(fileName, headers.vHead[index].FileInfo.DataFileName);
            token = strtok(fileName, "/");
            i = (token != NULL) ? 1 : 0;
            strcpy(part, token);
            while (token !=  NULL) {
                if (fd = opendir(part)){
                    closedir(fd);
                } else {
                    mkdir(part, S_IRWXU|S_IRWXG|S_IROTH);
                }
                token = strtok(NULL, "/");
                if (token != NULL) {
                    strcat(strcat(part, "/"), token);
                }
            }
            if (i == 1) {
                remove(part);
            }
	    lseek(fdo, headers.vHead[index].FileInfo.DatPosition, SEEK_SET);
	    if ((Ret = read(fdo, buf, BLOCK_SIZE)) != 0){
		symlink(buf, headers.vHead[index].FileInfo.DataFileName);
	    }
        break;
    }

    close(fdo);
    return(0);
}
