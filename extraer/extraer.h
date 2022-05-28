#define E_OPEN(e) {fprintf(stderr, e); exit(1);};
#define E_POS(e) {fprintf(stderr, e); exit(2);};
#define E_DIR1(e) {fprintf(stderr, e); exit(3);};
#define E_DIR2(e) {fprintf(stderr, e); exit(4);};
#define E_DESCO(e) {fprintf(stderr, e); exit(99);};

extern int extrae_fichero(char * f_mypackzip, int index);
