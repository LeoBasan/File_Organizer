#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "organizer.h"

// Funcion para obtener extension del archivo
static char* get_extension(char* fileName){
    char *dot;

    // Busco el ultimo .
    dot = strrchr(fileName, '.');
    // Si no encuentro un .
    if(dot == NULL){
        return NULL;
    }
    // Si el . es el primer caracter (.env, .gitignore)
    if(dot == fileName){
        return NULL;
    }
    // Si el . es el ultimo caracter (file.)
    if(*(dot+1) == '\0'){
        return NULL;
    }
    // Sino devuelvo lo que esta despues del punto
    return dot+1;
}

// Funcion para mover archivos
static void handle_file(const char *base_path, const char *filename){
    char *ext = get_extension((char*)filename);

    if(!ext){
        return;
    }
    // Armo el path del destino
    char target_dir[1024];
    snprintf(target_dir, sizeof(target_dir), "%s/%s", base_path, ext);

    struct stat st;
    if(stat(target_dir, &st) == -1){
        // no existe la carpeta asi que la creo
        if(mkdir(target_dir) == -1){
            perror("mkdir");
            return;
        } 
    }

    char source_path[1024];
    snprintf(source_path, sizeof(source_path), "%s/%s", base_path, filename);

    // Como no se si el target dir es mas grande verifico el retorno y lo salteo sino
    char target_path[1024];
    int written = snprintf(target_path, sizeof(target_path), "%s/%s", target_dir, filename);
    if(written < 0 || written>= (int)sizeof(target_path)){
        fprintf(stderr, "Ruta demasiado larga, se omite el archivo\n");
        return;
    }

    if(rename(source_path, target_path) == -1){
        perror("rename");
    }
    //printf("Archivo %s tiene extension %s\n", filename, ext);
}

void organize_files(const char *path){
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    char full_path[1024];

    printf("Organizando carpetas: %s\n", path);

    // 1. Primero abro el directorio
    dir = opendir(path);
    if(dir == NULL){
        perror("No se pudo abrir el directorio");
        return;
    }

    // 2. Leo cada entrada
    while((entry = readdir(dir)) != NULL){
        if((strcmp(entry->d_name,".")==0)||(strcmp(entry->d_name,"..")==0)){
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if(stat(full_path, &info) == -1){
            perror("stat error");
            continue;
        }
        if(S_ISDIR(info.st_mode)){
            printf("[DIR ] %s\n", entry->d_name);
        }else if(S_ISREG(info.st_mode)){
            handle_file(path, entry->d_name);
        }
        
    }

    // printf("Ext: %s\n", get_extension("foto.png"));

    // 3. Cierro el directorio
    closedir(dir);
}