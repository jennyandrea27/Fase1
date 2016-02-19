#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct disco{
    //guardara los datos de mkdisk
    int tam;
    char* unidad;
    char* path;
}disco;

typedef struct Part{
    //guardara los datos para fdisk
    int tam;
    char* unidad;
    char* path;
    char* tipo;
    char* ajuste;
    char* borrar;
    char* nombre;
    int agregar;

}Part;

typedef struct objeto{
    //guarda nombre y valor del parametro
    char nombre[10];
    char valor[100];
}objeto;

typedef struct Buffer{
    //usado para crear los archivos
    char b[1024];
}Buffer;

typedef struct Particion{
    //estructura para cada particion detro del mbr
    char estado[1];
    char tipo[1];
    char ajuste[2];
    int inicio;
    int tamano;
    char nombre[16];
}Particion;

typedef struct MBR{
    int tamano;
    char fecha_creacion[100];
    int clave;
    Particion particion_1;
    Particion particion_2;
    Particion particion_3;
    Particion particion_4;

}MBR;

typedef struct EBR{
    char estado[1];
    char ajuste[1];
    int inicio;
    int tamano;
    int siguiente;
    char nombre[16];
}EBR;


objeto* Separar(char* par){
    objeto* temp=(objeto*)malloc(sizeof(objeto));
        char igual='=';
        char espacio='\0';
        bool path=false;
        for(int i =0;i<=10;i++){
            temp->nombre[i]='\0';
        }
        for(int i =0;i<=100;i++){
            temp->nombre[i]='\0';
        }
        //quitar - del parametro
        par++;
        //cont indica en que lugar de la cadena escribir
        int cont=0;
        while(*par != igual){
            *par=tolower(*par);
            temp->nombre[cont]=*par;//escribir en atributo nombre
            par++;
            cont++;
        }
        //quitar = del parametro
        par++;
        if(strcmp(temp->nombre,"path") == 0){
            path = true;
        }
        //reiniciar cont para escribir en atributo valor
        cont=0;
        while(*par != espacio){
            //verificar si ya se uso parametro path
            if(!path){
                *par=tolower(*par);
            }
            temp->valor[cont]=*par;//escribir en atributo valor
            par++;
            cont++;
        }

    return temp;
}

disco* CrearDisco(char* linea){
    disco *discotemp=(disco*)malloc(sizeof(disco));
    discotemp->unidad="m";
    discotemp->path="";
    discotemp->tam=0;
    //tomar primer parametro que esta en puntero linea
    char* l=strtok(linea," ");
    //tomar el nombre y el valor del parametro
    while(l!=NULL){
        objeto* o=Separar(l);
        if(strcmp(o->nombre,"size") == 0){
            discotemp->tam=atoi(o->valor);
        }else if(strcmp(o->nombre,"unit")==0){
            discotemp->unidad=o->valor;
        }else if(strcmp(o->nombre,"path")==0){
            discotemp->path=o->valor;
        }
        l=strtok(NULL," ");
    }
    return discotemp;
}

Part* CrearPart (char* linea){
    Part *parttemp=(Part*)malloc(sizeof(Part));
   //inicializar las variables
    parttemp->tam=0;
    parttemp->unidad="k";
    parttemp->path=" ";
    parttemp->tipo="p";
    parttemp->ajuste="wf";
    parttemp->borrar=" ";
    parttemp->nombre=" ";
    parttemp->agregar=0;
    //separar comandos por espacio
    char* l=strtok(linea," ");
    //tomar el nombre y el valor de los parametros
    while(l!=NULL){
        objeto* o=Separar(l);
        if(strcmp(o->nombre,"size") == 0){
            parttemp->tam=atoi(o->valor);
        }else if(strcmp(o->nombre,"unit")==0){
            parttemp->unidad=o->valor;
            printf("unit separar %s",parttemp->unidad);
        }else if(strcmp(o->nombre,"path")==0){
            parttemp->path=o->valor;
        }else if(strcmp(o->nombre,"type")==0){
            parttemp->tipo=o->valor;
        }else if(strcmp(o->nombre,"fit")==0){
            parttemp->ajuste=o->valor;
        }else if(strcmp(o->nombre,"delete")==0){
            parttemp->borrar=o->valor;
        }else if(strcmp(o->nombre,"name")==0){
            parttemp->nombre=o->valor;
        }else if(strcmp(o->nombre,"add")==0){
            parttemp->agregar=atoi(o->valor);
        }
        l=strtok(NULL," ");
    }
    //verificar errores en los parametros


    return parttemp;

}

