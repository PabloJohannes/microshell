/*---------------------------------------------------------------+
 |     E J E C U C I O N. C                                 |
 +---------------------------------------------------------------+
 |     Versión : versión final                                   |
 |     Autor :  Pablo Johannes Bäumgen Nagore                    |
 |     Asignatura :  SOP-GIIROB                                  |
 |     Descripción :  Ésta es la versión que pretendo entregar   |
 +---------------------------------------------------------------*/
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
                if (redirigir_entrada(i) == ERROR){
                    printf("Error redirigiendo la entrada de la orden %d\n", i+1);
                    exit(ERROR);
                }
                if (redirigir_salida(i) == ERROR){
                    printf("Error redirigiendo la salida de la orden %d\n", i+1);
                    exit(ERROR);
                }               
                cerrar_fd();
                execvp(ordenes[i], args[i]);
                printf("Error haciendo el execvp en la orden %d\n",i+1);
                exit(ERROR);
            }
            if(val==-1) {
                printf("Error haciendo el fork para la orden %d\n",i+1);
                exit(ERROR);
                //mensaje error de fork
            }
        }
    //Padre:
        cerrar_fd();

        while(wait(NULL) != -1);
    }
    else {
        pid_t valb1=fork();
        if(valb1==-1){
            printf("Error al hacer el primer fork para la ejecución en background");
            exit(ERROR);
        }
        if(valb1==0) {
            pid_t valb2=fork();
            if(valb2==-1){
                printf("Error al hacer el segundo fork para la ejecución en background");
                exit(ERROR);
            }
            if(valb2==0) {
                for(int i=0;i<nordenes;i++) {
                    pid_t val=fork();
                    if(val==0) { //Hijo
                        if (redirigir_entrada(i) == ERROR){
                            printf("\nExit\n");
                            printf("la línea de ordenes que ha producido éste error es: ");
                            for(int aux=0; aux<nordenes; aux++){
                                printf("%s ",ordenes[aux]); }
                            printf("\n");
                            printf("Error redirigiendo la entrada de la orden %d\n", i+1);
                            exit(ERROR);
                        }
                        if (redirigir_salida(i) == ERROR){
                            printf("\nExit\n");
                            printf("la línea de ordenes que ha producido éste error es: ");
                            for(int aux=0; aux<nordenes; aux++){
                                printf("%s ",ordenes[aux]); }
                            printf("\n");
                            printf("Error redirigiendo la salida de la orden %d\n", i+1);                            
                            exit(ERROR);
                        }               
                        cerrar_fd();
                        execvp(ordenes[i], args[i]);
                        printf("\nExit\n");
                        printf("la línea de ordenes que ha producido éste error es: ");
                        for(int aux=0; aux<nordenes; aux++){
                            printf("%s ",ordenes[aux]); }
                        printf("\n");
                        printf("Error haciendo el execvp en la orden %d\n",i+1);
                        exit(ERROR);
                    }
                    if(val==-1) {
                        printf("\nExit\n");
                        printf("la línea de ordenes que ha producido éste error es: ");
                        for(int aux=0; aux<nordenes; aux++){
                            printf("%s ",ordenes[aux]); }
                        printf("\n");
                        printf("Error haciendo el fork para la orden %d\n",i+1);
                        exit(ERROR);
                        //mensaje error de fork
                    } 
                }
                cerrar_fd();

                while(wait(NULL) != -1);

                printf("\nDone\n");
                exit(0);
            }
            else {
                exit(0);//(el hijo creado con calb1=fork(); siempre va a hacer este exit, lo que permite que acabe el proceso padre rapidamente)
            }
        }
        cerrar_fd();

        while(wait(NULL) != -1);
    }    
    return OK;
} // Fin de la funcion "ejecutar"






















