/*---------------------------------------------------------------+
 |     R E D I R E C C I O N . C                                 |
 +---------------------------------------------------------------+
 |     Versión : versión final                                   |
 |     Autor :  Pablo Johannes Bäumgen Nagore                    |
 |     Asignatura :  SOP-GIIROB                                  |
 |     Descripción :  Ésta es la versión que pretendo entregar   |
 +---------------------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"

#include <unistd.h>

REDIRECCION_ORDENES red_ordenes;

//red_ordenes es un vector de estructuras (posici�n 0 �rden 0 etc. (por defecto: red_ordenes[0].entrada = 0 y red_ordenes[0].salida = 1))

void redireccion_ini(void)
{
    for(int i=0; i<9; i++){ //aquí estoy asumiendo que no van a haber mas de 9 órdenes. Podría también poner mas, creo yo)
    red_ordenes[i].entrada = 0;
    red_ordenes[i].salida = 1;
    }
}//Inicializar los valores de la estructura cmdfd

int num_descriptores;
int descriptor_fichero_entrada;
int descriptor_fichero_salida;
int descriptor_fichero_entrada_bgnd;

int pipeline(int nordenes, char * infile, char * outfile, int append, int bgnd)
{
    redireccion_ini();
    num_descriptores = 0;

    if(infile[0] != '\0'){
        descriptor_fichero_entrada = open(infile,O_RDONLY);
        if(descriptor_fichero_entrada<0) { 
            cerrar_fd();
            return ERROR; 
        }
        red_ordenes[0].entrada = descriptor_fichero_entrada;
        num_descriptores++;
        //abrir archivo y asignar descriptor para ponerlo luego como entrada de la primera orden
    }
    if(outfile[0] != '\0'){
        if(append) {
            descriptor_fichero_salida = open(outfile,O_WRONLY|O_APPEND|O_CREAT,0644);
                //a�adir al final del fichero utilizado(modo append)
        } else {
            descriptor_fichero_salida = open(outfile,O_WRONLY|O_TRUNC|O_CREAT,0644);
                //sobreescribir sobre el fichero utilizado(modo trunk)
        }
        if(descriptor_fichero_salida<0) {
            cerrar_fd();
            return ERROR; 
        }
        red_ordenes[nordenes-1].salida = descriptor_fichero_salida;
        num_descriptores++;
        //para poner en la salida de la �ltima orden
    }
    for(int i=0; i<(nordenes-1); i++){
        int fd[2];
        if(pipe(fd) == -1) { 
            cerrar_fd();           
            return ERROR; 
        }
        if(fd[0]<0 || fd[1]<0) { 
            cerrar_fd();           
            return ERROR; 
        }//(possiblemente redundante)
        num_descriptores+=2;
        red_ordenes[i].salida = fd[1];
        red_ordenes[i+1].entrada = fd[0];
    }
    //Hacer nordenes-1 pipes

    if(bgnd){

        descriptor_fichero_entrada_bgnd = open("/dev/null",O_RDONLY);
        if(descriptor_fichero_entrada_bgnd<0) { 
            cerrar_fd();            
            return ERROR; 
        }
        red_ordenes[0].entrada = descriptor_fichero_entrada_bgnd;
        num_descriptores++;        
        //abrir fichero "/dev/null"
        //asignar descriptor a red_ordenes[i].entrada
        //soy consciente que ésto significa que si en background tengo un fichero de entrada, éste no se va a tener en cuenta. 
        //podría poner un if, para evitarlo, pero con lo que pone en el enunciado de la práctica, voy a asumir que lo que se 
        //pide es lo que tengo aquí.
    }

    return OK;

    //(soy consciente de que en las comprobaciones de los valores retornados de los open,
    // podría hasta poner error si es menor que 3,
    // ya que se supone que STD0, STD1 y STDERR deberían occupar las 3 primeras posiciones)
    
    // (me he dado cuenta que es ventajoso cerrar todos los descriptores antes de retornar ERROR,
    // ya que el único sitio en el que se hace cerrar_fd() es en ejecutar, lo cuál no se ejecuta si pipeline devuelve 0)
} // Fin de la funci�n "pipeline"



int redirigir_entrada(int i)
{
    if(red_ordenes[i].entrada != 0){
        if((dup2(red_ordenes[i].entrada, 0))==-1) { return ERROR; }
    }
    return OK;
} // Fin de la funci�n "redirigir_entrada"



int redirigir_salida(int i)
{
    if(red_ordenes[i].salida !=1){
        if((dup2(red_ordenes[i].salida, 1))==-1) { return ERROR; }
    }
    return OK;
} // Fin de la funci�n "redirigir_salida"


int cerrar_fd()
{
    int resultado = OK;
    //cerrar todo lo que no sea 0, 1 o 2:

    for(int i = 0; i<num_descriptores;i++){
        if(close(i+3) == -1) {
            printf("ERROR cerrando el descriptor %d\n",i+3);
            resultado = ERROR;
        }
    }
    return resultado;
} // Fin de la funci�n "cerrar_fd"