MBR* CrearMBR (disco* o){
    MBR *mbr=(MBR*)malloc(sizeof(MBR));    
    if(strcmp(o->unidad,"m")==0){
        mbr->tamano=1024 * o->tam;
    }else{
        mbr->tamano=o->tam;
    }
    time_t fecha=time (0);
    struct tm* fecha_t=localtime(&fecha);
    strftime(mbr->fecha_creacion,100,"%d/%m/%y %H:%M:%S",fecha_t);
    mbr->clave=rand()%1000;
    //1
    mbr->particion_1.estado[0]='0';
    mbr->particion_1.ajuste[0]='o';
    strcpy(mbr->particion_1.nombre,"o");
    mbr->particion_1.inicio=0;
    mbr->particion_1.tamano=0;
    mbr->particion_1.tipo[0]='o';
    //2
    mbr->particion_2.estado[0]='0';
    mbr->particion_2.ajuste[0]='o';
    strcpy(mbr->particion_1.nombre,"o");
    mbr->particion_2.inicio=0;
    mbr->particion_2.tamano=0;
    mbr->particion_2.tipo[0]='o';
    //3
    mbr->particion_3.estado[0]='0';
    mbr->particion_3.ajuste[0]='o';
    strcpy(mbr->particion_1.nombre,"o");
    mbr->particion_3.inicio=0;
    mbr->particion_3.tamano=0;
    mbr->particion_3.tipo[0]='o';
    //4
    mbr->particion_4.estado[0]='0';
    mbr->particion_4.ajuste[0]='o';
    strcpy(mbr->particion_1.nombre,"o");
    mbr->particion_4.inicio=0;
    mbr->particion_4.tamano=0;
    mbr->particion_4.tipo[0]='o';

    return mbr;
}

int ContParticiones (MBR *mbr_temp){
    int i=0;
    if(mbr_temp->particion_1.estado[0] == '1'){
        i++;
    }
    if(mbr_temp->particion_2.estado[0] == '1'){
        i++;
    }
    if(mbr_temp->particion_3.estado[0] == '1'){
        i++;
    }
    if(mbr_temp->particion_4.estado[0] == '1'){
        i++;
    }
    return i;
}

int Extendida( MBR *mbr_temp){
    if(mbr_temp->particion_1.tipo[0] == 'e'){
        return 1;
    }
    if(mbr_temp->particion_2.tipo[0] == 'e'){
        return 2;
    }
    if(mbr_temp->particion_3.tipo[0] == 'e'){
        return 3;
    }
    if(mbr_temp->particion_4.tipo[0] == 'e'){
        return 4;
    }
    return 0;
}

int EliminarDisco(char* linea){
    int i=0;
    //quitar espacio que viene al inicio de la cadena de parametros
    char* path=strtok(linea," ");
    path=strtok(path,"=");
    path=strtok(NULL,"=");
    //eliminar archivo
    if(fopen(path,"rb") != NULL){
        remove(path);
        printf("Se ha eliminado disco\n");
        return 1;
    }else{
        printf("Disco a eliminar no existe, verifique path\n");
        return 0;
    }
}



