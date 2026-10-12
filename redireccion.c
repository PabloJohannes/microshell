/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                        
 |     Autor :   
 |     Asignatura :  SOP-GIIROB                                                       
 |     Descripcion: 
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"

#include <unistd.h>     //preguntar a Silvia si quiere que lo pongamos aquí también (lo pongo ahora porque así se me quitan los mensajes de warning)

REDIRECCION_ORDENES red_ordenes;

//red_ordenes es un vector de estructuras (posici�n 0 �rden 0 etc. (por defecto: red_ordenes[0].entrada = 0 y red_ordenes[0].salida = 1))

void redireccion_ini(void)
{
    for(int i=0; i<9; i++){
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


    if(infile != ""){
        descriptor_fichero_entrada = open(infile,O_RDONLY);
        if(descriptor_fichero_entrada<0) { return ERROR; }
        red_ordenes[0].entrada = descriptor_fichero_entrada;
        num_descriptores++;
        //abrir archivo y asignar descriptor para ponerlo luego como entrada de la primera orden
    }

    if(outfile != ""){
        if(append) {
            descriptor_fichero_salida = open(outfile,O_WRONLY,O_APPEND);
                //a�adir al final del fichero utilizado(modo append)
        } else {
            descriptor_fichero_salida = open(outfile,O_WRONLY,O_TRUNC);
                //sobreescribir sobre el fichero utilizado(modo trunk)
        }
        if(descriptor_fichero_salida<0) { return ERROR; }
        red_ordenes[nordenes-1].salida = descriptor_fichero_salida;
        num_descriptores++;
        //para poner en la salida de la �ltima orden
    }
    for(int i=0; i<(nordenes-1); i++){
        int fd[2];
        if(pipe(fd) == -1) { return ERROR; }
        if(fd[0]<0 || fd[1]<0) { return ERROR; }
        num_descriptores+=2;
        red_ordenes[i].salida = fd[1];
        red_ordenes[i+1].entrada = fd[0];
    }
    //Hacer nordenes-1 pipes

    if(bgnd){

        descriptor_fichero_entrada_bgnd = open("/dev/null",O_RDONLY);
        red_ordenes[0].entrada = descriptor_fichero_entrada_bgnd;
        if(descriptor_fichero_entrada_bgnd<0) { return ERROR; }
        num_descriptores++;        
        //abrir fichero "/dev/null"
        //asignar descriptor a red_ordenes[i].entrada
    }

    return OK;
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
    //cerrar todo lo que no sea 0, 1 o 2
    for(int i = 0; i<num_descriptores;i++){
        close(i+3);
    }
} // Fin de la funci�n "cerrar_fd"


