#include "helpers.h"
#include <math.h>

void grayscale(int height, int width, RGBTRIPLE image[height][width])
    
{
    for (int i = 0; i < height; i++)    //BUCLE RECORRE CADA FILA DE LA IMAGEN
    {
        for (int j = 0; j < width; j++)     //BLUCE RECORRE CADA FILA
        {
            int red = image[i][j].rgbtRed;      //OBTIENE LOS COLORES DE LOS BUCLES
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int average = round((red + green + blue) / 3.0);        //SUMA LOS COLORES RGB LOS + Y LOS /3 PARA OBTENER EL RESULTADO
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;

            //SUMA LOS RGB CONVIRTIENDOLO EN UN TONO GRIS
        }
    }
}

void sepia(int height, int width, RGBTRIPLE image[height][width])
        //DIMENCIONES DE LA IMAG. //MATRIS REDGREENBLUE
{
    for (int i = 0; i < height; i++)    //BUCLE FOR RECORRE CADA FILA DE LA IMAGEN
    {
        for (int j = 0; j < width; j++)      //BLUCE RECORRE CADA FILA
        {
            int originalRed = image[i][j].rgbtRed;       //OBTIENE LOS COLORES ORIGINALES DE LOS BUCLES
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);      //FORMULA PARA CONVERTIR LOS RGB
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);    //EN COLOR SEPIA (FORMULA)
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {                                   //NO PERMITE QUE LOS COLORES PASEN DE 255 ("FORMULA")
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;     //ACTUALIZA LOS RGB CON EL TOQUE DE SEPIA
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
        //DIMENCIONES DE LA IMAG. //MATRIS REDGREENBLUE
{
    for (int i = 0; i < height; i++)        //BUCLE RECORRE CADA FILA DE LA IMAGEN
    {
        for (int j = 0; j < width / 2; j++)     //BLUCE RECORRE LA MITAD DE CADA FILA
        {
            RGBTRIPLE temp = image[i][j];       //ALMACENA EL PIXEL ACTUAL EN LA VARIABLE TEMP
            image[i][j] = image[i][width - 1 - j];      //ASIGNA EL MISMO PIXEL A EL OTRO LADO
            image[i][width - 1 - j] = temp;     //COLOCA EL MISMO PIXEL A EL OTRO LADO
        }
    }
}

void blur(int height, int width, RGBTRIPLE image[height][width])
        //DIMENCIONES DE LA IMAG. //MATRIS REDGREENBLUE
{
    RGBTRIPLE temp[height][width];      //CREAR MATRIZ TEMPORAL

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)         //COPIAR IMAGEN A LA MATRIZ TEMPORAL    //DUPLICAR
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)    //RECORREN CADA FILA
    {
        for (int j = 0; j < width; j++)
        {

            int totalRed = 0, totalGreen = 0, totalBlue = 0;        //CALCULAN LOS VALORES DE LOS PIXELES VECINOS Y INICIA EN 0
            int count = 0;

            for (int di = -1; di <= 1; di++)        //RECORREN LOS PIXELES VECINOS
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int newRow = i + di;        //PIXEL ACTUAL MAS EL PIXEL VECINO
                    int newCol = j + dj;

                    if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width)        //VERIFICA QUE LOS PIXELES VECINOS SI
                                                                                                //PERTENEZCAN A LA IMAGEN
                    {
                        totalRed += temp[newRow][newCol].rgbtRed;
                        totalGreen += temp[newRow][newCol].rgbtGreen;       //SUMAN LOS COLORES TOTALES CON SUS VECINOS
                        totalBlue += temp[newRow][newCol].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = round(totalRed / (float) count);
            image[i][j].rgbtGreen = round(totalGreen / (float) count);      //DIVIDE Y APROXIMA EL COLOR PARA EL PIXEL OBTENIENDO UN
            image[i][j].rgbtBlue = round(totalBlue / (float) count);        //PROMEDIO EXACTO
        }
    }
}