void Mkdisk(char linea[200]){
    FILE* archivo_disco;
    disco* discotemp=CrearDisco(linea);
    if(discotemp->tam<=0){//verifica que el parametro tama;o sea correcto
        printf("No se pudo crear disco, verifique parametro -size\n");
    }else{//else tama;o
        if(strcmp(discotemp->unidad,"k")==0|| strcmp(discotemp->unidad,"m")==0){//verifica que el valor d unit se correcto
            if(fopen(discotemp->path,"rb")!=NULL){//archivo ya existe
                printf("Disco Duplicado\n");
            }else{//archivo no existe
                //crear arhivo
                printf("Se creara disco\n");
                archivo_disco=fopen(discotemp->path,"wb+");
                Buffer* b=(Buffer*)malloc(sizeof(Buffer));
                //verificar el tama;o del disco
                int tam;
                if(strcmp(discotemp->unidad,"m")==0){
                    tam=1024*discotemp->tam;
                }else{
                    tam=discotemp->tam;
                }
                //crear mbr
                MBR* mbr=CrearMBR(discotemp);
                mbr->tamano=mbr->tamano*1024;
                //llenar archivo hasta el tama;o especificado
                for(int i=1;i<=tam;i++){
                    fwrite(b,sizeof(Buffer),1,archivo_disco);
                }
                //agregar mbr al archivo
                fseek(archivo_disco,0,SEEK_SET);
                fwrite(mbr,sizeof(MBR),1,archivo_disco);
                //leer mbr
                fseek(archivo_disco,0,SEEK_SET);
                MBR *mbr_temp=(MBR*)malloc(sizeof(MBR));
                fread(mbr_temp,sizeof(MBR),1,archivo_disco);
                fclose(archivo_disco);
            }
        }else{//else unit
            printf("No se pudo crear disco, verifique parametro -unit\n");
        //verificar si el archivo ya existe
        }

    }
}

