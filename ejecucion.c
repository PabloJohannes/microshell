/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Asignatura :  SOP-GIIROB                        |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <signal.h>
#include "profe.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int ejecutar (int nordenes , int *nargs , char **ordenes , char ***args , int bgnd) {
    if(!bgnd){
        for(int i=0;i<nordenes;i++) {
            pid_t val=fork();
            if(val==0) { //Hijo
                redirigir_entrada(i);
                redirigir_salida(i);
                cerrar_fd();
                execvp(ordenes[i], args[i]);
                printf("Error haciendo el execvp en la orden %d",i+1);
                _exit(ERROR);
            }else if(val==-1) {
                printf("Error haciendo el fork para la orden %d",i+1);
                exit(ERROR);
                //mensaje error de fork
            }
        }
    //Padre:
    cerrar_fd();

    while(wait(NULL) != -1);
    }
    return OK;
} // Fin de la funcion "ejecutar"
