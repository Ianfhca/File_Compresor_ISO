# ARCHIVO DE VERIFICACIÓN
# -----------------------

# Generar el entorno de trabajo para las pruebas de verificación
# (Ficheros Regulares, Directorios y Enlaces Simbólicos)
BLUE='\033[1;34m' #Azul Claro
RED='\e[0;31m' #Rojo
YELLOW='\e[0;33m' #Amarillo
WHITE='\033[0m' # Sin Color
printf "${BLUE}Creando entorno de pruebas...${WHITE}\n"
sleep 0.5
if [ -d "pruebas/" ]; then
    rm -r pruebas/
fi
mkdir pruebas
echo "Brasil

Brasil, oficialmente República Federativa de Brasil (en portugués, República Federativa do Brasil, pron. AFI [ʁe'publikɐ fedeɾa'tʃivɐ 'dʊ braˈziw] ( escuchar)), es un país soberano de América del Sur que comprende la mitad oriental del continente y algunos grupos de pequeñas islas en el océano Atlántico. Es el país más grande de América Latina. Con una superficie estimada en más de 8,5 millones de km², es el quinto país más grande del mundo en área total (equivalente a 47 % del territorio sudamericano). Delimitado por el océano Atlántico al este, Brasil tiene una línea costera de 7491 km. Al norte limita con el departamento ultramarino francés de la Guayana Francesa, Surinam, Guyana y Venezuela; al noroeste con Colombia; al oeste con Perú y Bolivia; al suroeste con Paraguay y Argentina, y al sur con Uruguay. De este modo tiene frontera con todos los países de América del Sur, excepto Ecuador y Chile. La mayor parte del país está comprendido entre los trópicos terrestres, por lo que las estaciones climáticas no se sienten de una manera radical en gran parte de su territorio. La selva amazónica cubre 3,6 millones de km² del territorio. Gracias a su vegetación y al clima, es uno de los países con más especies de animales en el mundo." > pruebas/datos.dat
mkdir pruebas/dir1
cp pruebas/datos.dat pruebas/dir1
echo "Un sistema operativo es el conjunto de programas de un sistema informático que gestiona los recursos de hardware y provee servicios a los programas de aplicación de software. Estos programas se ejecutan en modo privilegiado respecto de los restantes." > pruebas/dir1/datos2.dat
cd pruebas/
ln -s dir1/datos2.dat link
cd ../
tree pruebas/
sleep 0.5

printf "\n${BLUE}Clonando directorio de pruebas a 'original/'...${WHITE}\n"
sleep 0.5
if [ -d "original/" ]; then
    rm -r original/
fi
cp -r pruebas/ original/
tree original/
sleep 0.5

# Creando directorio sin ningun permiso (Pruebas)
printf "\n${YELLOW}Creando directorio con los permisos restringidos 'noperm_dir/'...${WHITE}\n"
if [ -d "noperm_dir/" ]; then
    chmod ugo+rxw noperm_dir
    rm -r noperm_dir/
fi
mkdir noperm_dir
chmod ugo-rxw noperm_dir
sleep 0.5

# -- INSERCIÓN -- #

# Crear e Insertar el fichero de pruebas/datos.dat en fich.myzip
printf "\n${BLUE}Creando e insertando el fichero de 'pruebas/datos.dat' en fich.myzip...${WHITE}\n"
sleep 0.5
if [ -f "fich.myzip" ]; then
    rm fich.myzip
fi
./inserta_fichero fich.myzip pruebas/datos.dat 5
hexdump -C fich.myzip # Verificar fich.myzip
sleep 0.5

# Insertar en fich.myzip pruebas/dir1/ y todo su contenido
printf "\n${BLUE}Insertando en fich.myzip el directorio 'pruebas/dir1/' y todo su contenido...${WHITE}\n"
sleep 0.5
./inserta_fichero fich.myzip pruebas/dir1/ 2
hexdump -C fich.myzip # Verificar fich.myzip
sleep 0.5

# Insertar en fich.myzip el enlace pruebas/link
printf "\n${BLUE}Insertando en fich.myzip el enlace 'pruebas/link'...${WHITE}\n"
sleep 0.5
./inserta_fichero fich.myzip pruebas/link 10
hexdump -C fich.myzip # Verificar fich.myzip
sleep 0.5

# -- ERRORES --#

printf "\n${BLUE}Tratamiento de errores:${WHITE}\n"

# Insertar en fich.myzip el fichero pruebas/fichero - APERTURA DATOS
printf "\n${RED}Insertando en fich.myzip  el fichero 'pruebas/fichero ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/fichero 13
sleep 0.1

# Insertar en fich.myzip el directorio pruebas/directorio/ - APERTURA DATOS
printf "\n${RED}Insertando en fich.myzip el directorio 'pruebas/directorio/ ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/directorio/ 13
sleep 0.1

# Insertar en fich.myzip el enlace pruebas/enlace - APERTURA DATOS
printf "\n${RED}Insertando en fich.myzip el enlace 'pruebas/enlace ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/enlace 13
sleep 0.1

# Insertar en pruebas/dir1/ el fichero pruebas/fichero - APERTURA DATOS
printf "\n${RED}Insertando en pruebas/dir1/ el fichero 'pruebas/fichero ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero pruebas/dir1/ pruebas/datos.dat 13
sleep 0.1

# Insertar en pruebas/dir1/ el directorio pruebas/directorio/ - APERTURA DATOS
printf "\n${RED}Insertando en pruebas/dir1/ el directorio 'pruebas/directorio/ ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero pruebas/dir1/ pruebas/dir1/ 13
sleep 0.1

# Insertar en pruebas/dir1/ el enlace pruebas/enlace - APERTURA DATOS
printf "\n${RED}Insertando en pruebas/dir1/ el enlace 'pruebas/enlace ' erroneo o inexistente...${WHITE}\n"
sleep 0.1
./inserta_fichero pruebas/dir1/ pruebas/link 13
sleep 0.1


# Insertar en fich.myzip el fichero pruebas/datos.dat - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el fichero 'pruebas/datos.dat en una posición erronea'...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/datos.dat 80
sleep 0.1

# Insertar en fich.myzip el directorio pruebas/dir1/ - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el directorio 'pruebas/dir1/ en una posición erronea'...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/dir1/ 80
sleep 0.1

# Insertar en fich.myzip el enlace pruebas/link - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el enlace 'pruebas/link en una posición erronea'...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/link 80
sleep 0.1


# Insertar el fichero de pruebas/datos.dat - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el fichero de 'pruebas/datos.dat' en una posición ocupada...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/datos.dat 5
sleep 0.1

# Insertar en fich.myzip pruebas/dir1/ y todo su contenido - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el directorio 'pruebas/dir1/' y todo su contenido en una posición ocupada...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/dir1/ 2
sleep 0.1

# Insertar en fich.myzip el enlace pruebas/link - INDEX ERROR
printf "\n${RED}Insertando en fich.myzip el enlace 'pruebas/link' en una posición ocupada...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/link 10
sleep 0.1


# Insertar el fichero de noperm_dir/ - NOPERM
printf "\n${RED}Insertando en fich.myzip el directorio de 'noperm_dir/' en la posición 30...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip noperm_dir/ 30
sleep 0.1


# Insertar el fichero de fich.myzip - MISMO PID
printf "\n${RED}Insertando en fich.myzip el fichero de 'fich.myzip'...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip fich.myzip  16
sleep 0.1


# Insertar el fichero de pruebas/datos.dat - RETORNO INDEX
printf "\n${BLUE}Insertando en fich.myzip el fichero de 'pruebas/datos.dat' en la posición 29...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/datos.dat 29
echo "El codigo de retorno es $?"
sleep 0.1

# Insertar el fichero de pruebas/datos.dat - RETORNO INDEX
printf "\n${RED}Insertando en fich.myzip el fichero de 'pruebas/datos.dat' en la posición 5...${WHITE}\n"
sleep 0.1
./inserta_fichero fich.myzip pruebas/datos.dat 5
echo "El codigo de retorno es $?"
sleep 0.1



# -- EXTRACCIÓN -- #

# Extracción del fichero regular pruebas/datos.dat
printf "\n${BLUE}Extrayendo fichero regular 'pruebas/datos.dat'...${WHITE}\n"
rm -r pruebas/
sleep 0.5
ls
sleep 0.5
./extrae_fichero fich.myzip 5
ls
tree pruebas/
sleep 0.5
cat pruebas/datos.dat
sleep 0.5

# Extracción del directorio pruebas/dir1/
printf "\n${BLUE}Extrayendo directorio 'pruebas/dir1/'...${WHITE}\n"
sleep 0.5
ls
sleep 0.5
./extrae_fichero fich.myzip 2
ls
tree pruebas/
sleep 0.5

# Extracción del fichero regular pruebas/dir1/datos2.dat
printf "\n${BLUE}Extrayendo fichero regular 'pruebas/dir1/datos2.dat'...${WHITE}\n"
sleep 0.5
ls
sleep 0.5
./extrae_fichero fich.myzip 1
ls
tree pruebas/
sleep 0.5
cat pruebas/dir1/datos2.dat
sleep 0.5

# Extracción del enlace simbólico pruebas/link
printf "\n${BLUE}Extrayendo enlace simbólico 'pruebas/link'...${WHITE}\n"
sleep 0.5
ls
sleep 0.5
./extrae_fichero fich.myzip 10
ls
ls -l pruebas/


# Comprobando la integridad de los permisos del fichero pruebas/datos.dat
printf "\n${YELLOW}Comprobando la integridad de los permisos del fichero 'pruebas/datos.dat'...${WHITE}\n"
ls -l pruebas/
chmod go-r pruebas/datos.dat
printf "\n${YELLOW}Cambiando permisos del fichero 'pruebas/datos.dat' e insertando en fich.myzip...${WHITE}\n"
ls -l pruebas/
sleep 0.3
./inserta_fichero fich.myzip pruebas/datos.dat 11
printf "\n${YELLOW}Borrando 'pruebas/datos.dat'...${WHITE}\n"
rm pruebas/datos.dat
sleep 0.2
ls -l pruebas
sleep 0.1
printf "\n${YELLOW}Extrayendo 'pruebas/datos.dat'...${WHITE}\n"
sleep 0.2
./extrae_fichero fich.myzip 11
ls -l pruebas/
sleep 0.1

if [ -d "noperm_dir/" ]; then
    chmod ugo+rxw noperm_dir
    rm -r noperm_dir/
fi

# -- ERRORES -- #

# Extracción del fichero regular - INDEX ERROR
printf "\n${RED}Extrayendo fichero regular en una posición erronea...${WHITE}\n"
sleep 0.1
./extrae_fichero fich.myzip 87
sleep 0.1

# Extracción del fichero regular - INDEX ERROR
printf "\n${RED}Extrayendo fichero regular en una posición vacia...${WHITE}\n"
sleep 0.1
./extrae_fichero fich.myzip 22
sleep 0.1


# Extracción del fichero regular - FILE ERROR
printf "\n${RED}Extrayendo fichero regular de un fichero no existente o con formato erroneo...${WHITE}\n"
sleep 0.1
./extrae_fichero myzip 20
sleep 0.1




printf "\n\n\n${BLUE} ---FINALIZADA LA VERIFICACION.---${WHITE}\n"