void Fdisk(char* linea){
    Part *partt=CrearPart(linea);
    FILE* archivo_disco;
    MBR* lectura_mbr;
    int inicio=0;//lleva bit para agregar EBR a particion extendida
    bool insertado=false;//lleva control si se inserto la particion
    int extendida;//recibe valor de metodo Extendida
    archivo_disco=fopen(partt->path,"rb");
    if(archivo_disco!=NULL){//verifica que el disco exista
        fclose(archivo_disco);
        if(!strcmp(partt->nombre," ")==0){//verifica que el nombre no sea vacio
            //verificar si viene parametro delete
            if(!strcmp(partt->borrar," ")==0){//si viene parametro delete
               //verificar parametros name
                   //verificar que valor viene en el parametro
                   if(strcmp(partt->borrar,"fast")){

                   }else if(strcmp(partt->borrar,"full")==0){

                   }else{
                       printf("Verifique valor de comando delete\n");
                   }
            }else{//no viene parametro borrar
                //verifica si viene parametro add
                if(partt->agregar!=0){

                }else{//no viene add                                        
                    //abrir archivo
                    printf("path %s",partt->path);
                    archivo_disco=fopen(partt->path,"rb+");
                    //leer mbr
                    lectura_mbr=(MBR*)malloc(sizeof(MBR));
                    fseek(archivo_disco,0,SEEK_SET);
                    fread(lectura_mbr,sizeof(MBR),1,archivo_disco);
                    extendida=Extendida(lectura_mbr);//verificar si existe particion extendida
                    fclose(archivo_disco);
                    //verificar que no exista particion
                    if(strcmp(lectura_mbr->particion_1.nombre,partt->nombre)==0){
                        printf("Ya existe particion %s\n",partt->nombre);
                    }else if (strcmp(lectura_mbr->particion_2.nombre,partt->nombre)==0){
                        printf("Ya existe particion %s\n",partt->nombre);
                    }else if (strcmp(lectura_mbr->particion_3.nombre,partt->nombre)==0){
                        printf("Ya existe particion %s\n",partt->nombre);
                    }else if (strcmp(lectura_mbr->particion_4.nombre,partt->nombre)==0){
                        printf("Ya existe particion %s\n",partt->nombre);
                    }else{
                        //si la particion no existe entonces verificar la cantidad de particiones existen
                        printf("si la particion no existe entonces verificar la cantidad de particiones existen\n");
                        if(ContParticiones(lectura_mbr) == 4){
                            printf("Se excede el total de particiones permitidas\n");
                        }else if(partt->tam >0){//verificar que el tama;o de la particion sea valido
                            printf("size esta bien\n");
                            if(strcmp(partt->ajuste,"bf")==0 || strcmp(partt->ajuste,"ff")==0 || strcmp(partt->ajuste,"wf")==0 ){//verificar que el ajuste sea valido
                                if(strcmp(partt->unidad,"b")==0 || strcmp(partt->unidad,"k")==0 || strcmp(partt->unidad,"m")==0){
                                    //pasar unidades a bytes
                                    if(strcmp(partt->unidad,"k")==0){
                                        partt->tam=partt->tam*1024;
                                    }else if(strcmp(partt->unidad,"m")==0){
                                        partt->tam=partt->tam*1024*1024;
                                    }
                                            printf("insertar e\n");
                                            //agregar particion extendida
                                            //buscar particion libre
                                            printf("if 1=0\n");
                                            if(insertado){
                                                printf("verdadero\n");
                                            }else{
                                                printf("falso\n");
                                            }
                                            printf("1.estado %c\n",lectura_mbr->particion_1.estado[0]);
                                            if(lectura_mbr->particion_1.estado[0] == '0' && !insertado){
                                                printf("1=0\n");
                                                //buscar siguiente particion ocupada si hay
                                                if(lectura_mbr->particion_2.estado[0] == '0'){//particion 2 libre, verificar particion 3
                                                    printf("particion 2 libre\n");
                                                    if(lectura_mbr->particion_3.estado[0]== '0'){//particion 3 libre, verificar particion 4
                                                        printf("particion 3 libre\n");
                                                        if(lectura_mbr->particion_4.estado[0] == '0'){//particion 4 libre, verificar tama;o total del disco
                                                            printf("particion 4 libre\n");
                                                            int espacio=lectura_mbr->tamano - sizeof(MBR);
                                                            if(espacio >= partt->tam){//si se puede insertar particion
                                                                strcpy(lectura_mbr->particion_1.ajuste,partt->ajuste);
                                                                inicio=sizeof(MBR);
                                                                lectura_mbr->particion_1.estado[0]='1';
                                                                lectura_mbr->particion_1.inicio=sizeof(MBR);
                                                                strcpy(lectura_mbr->particion_1.nombre,partt->nombre);
                                                                lectura_mbr->particion_1.tamano=partt->tam;
                                                                strcpy(lectura_mbr->particion_1.tipo,partt->tipo);
                                                                insertado=true;
                                                                printf("Se inserto, todas libres\n");
                                                            }
                                                        }else{//cuarta particion ocupada, se verifica si hay espacio suficiente
                                                            int espacio=lectura_mbr->particion_4.inicio - sizeof(MBR);
                                                            if(espacio >= partt->tam){//si se puede insertar particion
                                                                printf("insertar particion 1, 4 ocupada\n");
                                                                strcpy(lectura_mbr->particion_1.ajuste,partt->ajuste);
                                                                lectura_mbr->particion_1.estado[0]='0';
                                                                lectura_mbr->particion_1.inicio=sizeof(MBR);
                                                                inicio=sizeof(MBR);
                                                                strcpy(lectura_mbr->particion_1.nombre,partt->nombre);
                                                                lectura_mbr->particion_1.tamano=partt->tam;
                                                                strcpy(lectura_mbr->particion_1.tipo,partt->tipo);
                                                                insertado=true;
                                                            }
                                                        }
                                                    }else{//tercera particion ocupada, se verifica si hay espacio suficiente
                                                        int espacio=lectura_mbr->particion_3.inicio - sizeof(MBR);
                                                        if(espacio >= partt->tam){//si se puede insertar particion
                                                            printf("insertar particion 1, 3 ocupada\n");
                                                            strcpy(lectura_mbr->particion_1.ajuste,partt->ajuste);
                                                            lectura_mbr->particion_1.estado[0]='0';
                                                            lectura_mbr->particion_1.inicio=sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                            strcpy(lectura_mbr->particion_1.nombre,partt->nombre);
                                                            lectura_mbr->particion_1.tamano=partt->tam;
                                                            strcpy(lectura_mbr->particion_1.tipo,partt->tipo);
                                                            insertado=true;
                                                        }
                                                    }

                                                }else{//segunda particion ocupada, se verifica si hay espacio suficiente
                                                    printf("insertar particion 1, 2 ocupada\n");
                                                    int espacio=lectura_mbr->particion_2.inicio - sizeof(MBR);
                                                    if(espacio >= partt->tam){//si se puede insertar particion
                                                        strcpy(lectura_mbr->particion_1.ajuste,partt->ajuste);
                                                        lectura_mbr->particion_1.estado[0]='0';
                                                        lectura_mbr->particion_1.inicio=sizeof(MBR);
                                                        inicio=sizeof(MBR);
                                                        strcpy(lectura_mbr->particion_1.nombre,partt->nombre);
                                                        lectura_mbr->particion_1.tamano=partt->tam;
                                                        strcpy(lectura_mbr->particion_1.tipo,partt->tipo);
                                                        insertado=true;
                                                    }
                                                }
                                            }
                                            printf("if 2=0\n");
                                            if(insertado){
                                                printf("verdadero\n");
                                            }else{
                                                printf("falso\n");
                                            }
                                            printf("2.estado %c\n",lectura_mbr->particion_2.estado[0]);
                                            if(lectura_mbr->particion_2.estado[0] == '0' && !insertado){//segunda particion libre, verificar particion 3
                                                printf("2 vacia\n");
                                                if(lectura_mbr->particion_3.estado[0] == '0'){//particion 3 libre, verificar particion 4
                                                    printf("3 vacia\n");
                                                    int espacio=0;
                                                    inicio=0;
                                                    if(lectura_mbr->particion_4.estado[0] == '0'){//particion 4 libre, verificar tama;o total del disco
                                                        printf("4 vacia\n");
                                                        if(lectura_mbr->particion_1.estado[0] == '0'){//pimer particion libre
                                                            espacio=lectura_mbr->tamano - sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                        }else{
                                                            espacio=lectura_mbr->tamano - (lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano);
                                                            inicio=lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano;
                                                        }
                                                        printf("insertar particion 2, 3 vacia\n");
                                                        if(espacio >= partt->tam){//si se puede insertar particion
                                                            strcpy(lectura_mbr->particion_2.ajuste,partt->ajuste);
                                                            lectura_mbr->particion_2.estado[0]='1';
                                                            lectura_mbr->particion_2.inicio=inicio;
                                                            strcpy(lectura_mbr->particion_2.nombre,partt->nombre);
                                                            lectura_mbr->particion_2.tamano=partt->tam;
                                                            strcpy(lectura_mbr->particion_2.tipo,partt->tipo);
                                                            insertado=true;
                                                            printf("primera ocupada, todas vacias\n");
                                                        }
                                                    }else{//cuarta particion ocupada, se verifica si hay espacio suficiente
                                                        if(strcmp(lectura_mbr->particion_1.estado,"0")==0){//pimer particion libre
                                                            espacio=lectura_mbr->particion_4.inicio - sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                        }else{
                                                            espacio=lectura_mbr->particion_4.tamano - (lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano);
                                                            inicio=lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano;
                                                        }
                                                        printf("insertar particion 2, 4 ocupada\n");
                                                        if(espacio >= partt->tam){//si se puede insertar particion
                                                            strcpy(lectura_mbr->particion_2.ajuste,partt->ajuste);
                                                            lectura_mbr->particion_2.estado[0]='1';
                                                            lectura_mbr->particion_2.inicio=inicio;
                                                            strcpy(lectura_mbr->particion_2.nombre,partt->nombre);
                                                            lectura_mbr->particion_2.tamano=partt->tam;
                                                            strcpy(lectura_mbr->particion_2.tipo,partt->tipo);
                                                            insertado=true;
                                                        }
                                                    }
                                                }else{//tercera particion ocupada, se verifica si hay espacio suficiente
                                                    int espacio=0;
                                                    if(strcmp(lectura_mbr->particion_1.estado,"0")==0){//pimer particion libre
                                                        espacio=lectura_mbr->particion_3.inicio - sizeof(MBR);
                                                    }else{
                                                        espacio=lectura_mbr->particion_3.inicio - (lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano);
                                                    }
                                                    printf("insertar particion 2, 3 ocupada\n");
                                                    if(espacio >= partt->tam){//si se puede insertar particion
                                                        strcpy(lectura_mbr->particion_2.ajuste,partt->ajuste);
                                                        lectura_mbr->particion_2.estado[0]='1';
                                                        lectura_mbr->particion_2.inicio=sizeof(MBR);
                                                        strcpy(lectura_mbr->particion_2.nombre,partt->nombre);
                                                        lectura_mbr->particion_2.tamano=partt->tam;
                                                        strcpy(lectura_mbr->particion_2.tipo,partt->tipo);
                                                        insertado=true;
                                                    }
                                                }
                                            }
                                            printf("if 3=0\n");
                                            if(insertado){
                                                printf("verdadero\n");
                                            }else{
                                                printf("falso\n");
                                            }
                                            printf("3.estado %c\n",lectura_mbr->particion_3.estado[0]);
                                            if(lectura_mbr->particion_3.estado[0] == '0' && !insertado){
                                                printf("3 vacia\n");
                                                int espacio=0;
                                                inicio=0;
                                                if(lectura_mbr->particion_4.estado[0] == '0'){//particion 4 libre, verificar tama;o total del disco
                                                    printf("4 vacia\n");
                                                    if(lectura_mbr->particion_2.estado[0] == '0'){//2 particion libre
                                                        if(lectura_mbr->particion_1.estado[0] == '0'){ //1 tambien libre
                                                            espacio=lectura_mbr->tamano - sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                        }else{//1 ocupada
                                                            espacio=lectura_mbr->tamano - (lectura_mbr->particion_1.inicio+lectura_mbr->particion_1.tamano);
                                                            inicio=lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano;
                                                        }
                                                    }else{//particion 2 ocupada
                                                        espacio=lectura_mbr->tamano - (lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano);
                                                        inicio=lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano;
                                                    }
                                                    if(espacio >= partt->tam){//si se puede insertar particion
                                                        printf("insertar particion 3, 4 libre\n");
                                                        strcpy(lectura_mbr->particion_3.ajuste,partt->ajuste);
                                                        lectura_mbr->particion_3.estado[0]='1';
                                                        lectura_mbr->particion_3.inicio=inicio;
                                                        strcpy(lectura_mbr->particion_3.nombre,partt->nombre);
                                                        lectura_mbr->particion_3.tamano=partt->tam;
                                                        strcpy(lectura_mbr->particion_3.tipo,partt->tipo);
                                                        insertado=true;
                                                        printf("primera ocupada, todas vacias\n");
                                                    }
                                                }else{//cuarta particion ocupada, se verifica si hay espacio suficiente
                                                    if(lectura_mbr->particion_2.estado[0] == '0'){//2 particion libre
                                                        if(lectura_mbr->particion_1.estado[0] == '0'){ //1 tambien libre
                                                            espacio=lectura_mbr->particion_4.inicio - sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                        }else{//1 ocupada
                                                            espacio=lectura_mbr->particion_4.inicio - (lectura_mbr->particion_1.inicio+lectura_mbr->particion_1.tamano);
                                                            inicio=lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano;
                                                        }
                                                    }else{//particion 2 ocupada
                                                        espacio=lectura_mbr->particion_4.inicio - (lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano);
                                                        inicio=lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano;
                                                    }
                                                    printf("insertar particion 3, 4 ocupada\n");
                                                    if(espacio >= partt->tam){//si se puede insertar particion
                                                        strcpy(lectura_mbr->particion_3.ajuste,partt->ajuste);
                                                        lectura_mbr->particion_3.estado[0]='1';
                                                        lectura_mbr->particion_3.inicio=inicio;
                                                        strcpy(lectura_mbr->particion_3.nombre,partt->nombre);
                                                        lectura_mbr->particion_3.tamano=partt->tam;
                                                        strcpy(lectura_mbr->particion_3.tipo,partt->tipo);
                                                        insertado=true;
                                                    }
                                                }
                                            }
                                            printf("if 4=0\n");
                                            if(insertado){
                                                printf("verdadero\n");
                                            }else{
                                                printf("falso\n");
                                            }
                                            printf("4.estado %c\n",lectura_mbr->particion_4.estado[0]);
                                            if(lectura_mbr->particion_4.estado[0] == '0' && !insertado){//4 libre
                                                int espacio=0;
                                                inicio=0;
                                                if(lectura_mbr->particion_3.estado[0] == '0'){//3 particion libre
                                                    if(lectura_mbr->particion_2.estado[0] == '0'){//2libre
                                                        if(lectura_mbr->particion_1.estado[0] == '0'){//1 libre
                                                            espacio=lectura_mbr->tamano - sizeof(MBR);
                                                            inicio=sizeof(MBR);
                                                        }else{//1 ocupada
                                                            espacio=lectura_mbr->tamano - (lectura_mbr->particion_1.inicio+lectura_mbr->particion_1.tamano);
                                                            inicio=lectura_mbr->particion_1.inicio + lectura_mbr->particion_1.tamano;
                                                        }
                                                    }else{//2 ocupada
                                                        espacio=lectura_mbr->tamano - (lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano);
                                                        inicio=lectura_mbr->particion_2.inicio + lectura_mbr->particion_2.tamano;
                                                    }
                                                }else{//3 ocupada
                                                    espacio=lectura_mbr->tamano - (lectura_mbr->particion_3.inicio + lectura_mbr->particion_3.tamano);
                                                    inicio=lectura_mbr->particion_3.inicio + lectura_mbr->particion_3.tamano;
                                                }
                                                if(espacio >= partt->tam){//si se puede insertar particion
                                                    printf("insertar particion 4\n");
                                                    strcpy(lectura_mbr->particion_4.ajuste,partt->ajuste);
                                                    lectura_mbr->particion_4.estado[0]='1';
                                                    lectura_mbr->particion_4.inicio=inicio;
                                                    strcpy(lectura_mbr->particion_4.nombre,partt->nombre);
                                                    lectura_mbr->particion_4.tamano=partt->tam;
                                                    strcpy(lectura_mbr->particion_4.tipo,partt->tipo);
                                                    insertado=true;
                                                }
                                            }

                                       // }
                                    //}
                                }else{
                                    printf("No se puede crear particion, verifique parametro -unit\n");
                                }
                            }else{
                                printf("No se puede crear particion, verifique parametro -fit\n");
                            }
                            //se inserto particion
                            if(strcmp(partt->tipo,"e")==0){//ver que tipo se desea crear
                                if(extendida != 0){
                                    printf("Ya existe particion extendida en ese disco.\n");
                                }else{
                                    if(insertado){
                                        //agregar mbr al archivo
                                        archivo_disco=fopen(partt->path,"wb+");
                                        fseek(archivo_disco,0,SEEK_SET);
                                        fwrite(lectura_mbr,sizeof(MBR),1,archivo_disco);
                                        //insertar EBR
                                        fseek(archivo_disco,inicio,SEEK_SET);
                                        EBR *ebr_temp=(EBR*)malloc(sizeof(EBR));
                                        ebr_temp->estado[0]='0';
                                        strcpy(ebr_temp->ajuste,partt->ajuste);
                                        ebr_temp->inicio=inicio;
                                        ebr_temp->tamano=partt->tam;
                                        ebr_temp->siguiente=-1;
                                        strcpy(ebr_temp->nombre,partt->nombre);
                                        fwrite(ebr_temp,sizeof(EBR),1,archivo_disco);
                                        fclose(archivo_disco);
                                        printf("Particion extendida agregada correctamente\n");
                                        printf("Leer EBR\n");
                                        //abrir archivo
                                        archivo_disco=fopen(partt->path,"rb+");
                                        fseek(archivo_disco,0,SEEK_SET);
                                        fread(lectura_mbr,sizeof(MBR),1,archivo_disco);
                                        int i=Extendida(lectura_mbr);
                                        int inicioEBR=0;
                                        switch(i){
                                            case 0:
                                                printf("No existe particion extendida\n");
                                            break;
                                            case 1:
                                                printf("Particion extendida 1 %i\n",lectura_mbr->particion_1.inicio);
                                                inicioEBR=lectura_mbr->particion_1.inicio;
                                            break;
                                            case 2:
                                                printf("Particion extendida 2 %i\n",lectura_mbr->particion_2.inicio);
                                                inicioEBR=lectura_mbr->particion_2.inicio;
                                            break;
                                            case 3:
                                                printf("Particion extendida 3 %i\n",lectura_mbr->particion_3.inicio);
                                                inicioEBR=lectura_mbr->particion_3.inicio;
                                            break;
                                            case 4:
                                                printf("Particion extendida 4 %i\n",lectura_mbr->particion_4.inicio);
                                                inicioEBR=lectura_mbr->particion_4.inicio;

                                            break;
                                        default:
                                            break;
                                        }

                                        EBR *lectura_ebr;
                                        fseek(archivo_disco,inicioEBR,SEEK_SET);
                                        fread(lectura_ebr,sizeof(EBR),1,archivo_disco);
                                        printf("estado %c\n",lectura_ebr->estado[0]);
                                        printf("ajuste %c\n",lectura_ebr->ajuste[0]);
                                        printf("inicio %i\n",lectura_ebr->inicio);
                                        printf("tam %i\n",lectura_ebr->tamano);
                                        printf("siguiente %i\n",lectura_ebr->siguiente);
                                        printf("nombre %s\n",lectura_ebr->nombre);
                                        fclose(archivo_disco);
                                    }
                                }
                            }else if(strcmp(partt->tipo,"p")==0){
                                if(insertado){
                                    //agregar mbr al archivo
                                    archivo_disco=fopen(partt->path,"wb+");
                                    fseek(archivo_disco,0,SEEK_SET);
                                    fwrite(lectura_mbr,sizeof(MBR),1,archivo_disco);
                                    fclose(archivo_disco);
                                    insertado=false;
                                    free(lectura_mbr);
                                    printf("Particion primaria agregada correctamente\n");
                                }
                            }else if(strcmp(partt->tipo,"l")==0){
                                printf("logica\n");
                            }

                        }else{
                            printf("No se puede crear particion, verifique parametro -size\n");
                        }
                    }

                }
            }
        }else{//parametro name viene vacio
            printf("No se puede crear particion, verifique parametro -name\n");
        }
    }else{//disco no existe
        printf("Disco no existe, verifique datos\n");
    }
}

