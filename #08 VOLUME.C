#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int HEADER_SIZE = 44;

int main(int agrc, char *argv[])
{
    if (agrc != 4)
    {
        printf("Usar: ./12_volume input.wav output.wav factor\n");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("No se pudo abrir el archivo de entrada.\n");
        return 1;
    }
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("No se pudo abrir el archivo de salida.\n");
        fclose(input);
        return 1;
    }
    float factor = atof(argv[3]);       //(atof) convierte el tercer argumento de cadena a numero
                                                        //sizeof devueve el tamano de bytes en una variables
    uint8_t header[HEADER_SIZE];                        //Declara un arreglo de 44 bytes (uint8_t es 1 byte) solo 1 byte por que es para copiarlo no para modificarlo
    fread(header, sizeof(header), 1, input);            //Lee 44 bytes del archivo y los guarda en header.
    fwrite(header, sizeof(header), 1, output);          //Copia esos 44 bytes al nuevo archivo sin modificarlos.
                                                        // el 1 solo es para indicar cuantas veces lee y escribe
    int16_t buffer;                                         //Declara una variable de 16 bits (2 bytes). esto por que se guarda el audio en muestras de 16 bits (2 bites)
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        buffer *= factor;                                   // multimplica la muestra por el factos
        fwrite(&buffer, sizeof(int16_t), 1, output);        //guarda los bytes modificados
    }
    fclose(input);
    fclose(output);
    return 0;
}

// 8 bits (1 byte)
