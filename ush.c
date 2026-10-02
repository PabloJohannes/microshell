/*-----------------------------------------------------+
 |     U S H. C                                        
 +-----------------------------------------------------+
 |     Versión :                                       |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |                               
 |     Descripción :                                   |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "profe.h"

#include <stdio.h>


//
// Declaraciones de funciones locales
//
void visualizar( void );
int leerLinea( char *linea, int maxLinea );


//
// Prog. ppal.
// 
int main(int argc, char * argv[])
{
    
  char line[255];
  int res;
  char **m_ordenes;
  char ***m_argumentos;
  int *m_num_arg;
  int m_n;

  while(1)
  {
    
    do
    {
        res=leerLinea(line,MAXLINE);    
        if (res==-2) {
      		fprintf(stdout,"logout\n");
		      exit(0);
        }
	
    	  if (res==-1){
		      fprintf(stdout,"linea muy larga\n");
	      }
     }while(res<=0);

     if (analizar(line)==OK){
       m_n=num_ordenes();
	     m_num_arg=num_argumentos();
	     m_ordenes=get_ordenes();
	     m_argumentos=get_argumentos();
	     if(m_n>0)
	     {
          if (pipeline_profe(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==OK)
                    ejecutar(m_n,m_num_arg,m_ordenes,m_argumentos,es_background());
        }

       visualizar();
     }
 }    

  return 0;
}





/****************************************************************/
/*                       leerLinea                             
  --------------------------------------------------------------
                                                               
   DESCRIPCIÓN:                                                 
   Obtiene la línea de órdenes para el mShell.    
   Util para depuracion.                                        
                                                                
   ENTRADA:                                                 
    linea - puntero a un vector de carácteres donde se almancenan los caracteres 
   leídos del teclado
    tamanyLinea - tamaño máximo del vector anterior

   SALIDA:
    -- linea - si termina bien, contiene como último carácter el retorno de carro.
    -- leerLinea -  Entero que representa el motivo de finalización de la función:
     > 0 - terminación correcta, número de caracteres leídos, incluído '\n'
     -1 - termina por haber alcanzado el número máximo de caracteres que se 
    espera leer de teclado, sin encontrar '\n'.
     -2 - termina por haber leído fin de fichero (EOF).
*/
/****************************************************************/
//char * getline(void)
int leerLinea( char *linea, int maxLinea ) {
  char dir[MAXDIRECTORYLENGTH];
  if(getcwd(dir,sizeof(dir))!=NULL){
    printf("%s%s",dir,PROMPT);
  }else{
    printf("Error al sacar el directorio.\n");
  }
  int c;
  int i = 0;
  while(OK){
    c = getchar();
    if(c == EOF){
      return -2;
    }
    linea[i] = c;
    i++;
    if(i>=maxLinea){
      return -1;
    }
    if(c=='\n'){
      linea[i] = '\0';
      return i;
    }
  }
}




/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/*                                                              */
/* DESCRIPCIÓN:                                                 */
/* Visualiza los distintos argumentos de la orden analizada.    */
/* Util para depuracion.                                        */
/*                                                              */
/* ENTRADA: void                                                */
/*                                                              */
/* SALIDA: void                                                 */
/*                                                              */
/****************************************************************/
void visualizar(void) {  
    int n = num_ordenes();
    char **ordenes = get_ordenes();
    int *num_arg = num_argumentos();
    char ***argumentos = get_argumentos();

    printf("\n");
    printf("\033[35m╔══════════════════════════════════════╗\033[0m\n");
    printf("\033[35m║          ANALISIS DE LA ORDEN        ║\033[0m\n");
    printf("\033[35m╚══════════════════════════════════════╝\033[0m\n");


    printf("Numero de ordenes: %d\n\n", n);

    for (int i = 0; i < n; i++) {
        printf("┌─ \033[35mOrden %d: %s\033[0m\n", i + 1, ordenes[i]);
        printf("│  Numero de argumentos: %d\n", num_arg[i]);
        for (int j = 0; j < num_arg[i]; j++) {
            printf("│  - Argumento %d: %s\n", j, argumentos[i][j]);
        }
        printf("└─────────────────────────────\n\n");
    }
      //┌│└─


    if (strlen(fich_entrada()) > 0) {
        printf("Redireccion de entrada -> %s\n", fich_entrada());
    }else {
        printf("Redireccion de entrada -> \033[31mninguna\033[0m\n");
    }
    if (strlen(fich_salida()) > 0) {
        printf("Redireccion de salida -> %s\n", fich_salida());

        if (es_append())
            printf("     -> Append\n");
        else
            printf("     -> Trunk\n");
    } else {
        printf("Redireccion de salida -> \033[31mninguna\033[0m\n");
    }
    printf("\n");

    if (es_background()) {
        printf("BACKGROUND\n");
    }else {
        printf("FOREGROUND\n");
    }
} // Fin de "visualizar"