int main(int argc, char *argv[])
{
    char comando[10];//variable que guarda el comando a ejecutar
    char linea[200];//cadena con los parametros a utilizar por el comando


    while(true){
        fflush(stdin);
        for(int i=0;i<=10;i++){
            comando[i]=' ';
        }
        for(int i=0;i<=200;i++){
            linea[i]=' ';
        }
        printf("Ingrese comando a ejecutar\n");
        //guardar en variable
        scanf("%s",&comando);
        //pasar a minusculas comando
        for(int i=0;i<10;i++){
            comando[i]=tolower(comando[i]);
        }
        //parametros
        scanf("%[^\n]",&linea);

        //verifica que comando viene para decidir que ejecutar
        printf(" %s\n",comando);
        if(strcmp(comando,"exit")==0){
                return 0;
        }else if(strcmp(comando,"mkdisk")==0){
            Mkdisk(linea);
        }else if(strcmp(comando,"rmdisk")==0){//elimina disco en la direccion especificada
            int i=EliminarDisco(linea);
        }else if(strcmp(comando,"fdisk")==0){
            //separa linea de parametros y devuelve un objeto Part con los datos almacenados
            Fdisk(linea);

        }

    }


    return 0;
    QCoreApplication a(argc, argv);
    return a.exec();
}
