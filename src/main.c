#include <stdio.h>
#include <stdlib.h>

#include "organizer.h"

int main(int argc, char *argv[]){
    // argc = cantidad de argumentos
    // argv = arreglo de strings

    if(argc != 2){
        printf("Uso: file-organizer <ruta_carpeta>\n");
        return EXIT_FAILURE;
    }

    organize_files(argv[1]);
    
    return EXIT_SUCCESS;
}