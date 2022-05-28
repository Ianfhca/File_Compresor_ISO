#define E_OPEN1(e) {fprintf(stderr, e); exit(-1);};
#define E_OPEN2(e) {fprintf(stderr, e); exit(-2);};
#define E_POS1(e) {fprintf(stderr, e); exit(-3);};
#define E_POS2(e) {fprintf(stderr, e); exit(-4);};
#define E_OPENDIR1(e) {fprintf(stderr, e); exit(-5);};
#define E_DESCO(e) {fprintf(stderr, e); exit(-99);};

extern int inserta_fichero(char * f_mypackzip, char * f_dat, int index);
