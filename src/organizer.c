#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "organizer.h"

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
            printf("[FILE ] %s\n", entry->d_name);
        }
        
    }

    // 3. Cierro el directorio
    closedir(dir);
}