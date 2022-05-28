#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#include "s_header.h"
#include "insertar.h"

//ficheros
    int fdo, fdd;
    struct s_mypack_headers headers;
//directorios
    DIR *fd;
    struct dirent * d_file;
    int empty = 0;
    char path[MAX_FILE_NAME];
//permisos
//variables del sistema
    char buf[BLOCK_SIZE];
    int file;
    ino_t pid1, pid2;
//insertar cabecera
    int Ret;
    struct stat StVar;
    int fileSize;
    mode_t fileType;
    char header[S_HEADER_SIZE];
//insertar datos
    int odataSize, ddataSize;
//variables auxiliares
    int i = 0;

//-------FUNCIÓN INICIAL-------
inserta_fichero(char * f_mypackzip, char * f_dat, int index) {

//COMPROBAR EL INDICE DE LA CABECERA
    if (index < 0 || index > (NUM_HEADERS-1)) {
        return(-3);
    }

//COMPROBAR E INSERTAR EL TIPO DE FICHERO
    Ret = lstat(f_dat, &StVar);
    if (Ret < 0 ){
    return(-1);
    } else {

        fileType = StVar.st_mode;

	if (S_ISREG(fileType)) {
	    insertar_freg(f_mypackzip, f_dat, index);
	} else if (S_ISDIR(fileType)) {
	    inserta_fdir(f_mypackzip, f_dat, index);
	} else {
	    if (S_ISLNK(fileType)){
                inserta_fenl(f_mypackzip, f_dat, index);
	    }
	}
    }
}


//-------FUNCIÓN PARA INSERTAR FICHERO REGULAR-------
insertar_freg(char * f_mypackzip, char * f_dat, int index) {

    fdo = open(f_dat, O_RDONLY);
    if (fdo < 0) {
        return(-1);
    }

//ABRIR FICHERO DESTINO (DAR FORMATO A LAS CABECERAS)
    if (access(f_mypackzip, F_OK) == 0){

	stat(f_mypackzip, &StVar);
	pid1 = StVar.st_ino;
	stat(f_dat, &StVar);
	pid2 =StVar.st_ino;
	if (pid1 == pid2) {
            return(-99);
	}

	fdd = open(f_mypackzip, O_RDWR, 0600);
        if (fdd < 0) {
            return(-2);
    	}
    } else {
        fdd = open(f_mypackzip, O_RDWR|O_CREAT, 0600);
        if (fdd < 0) {
            return(-2);
        }
	bzero(&headers, S_HEADER_SIZE*NUM_HEADERS);
        for (i=0; i < NUM_HEADERS;  i++) {
            headers.vHead[i].FileInfo.Type = '\\';
        }
	 write(fdd, &(headers), sizeof(headers));
    }

    read (fdd, &headers, sizeof(headers));
    if(headers.vHead[index].FileInfo.Type == 'b') {
	return(-4);
    }

//ESCRITURA DE CABECERA
    file = index * S_HEADER_SIZE;
    lseek(fdd, file, SEEK_SET);
    Ret = stat(f_dat, &StVar);
    if (Ret < 0 ){
    return(-99);
    } else {
	fileSize = StVar.st_size;
	fileType = StVar.st_mode;

        headers.vHead[index].FileInfo.Type = 'b';
        headers.vHead[index].FileInfo.Compress = 'n';
        headers.vHead[index].FileInfo.DataSize = fileSize;
        headers.vHead[index].FileInfo.CompSize = fileSize;
        headers.vHead[index].FileInfo.NumBlocks = (fileSize%BLOCK_SIZE == 0) ? fileSize/BLOCK_SIZE : (fileSize/BLOCK_SIZE)+1;
        strcpy(headers.vHead[index].FileInfo.DataFileName, f_dat);

        Ret = stat(f_mypackzip, &StVar);
        fileSize = StVar.st_size;
        headers.vHead[index].FileInfo.DatPosition = fileSize;
        headers.vHead[index].FileInfo.FileType = 'r';
	headers.vHead[index].FileInfo.Permissions = fileType;

        write(fdd, &headers.vHead[index].FileInfo, S_INFO_SIZE);
    }
//ESCRITURA DE DATOS
    lseek(fdd, headers.vHead[index].FileInfo.DatPosition, SEEK_SET);

    while ((odataSize = read(fdo, buf, BLOCK_SIZE)) > 0) {
        ddataSize = write(fdd, buf, BLOCK_SIZE);
	memset(buf, 0, BLOCK_SIZE);
    }

    close(fdo);
    close(fdd);
    return(0);
}

