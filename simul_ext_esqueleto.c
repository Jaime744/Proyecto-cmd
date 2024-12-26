#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100
#define SIZE_BLOQUE 512

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
/*
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);




int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
*/
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_SIMPLE_INODE *inodo);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombreantiguo, char *nombrenuevo);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
// para copiar necesito las funciones comentadas de arriba 
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
     EXT_SIMPLE_INODE inodo;
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
     //...
     
     fent = fopen("particion.bin","r+b");
     if(fent==NULL){
         printf("Error al abrir el archivo\n");
         return -1;
     }

     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent); // LEER EL FICHERO COMPLETO
     fread(directorio,SIZE_BLOQUE, MAX_FICHEROS, fent);  
     fread(memdatos, SIZE_BLOQUE, MAX_BLOQUES_DATOS, fent); 
     fread(&ext_blq_inodos, SIZE_BLOQUE, 1, fent);
     fread(&ext_bytemaps, SIZE_BLOQUE, 1, fent);
     fread(&ext_superblock, SIZE_BLOQUE, 1, fent);     
     
     
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
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
            Printbytemaps(&ext_bytemaps);
           // printf("has introducido info\n");
            continue;  
         } 
         if (strcmp(orden,"bytemaps")==0){
            
            printf("has introducido bytemaps\n");
            continue;  
         }
         if (strcmp(orden,"cat")==0){
            
            printf("has introducido cat (imprimir)\n");
            //Imprimir()
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
            Directorio(directorio,&ext_blq_inodos,&inodo);
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

// ajustar el for :)
// hacer primero las funciones de salir :)
// y la de si el usuario pone otra cosa que no haya programado le salte un error (comando desconocido):)
// comando ayuda(help) para saber que comandos puede poner el usuario :)
// comando info 
// comando bytemaps hecho
// comando dir casi hecho
// comando rename 
//comando cat (imprimir) echo pero no implementado 
//comando remove

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_SIMPLE_INODE *inodo ){ // tengo que poner la referencia al struct el EXT_SIMPLE_INODE porque es donde esta el size 

   for(int i=0;i<MAX_FICHEROS;i++){
      if(directorio[i].dir_nfich[0] != '\0' && directorio[i].dir_inodo != -1){
         int indice = directorio[i].dir_inodo;
         if(indice>= 0 && indice<MAX_INODOS){
            printf("Nombre: %s  tamaño: %d inodos: %d ",directorio[i].dir_nfich,inodo[indice].size_fichero,indice);
            printf("\n");
         }else{
            printf("Error: indice del inodo Invalido para el archivo '%s'. Indice: %d\n",directorio[i].dir_nfich,indice);
         }
         
      }
   }
   printf("termine\n"); // esto lo hace pero lo otro no jiji
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   int contadorBits=0;
   printf("Inodos: ");
   for(int i=0;contadorBits<25;i++){
      for(int j=0;contadorBits<25 && j<8;j++){
         if((ext_bytemaps->bmap_inodos[i]&(1>>j))!=0){
            printf("1");
         }else{
            printf("0");
         }
         contadorBits++;
      }
   }
   printf("\n");
   contadorBits=0;

   printf("Bloques[0-25]: ");
   for(int i=0;contadorBits<25;i++){
      for(int j=0;contadorBits<25 && j<8;j++){
         if((ext_bytemaps->bmap_bloques[i]&(1>>j))!=0){
            printf("1");
         }else{
            printf("0");
         }
         contadorBits++;
      }
   }
   printf("\n");
}
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombreantiguo, char *nombrenuevo){
   printf("hola");
}









/*

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_DATOS *memdatos, char *nombre){
   int i,j;
   unsigned int blnumber;
   EXT_DATOS datosFichero[MAX_NUMS_BLOQUE_INODO];
   i= BuscaFich(directorio,inodos,nombre);
   if(i>0){
      j=0;
      do{
         blnumber= inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j];
         if(blnumber!= NULL_BLOQUE){
            datosFichero[j]=memdatos[blnumber-PRIM_BLOQUE_DATOS];
         }
         j++;
      }while((blnumber!= NULL_BLOQUE)&& (j<MAX_NUMS_BLOQUE_INODO));
      printf("%s\n",datosFichero);
   }
   return -1;// codigo error no se encuentra
}
*/
