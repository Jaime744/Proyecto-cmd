#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100
#define SIZE_BLOQUE 512

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombre);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_DATOS *memdatos, char *nombre,EXT_SIMPLE_INODE inodo);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
int copia(EXT_ENTRADA_DIR *directorio,EXT_DATOS *memdatos, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, 
          EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombreOriginal,char* nombreCopia,  FILE *fich);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombreantiguo, char *nombrenuevo);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
void helpInformacion();



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
           printf(">> ");
           fgets(orden,sizeof(orden),stdin);
           orden[strcspn(orden,"\n")]='\0';
         if (strcmp(orden,"info")==0){
            LeeSuperBloque(&ext_superblock);
            continue;  
         } 
         if (strcmp(orden,"bytemaps")==0){
            Printbytemaps(&ext_bytemaps);
            continue;  
         }
         if (strcmp(orden,"cat")==0){
            char nombre[LEN_NFICH];
            printf("introduce el fichero que deseas imprimir: ");
            fgets(nombre, LEN_NFICH,stdin);
            nombre[strcspn(nombre,"\n")] = '\0';
            Imprimir(directorio,&ext_blq_inodos,memdatos,nombre,inodo);
            continue;  
         }
         if (strcmp(orden,"help")==0){
            helpInformacion();
            continue;  
         }
         if (strcmp(orden,"rename")==0){

            char nombreantiguo[LEN_NFICH];
            char nombrenuevo[LEN_NFICH];

            printf("introduce el fichero que deseas renombrar: ");
            fgets(nombreantiguo, LEN_NFICH,stdin);
            nombreantiguo[strcspn(nombreantiguo,"\n")] = '\0';
            printf("introduce el nuevo nombre del fichero: ");
            fgets(nombrenuevo, LEN_NFICH,stdin);
            nombrenuevo[strcspn(nombrenuevo,"\n")] = '\0';
            Renombrar(directorio, &ext_blq_inodos,nombreantiguo,nombrenuevo);
            continue;  
         }
         if (strcmp(orden,"copy")==0){

            char nombreOriginal[LEN_NFICH];
            char nombreCopia[LEN_NFICH];
            printf("introduce el fichero que deseas copiar: ");
            fgets(nombreOriginal, LEN_NFICH,stdin);
            nombreOriginal[strcspn(nombreOriginal,"\n")] = '\0';
            printf("introduce el nuevo nombre del fichero: ");
            fgets(nombreCopia, LEN_NFICH,stdin);
            nombreCopia[strcspn(nombreCopia,"\n")] = '\0';
            printf("nombre original : %s nombre copia %s",nombreOriginal,nombreCopia);

            copia(directorio,memdatos,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,nombreOriginal,nombreCopia,fent);

            continue;  
         }
         if (strcmp(orden,"remove")==0){
            char nombreEliminar[LEN_NFICH];
            printf("Introduce el archivo que quieres eliminar: ");
            fgets(nombreEliminar, LEN_NFICH,stdin);
            nombreEliminar[strcspn(nombreEliminar,"\n")] = '\0';
            Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,nombreEliminar,fent);
            continue;  
         }
         if (strcmp(orden,"dir")==0){
            Directorio(directorio,&ext_blq_inodos);
            continue;  
         }
         if (strcmp(orden,"salir")==0){
            GrabarDatos(memdatos,fent);
            printf("Adios\n");
            fclose(fent);
            apagado=1;
            continue;  
         }
         printf("ERROR: comando ilegal [bytemaps,copy,dir,info,cat,rename,remove,salir]\n");
     }
     return 0;
}

