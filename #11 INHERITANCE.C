#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct person
{
    struct person *parents[2];      //GUARDA LOS PADRES DE CADA PERSONA
    char alleles[2];            //GUARDA LOS TIPOS SANGUINEOS A,B Y O
} person;

const int GENERATIONS = 3;          //CREA CONSTANTES GENERACIONES BOY FATHER GRANDFATHER
const int INDENT_LENGTH = 4;        //CONSTANTE POR CADA GENERACION SON 4 TIPOS DE SANGRE ABUELO 2DA 2X4= 8 TIPOS

person *create_family(int generations);         //CREA Y CONECTA LAS FAMILIAS
void print_family(person *p, int generation);       //IMPRIME LOS TIPOS DE SANGRE
void free_family(person *p);            //LIBERA LA MEMORIA NECESARIA PARA EVITAR FUGAS
char random_allele();           //GENERA TIPOS DE SANGRE ALEATORIO PARA LA 1MRA GENERACION

int main(void)
{
    srand(time(0));     //INICIA GENERADOR DE NUMEROS ALEATORIOS
    person *p = create_family(GENERATIONS);     //LLAMA A create_family PARA CONSTRUIR UNA FAMILIA DE GENERATIONS (CREA LA FAMILIA)
    print_family(p, 0);         //IMPRIME LA ESTRUCTURA DE LA FAMILIA INDICA EL TIPO DE SANGRE
    free_family(p);     //LIBERA LA MEMORIA QUE USO PARA CREAR LA FAMILIA
}

person *create_family(int generations)
{
    person *new_person = malloc(sizeof(person));  //ASIGNA MEMORIA PARA UNA NUEVA PERSONA

    //SI QUEDAN GENERACIONES POR CREAR
    if (generations > 1)
    {
        //CREA DOS NUEVOS PADRES LLAMANDO A CREATE_FAMILY RECURSIVAMENTE
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        //ASIGNA LOS PUNTEROS A LOS PADRES
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        //ASIGNA ALELOS ALEATORIOS DE LOS PADRES
        new_person->alleles[0] = parent0->alleles[rand() % 2];
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }
    //SI YA NO QUEDAN GENERACIONES POR CREAR
    else
    {
        //LOS PADRES SE ESTABLECEN COMO NULL PORQUE ES LA GENERACIÓN MÁS ANTIGUA
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        //ASIGNA ALELOS ALEATORIOS A ESTA PERSONA
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;  //DEVUELVE EL PUNTERO A LA NUEVA PERSONA CREADA
}

//FUNCIÓN QUE IMPRIME LA INFORMACIÓN DE CADA PERSONA DE LA FAMILIA
void print_family(person *p, int generation)
{
    //SI LA PERSONA ES NULL, NO HAY NADA QUE IMPRIMIR
    if (p == NULL)
    {
        return;
    }

    //IMPRIMIR LA SANGRÍA DE ACUERDO A LA GENERACIÓN
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    //IMPRIMIR LA GENERACIÓN Y LOS ALELOS
    if (generation == 0)
    {
        printf("Generación inicial con alelos %c%c\n", p->alleles[0], p->alleles[1]);
    }
    else
    {
        printf("Hijo de generación %i con alelos %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    //LLAMAR RECURSIVAMENTE PARA IMPRIMIR LOS PADRES
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

//FUNCIÓN QUE LIBERA LA MEMORIA ASIGNADA A CADA PERSONA EN LA FAMILIA
void free_family(person *p)
{
    //SI LA PERSONA ES NULL, NO HAY NADA QUE LIBERAR
    if (p == NULL)
    {
        return;
    }

    //LLAMAMOS RECURSIVAMENTE PARA LIBERAR A LOS PADRES
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    //LIBERAMOS LA MEMORIA DE LA PERSONA ACTUAL
    free(p);
}

//FUNCIÓN QUE DEVUELVE UN ALELO ALEATORIO ('A', 'B' O 'O')
char random_allele(void)
{
    int r = rand() % 3; //NÚMERO ALEATORIO ENTRE 0 Y 2
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
