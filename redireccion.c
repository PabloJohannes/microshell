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

int pipeline(int nordenes, char * infile, char * outfile, int append, int bgnd)
{
    redireccion_ini();


    if(infile != ""){
        red_ordenes[0].entrada = open(infile,O_RDONLY);
        num_descriptores++;
        //abrir archivo y asignar descriptor para ponerlo luego como entrada de la primera orden
    }

    if(outfile != ""){
            if(append) {
                red_ordenes[nordenes-1].salida = open(outfile,O_WRONLY,O_APPEND);
                //a�adir al final del fichero utilizado(modo append)
            } else {
                red_ordenes[nordenes-1].salida = open(outfile,O_WRONLY,O_TRUNC);
                //sobreescribir sobre el fichero utilizado(modo trunk)
            }
        num_descriptores++;
        //para poner en la salida de la �ltima orden
    }
    for(int i=0; i<(nordenes-1); i++){
        int fd[2];
        pipe[fd];
        num_descriptores+=2;
        red_ordenes[i].salida = fd[1];
        red_ordenes[i+1].entrada = fd[0];
    }
    //Hacer nordenes-1 pipes

    if(bgnd){

        red_ordenes[0].entrada = open("/dev/null",O_RDONLY);
        num_descriptores++;        
        //abrir fichero "/dev/null"
        //asignar descriptor a red_ordenes[i].entrada
    }

    return OK;
} // Fin de la funci�n "pipeline"



int redirigir_entrada(int i)
{
    if(red_ordenes[i].entrada != 0){
        dup2(red_ordenes[i].entrada, 0);
    }
    //falta devolver 0 en caso de error
    return OK;
} // Fin de la funci�n "redirigir_entrada"



int redirigir_salida(int i)
{
    if(red_ordenes[i].salida !=1){
        dup2(red_ordenes[i].salida, 1);
    }
    //falta devolver 0 en caso de error
    return OK;
} // Fin de la funci�n "redirigir_salida"


int cerrar_fd()
{
    //cerrar todo lo que no sea 0, 1 o 2
    for(int i = 0; i<num_descriptores;i++){
        close(i+3);
    }
} // Fin de la funci�n "cerrar_fd"


