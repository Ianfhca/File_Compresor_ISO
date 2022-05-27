# CompresorISO
Aplicación para comprimir y descomprimir ficheros en linux

La aplicación, desarrollada en C, fue creada para la asignatura de Introducción a los Sistemas Operativos y pensada para ser utilizada en cualquier sistema linux gracias a las diversas llamadas al sistema implementadas.

El compresor está compuesto por dos funcionalidades, inserta_fichero la que es la encargada de comprimir los archivos y extrae_fichero que los descomprime.

Para comprimir y descomprimir archivos, se hace uso de una estructura de datos formada por una lista de 30 cabeceras donde se almacena la información descriptiva de los ficheros y una sección donde se guardan los datos de dichos ficheros. En cada cabecera está guardado, el nombre del fichero, tipo, tamaño, permisos... Y una posición para indicar en que parte del archivo se almacenan los datos del mismo.

En el momento actual, la aplicación trabaja con ficheros regulares, directorios y enlaces simbólicos.

Para más información acerca de inserta_fichero y extrae_fichero mirar la especificación de cada uno de ellos.

Desarrollado por Ian Fernandez e Iker Goñi
