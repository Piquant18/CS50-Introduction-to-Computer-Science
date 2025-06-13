#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512      //CONSTANTE DE 512 BYTES

int main(int argc, char *argv[])        //ALMACENA EL NUMERO DE DATOS DE EL COMANDO EJECUTADO //CONTIENE LOS ARGUMENTOS DEL COMANDO EJECUTADO
{
    if (argc != 2)
    {                                           //COMPRUEBA QUE EL CODIGO SE ALLA EJECUTADO CORRECTAMENTE O MARCA ERROR
        printf("usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)

    {                                               //INTENTA LLAMAR ARGV 1 EN MODO DE LECTURA "r" read
        printf("Could not open %s\n", argv[1]);     //OSEA EL NOMBRE DE LA IMAGEN SI NO LO ENCUENTRA MARCA ERROR
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];     //EL ARREGLO DE 512 BYTES DE ARRIBA
    FILE *jpeg = NULL;          //PUNTERO SE USARA PARA ESCRIBIR LA IMAGEN RECUPERADA
    int jpeg_count = 0;         //CONTADOR DE CUANTOS IMAGENES HAY CON ESE NOMBRE Y LOS GUARDA ASI 000.JPJ, 001.JPJ

    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&      //VERIFICA LOS ARCHIVOS JPEG LOS PRIMERO DIGITOS COMO:
            (buffer[3] & 0xf0) == 0xe0)                                         // 0xffd8ffe0 o 0xffd8ffe1
        {

            if (jpeg != NULL)
            {                       //SI YA ENCONTRO UN JPEG SE CIERRA
                fclose(jpeg);
            }

            char filename[8];
            sprintf(filename, "%03d.jpg", jpeg_count);      //SE CREA UN ARCHIVO USANDO EL jpeg_count
            jpeg = fopen(filename, "w");                    //SE ABRE USANDO EL MODO DE LECTURA "w" write
            jpeg_count++;

        }

        if (jpeg != NULL)                   // //SI NO ENCONTRO UN JPEG, BUFFER SE ESCRIBE EN ESE ARCHIVO
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, jpeg);
        }
    }

    if (jpeg != NULL)
    {
        fclose(jpeg);       //SI YA SE ESTA ESCRIBIENDO EL JPEG SE CIERRA EL ARCHIVO CARD
    }
    fclose(card);
    return 0;
}