//comando remove
//comando copy 
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos) {
    for (int i = 1; i < MAX_FICHEROS; i++) {
        if (directorio[i].dir_inodo != NULL_INODO && directorio[i].dir_nfich[0] != '\0') {
            EXT_SIMPLE_INODE *inodo_actual = &inodos->blq_inodos[directorio[i].dir_inodo];
            printf("Nombre: %s  Tamaño: %u bytes  Inodo: %d  Bloques:",directorio[i].dir_nfich,inodo_actual->size_fichero,directorio[i].dir_inodo);
            
            for(int k=0; k<MAX_NUMS_BLOQUE_INODO && inodo_actual->i_nbloque[k] != NULL_BLOQUE;k++){
               printf(" %d",inodo_actual->i_nbloque[k]);
            }
            printf("\n");
        }
    }

}
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   int contadorBits=0;
   printf("Inodos: ");
   for(int i=0;i<MAX_INODOS;i++){
      printf("%d ",ext_bytemaps->bmap_inodos[i]);
   }
   printf("\n");
   printf("Bloques[0-25]: ");
   for(int j=0;j<25;j++){
      printf("%d ",ext_bytemaps->bmap_bloques[j]);
   }
   printf("\n");
}
void helpInformacion(){
   printf("Has introducido ayuda:\nPuedes poner los siguientes comandos:\n"
            "dir: para ver los directorios\nbytemaps: para ver los bytemaps\n"
            "rename: para renombrar un fichero\nremove: para eliminar un fichero\n"
            "info: para saber la info de los ficheros\ncopy: para copiar un archivo en otro\ncat: imprime el fichero que le pidas\n"
            "salir: para abandonar el programa\n");
}
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   printf("Superbloque: \nNumero total de inodos: %u\nNumero total de bloques: %u\nBloques libres: %u\nInodos libres: %u"
         "\nPrimer bloque de datos: %u\nTamaño de bloque: %u\n"
         " ",psup->s_inodes_count,psup->s_blocks_count,psup->s_free_blocks_count,psup->s_free_inodes_count,psup->s_first_data_block,psup->s_block_size);
}
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_DATOS *memdatos, char *nombre,EXT_SIMPLE_INODE inodo){
   int i,j;
   unsigned int blnumber;
   //EXT_DATOS datosFichero[MAX_NUMS_BLOQUE_INODO];
   i= BuscaFich(directorio,inodos,nombre);
   if(i==-1){
      printf("No se encontro el archivo\n");
   }
   // no va el imprimir pero si va el fichewro no encontrado
   
    if(i>0){
      j=0;
      do{
         blnumber= inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j];
         if(blnumber!= NULL_BLOQUE){
            //datosFichero[j]=memdatos[blnumber-PRIM_BLOQUE_DATOS];
            printf("%s ",memdatos[blnumber-PRIM_BLOQUE_DATOS].dato);
            j++;
         }
         
      }while((blnumber!= NULL_BLOQUE)&& (j<MAX_NUMS_BLOQUE_INODO));
      printf("\n");
   }
   return -1;// codigo error no se encuentra
}
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombre){
   //int numerin=0;
   for(int i=0;i<MAX_FICHEROS;i++){
      if(strcmp(nombre,directorio[i].dir_nfich)==0)
      return i;
   }
   return -1;
}
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombreantiguo, char *nombrenuevo){
   for(int i=0;i<MAX_FICHEROS;i++){
      if(strcmp(nombreantiguo,directorio[i].dir_nfich)==0){
         if (strlen(nombrenuevo) >= LEN_NFICH) {
                printf("Error: El nuevo nombre es demasiado largo"); // esta funcion en vez de hacerlo de nuevo podria llamar a l
                                                                    //a funcion buscarFich y que me lo busque para luego cambiarlo de nombre 
                return -1;
            }
         strcpy(directorio[i].dir_nfich,nombrenuevo);
         printf("El nombre se ha cambiado correctamente\n");
         return 0;
      }
   }
   printf("No se ha encontrado el archivo y por lo tanto no se ha cambiado el nombre\n");
}
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){
   fseek(fich,4*SIZE_BLOQUE,SEEK_SET);
   fwrite(memdatos,SIZE_BLOQUE,MAX_BLOQUES_DATOS,fich);
   printf("Los datos se han guardado correctamente\n");
}
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){
   fseek(fich,2 *SIZE_BLOQUE,SEEK_SET); // mueve el puntero al bloque de los inodos 
   fwrite(inodos,SIZE_BLOQUE,1,fich);// y escribo los inodos 
   fseek(fich,3*SIZE_BLOQUE,SEEK_SET);// muevo el puntero al bloque del directorio 
   fwrite(directorio,SIZE_BLOQUE,1,fich);//escribo el directorio
   printf(":)\n");
}
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){
   fseek(fich,SIZE_BLOQUE,SEEK_SET); // mueve el puntero al bloque de los bytemaps 
   fwrite(ext_bytemaps,SIZE_BLOQUE,1,fich);// escribo los bytemaps 
   printf(":)\n");
}
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){
   fseek(fich,0 *SIZE_BLOQUE,SEEK_SET); // mueve el puntero al inicio del archivo
   fwrite(ext_superblock,SIZE_BLOQUE,1,fich);
   printf(":)\n");
}
int copia(EXT_ENTRADA_DIR *directorio,EXT_DATOS *memdatos, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombreOriginal,char* nombreCopia,  FILE *fich){

   // inicializo variables 
      int numerin=BuscaFich(directorio,inodos,nombreOriginal);
    int numerinVerificacion=BuscaFich(directorio,inodos,nombreCopia);
    int inodoLibre=-1;
    int bloqueNecesarios;
    int bloquesAsigandos[MAX_NUMS_BLOQUE_INODO]={0};
    int bloquesEncontrados=0;
    
   
   // comprobaciones
   if(numerin==-1){
      printf("No se encuentra el archivo %s\n",nombreOriginal);
      return -1;
   }
   //compruebo si el usuario me ha introducido un nombre ya existente
  
   if(numerinVerificacion>=0){
      printf("El archivo %s ya existe, intetelo con otro nombre\n",nombreCopia);
      return -1;
   }
    EXT_SIMPLE_INODE *inodoOrigen=&inodos->blq_inodos[directorio[numerin].dir_inodo];
    bloqueNecesarios=inodos->blq_inodos[directorio[numerin].dir_inodo].size_fichero/SIZE_BLOQUE + (inodos->blq_inodos[directorio[numerin].dir_inodo].size_fichero %SIZE_BLOQUE !=0);

   // encontrar inodos libres
   for(int i=0;i<MAX_INODOS;i++){
      if(ext_bytemaps->bmap_inodos[i]==0){
         inodoLibre=i;
         ext_bytemaps->bmap_inodos[i]=1;
         ext_superblock->s_free_inodes_count--;
         break;
      }
   }
   if(inodoLibre==-1){
      printf("No hay inodos Libres\n");
      return -1;
   }
   // encontar suficientes bloques libres 

   for(int j=0;j<MAX_BLOQUES_PARTICION && bloquesEncontrados< bloqueNecesarios;j++){
      if(ext_bytemaps->bmap_bloques[j]==0){
         bloquesAsigandos[bloquesEncontrados++]=j;
         ext_bytemaps->bmap_bloques[j]=1;
         ext_superblock->s_free_blocks_count--;
      }
   }

   if(bloquesEncontrados < bloqueNecesarios){
      printf("No hay bloques suficientes\n");
      return -1;
   }

  
   EXT_SIMPLE_INODE * inodoDestino=&inodos->blq_inodos[inodoLibre];

   for (int k = 0; k < bloqueNecesarios; k++) {
        int bloqueOrigen = inodoOrigen->i_nbloque[k];
        int bloqueDestino = bloquesAsigandos[k];
        inodos->blq_inodos[inodoLibre].i_nbloque[k] = bloqueDestino;
        memcpy(memdatos[bloqueDestino].dato, memdatos[bloqueOrigen].dato,SIZE_BLOQUE);    
    }
   
   inodoDestino->size_fichero=inodoOrigen->size_fichero;// tiene que ser igual de tamaño 


    for (int l = 0; l < MAX_FICHEROS; l++) {
        if (directorio[l].dir_inodo == NULL_INODO) { 
            strcpy(directorio[l].dir_nfich, nombreCopia);
            directorio[l].dir_inodo = inodoLibre;
            break;
        }
    }

   // actualizamos en el fichero particiones.bin-->fich
    GrabarSuperBloque(ext_superblock, fich);
    GrabarByteMaps(ext_bytemaps, fich);
    Grabarinodosydirectorio(directorio, inodos, fich);
    //GrabarDatos(memdatos, fich);

   printf("Archivo %s ha sido copiado como %s con exito\n",nombreOriginal,nombreCopia);
}
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombre,  FILE *fich){
      int numerin= BuscaFich(directorio,inodos,nombre);

      if(numerin==-1){
      printf("No se encontro el archivo\n");
      return -1;
   }

   int numerin2=directorio[numerin].dir_inodo;
   EXT_SIMPLE_INODE *inodo =&inodos->blq_inodos[numerin2];

   for(int i=0;i<MAX_NUMS_BLOQUE_INODO;i++){
      int bloque = inodo->i_nbloque[i];
      if(bloque!=NULL_BLOQUE){
         ext_bytemaps->bmap_bloques[bloque]=0;
         ext_superblock->s_free_blocks_count++;
         inodo->i_nbloque[i]=NULL_BLOQUE;
      }
   }

   inodo->size_fichero=0;
   for(int j=0;j<MAX_NUMS_BLOQUE_INODO;j++){
      inodo->i_nbloque[j]=NULL_BLOQUE;
   }

   ext_bytemaps->bmap_inodos[numerin2]=0;
   ext_superblock->s_free_inodes_count++;

   GrabarSuperBloque(ext_superblock, fich);
    GrabarByteMaps(ext_bytemaps, fich);
    Grabarinodosydirectorio(directorio, inodos, fich);

   printf("El fichero %s se ha borrado con exito\n",nombre);

}