//-------FUNCIÓN PARA INSERTAR DIRECTORIO-------
inserta_fdir(char * f_mypackzip, char * f_dat, int index) {

//ABRIR FICHERO ORIGEN
    fd = opendir(f_dat);
    if (fd == NULL) {
	return(-5);
    }

//ABRIR FICHERO DESTINO
    if (access(f_mypackzip, F_OK) == 0){
        fdd = open(f_mypackzip, O_RDWR, 0600);
        if (fdd < 0) {
            return(-2);
        }
    } else {
        fdd = open(f_mypackzip, O_RDWR|O_CREAT, 0600);
        if (fdd < 0) {
            return(-2);
        }
	bzero(&headers, S_HEADER_SIZE*NUM_HEADERS);
        for (i=0; i < NUM_HEADERS;  i++) {
            headers.vHead[i].FileInfo.Type = '\\';
        }
         write(fdd, &(headers), sizeof(headers));
    }
    read (fdd, &headers, sizeof(headers));
    if(headers.vHead[index].FileInfo.Type == 'b') {
        return(-4);
    }

//ESCRITURA DE CABECERA
    file = index * S_HEADER_SIZE;
    lseek(fdd, file, SEEK_SET);

    headers.vHead[index].FileInfo.Type = 'b';
    headers.vHead[index].FileInfo.Compress = 'n';
    headers.vHead[index].FileInfo.DataSize = 0;
    headers.vHead[index].FileInfo.CompSize = 0;
    headers.vHead[index].FileInfo.NumBlocks = 0;
    strcpy(headers.vHead[index].FileInfo.DataFileName, f_dat);
    headers.vHead[index].FileInfo.DatPosition = 0;
    headers.vHead[index].FileInfo.FileType = 'd';
    headers.vHead[index].FileInfo.Permissions = fileType;

    write(fdd, &headers.vHead[index].FileInfo, S_INFO_SIZE);

//LECTURA DE LOS ARCHIVOS DEL DIRECTORIO
    i = 0;
    lseek(fdd, 0, SEEK_SET);

     while((d_file = readdir(fd)) != NULL) {
	sprintf(path, "%s/%s", f_dat, d_file->d_name);   // imprimir el normbre del fichero.
	Ret = stat(path, &StVar);
	fileType = StVar.st_mode;
        if (S_ISREG(fileType) > 0) {
            read(fdd, &headers, sizeof(headers));
            empty = 0;
            while(empty == 0 && i < NUM_HEADERS){
                if(headers.vHead[i].FileInfo.Type == '\\') {
                    empty = 1;
                    insertar_freg(f_mypackzip, path, i);
                }
                i++;
            }
            if (i >= NUM_HEADERS) {
                return(-99);
            }
	}
    }



    closedir(fd);
    close(fdd);
    return(0);
}



//-------FUNCIÓN PARA ENLACE SIMBOLICO-------
inserta_fenl(char * f_mypackzip, char * f_dat, int index) {

    if (access(f_mypackzip, F_OK) == 0){

	fdd = open(f_mypackzip, O_RDWR, 0600);
        if (fdd < 0) {
            return(-2);
        }
    } else {
	fdd = open(f_mypackzip, O_RDWR|O_CREAT, 0600);
        if (fdd < 0) {
            return(-2);
        }
	bzero(&headers, S_HEADER_SIZE*NUM_HEADERS);
        for (i=0; i < NUM_HEADERS;  i++) {
            headers.vHead[i].FileInfo.Type = '\\';
        }
         write(fdd, &(headers), sizeof(headers));
    }


    read (fdd, &headers, sizeof(headers));
    if(headers.vHead[index].FileInfo.Type == 'b') {
        return(-4);
    }

//ESCRITURA DE CABECERA
    file = index * S_HEADER_SIZE;
    lseek(fdd, file, SEEK_SET);

    headers.vHead[index].FileInfo.Type = 'b';
    headers.vHead[index].FileInfo.Compress = 'n';
    headers.vHead[index].FileInfo.DataSize = BLOCK_SIZE;
    headers.vHead[index].FileInfo.CompSize = BLOCK_SIZE;
    headers.vHead[index].FileInfo.NumBlocks = 1;
    strcpy(headers.vHead[index].FileInfo.DataFileName, f_dat);

    Ret = stat(f_mypackzip, &StVar);
    fileSize = StVar.st_size;
    headers.vHead[index].FileInfo.DatPosition = fileSize;
    headers.vHead[index].FileInfo.FileType = 'o';
    headers.vHead[index].FileInfo.Permissions = fileType;

    write(fdd, &headers.vHead[index].FileInfo, S_INFO_SIZE);

//ESCRITURA DE DATOS
    lseek(fdd, headers.vHead[index].FileInfo.DatPosition, SEEK_SET);
    realpath(f_dat, buf );
    write(fdd, buf, BLOCK_SIZE);
    memset(buf, 0, BLOCK_SIZE);


    close(fdo);
    close(fdd);
    return(0);

}
