#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
/*
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
*/
int main()
{
	 char *comando[LONGITUD_COMANDO];
	 char orden[LONGITUD_COMANDO];
	 char *argumento1[LONGITUD_COMANDO];
	 char *argumento2[LONGITUD_COMANDO];
	 
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
     //...
     
     fent = fopen("particion.bin","r+b");
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     /*
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     */
    int apagado=-1;
     // Buce de tratamiento de comandos
     while(apagado!=1){
		 
       /*do {
		 printf (">> ");
		 fflush(stdin);
		 fgets(comando, LONGITUD_COMANDO, stdin);
		 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
	     if (strcmp(orden,"dir")==0) {
            Directorio(&directorio,&ext_blq_inodos);
            continue;
            }*/

           printf(">> ");
           fgets(orden,sizeof(orden),stdin);
           orden[strcspn(orden,"\n")]='\0';

           //printf("has introducido: %s\n",orden);

         //...
         // Escritura de metadatos en comandos rename, remove, copy     
         //Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
         //GrabarByteMaps(&ext_bytemaps,fent);
         //GrabarSuperBloque(&ext_superblock,fent);
         //if (grabardatos)
           //GrabarDatos(&memdatos,fent);
         //grabardatos = 0;
         //Si el comando es salir se habrán escrito todos los metadatos
         //faltan los datos y cerrar

         // todas las ordenes van a tener esta estructura
         if (strcmp(orden,"info")==0){
            
            printf("has introducido info\n");
            continue;  
         } 
         if (strcmp(orden,"bytemaps")==0){
            
            printf("has introducido bytemaps\n");
            continue;  
         }
         if (strcmp(orden,"cat")==0){
            
            printf("has introducido cat (imprimir)\n");
            continue;  
         }
         if (strcmp(orden,"help")==0){
            
            printf("Has introducido ayuda:\nPuedes poner los siguientes comandos:\n"
            "dir: para ver los directorios\nbytemaps: para ver los bytemaps\n"
            "rename: para renombrar un fichero\nremove: para eliminar un fichero\n"
            "info: para saber la info de los ficheros\ncopy: para copiar un archivo en otro\n"
            "salir: para abandonar el programa\n");
            continue;  
         }
         if (strcmp(orden,"rename")==0){
            
            printf("has introducido rename\n");
            continue;  
         }
         if (strcmp(orden,"copy")==0){
            
            printf("has introducido copyn");
            continue;  
         }
         if (strcmp(orden,"remove")==0){
            
            printf("has introducido remove\n");
            continue;  
         }

         if (strcmp(orden,"dir")==0){
            
            printf("has introducido dir\n");
            continue;  
         }
         if (strcmp(orden,"salir")==0){
            //GrabarDatos(&memdatos,fent);
            printf("Adios\n");
            fclose(fent);
            apagado=1;
            //close();
            continue;  
         }
         printf("ERROR: comando ilegal [bytemaps,copy,dir,info,cat,rename,remove,salir]\n");
     }
     return 0;
}

// ajustar el for 
// hacer primero las funciones de salir
// y la de si el usuario pone otra cosa que no haya programado le salte un error (comando desconocido)
// comando ayuda(help) para saber que comandos puede poner el usuario 
// comando info 
// comando bytemaps 
// comando dir 
// comando rename 
//comando cat (imprimir)
//comando remove
