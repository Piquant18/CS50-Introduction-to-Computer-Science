// Simulate genetic inheritance of blood type

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
    person *new_person = malloc(sizeof(person));  // Asigna memoria para una nueva persona

    // Si quedan generaciones por crear (generations > 1)
    if (generations > 1)
    {
        // Crea dos nuevos padres llamando a create_family recursivamente
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // Asigna los punteros a los padres
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // Asigna alelos aleatorios de los padres
        new_person->alleles[0] = parent0->alleles[rand() % 2];
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }
    // Si ya no quedan generaciones por crear (generations == 1)
    else
    {
        // Los padres se establecen como NULL porque es la generación más antigua
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        // Asigna alelos aleatorios a esta persona
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;  // Devuelve el puntero a la nueva persona creada
}

// Función que imprime la información de cada persona de la familia
void print_family(person *p, int generation)
{
    // Si la persona es NULL, no hay nada que imprimir
    if (p == NULL)
    {
        return;
    }

    // Imprimir la sangría de acuerdo a la generación
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Imprimir la generación y los alelos
    if (generation == 0)
    {
        printf("Generación inicial con alelos %c%c\n", p->alleles[0], p->alleles[1]);
    }
    else
    {
        printf("Hijo de generación %i con alelos %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // Llamar recursivamente para imprimir los padres
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Función que libera la memoria asignada a cada persona en la familia
void free_family(person *p)
{
    // Si la persona es NULL, no hay nada que liberar
    if (p == NULL)
    {
        return;
    }

    // Llamamos recursivamente para liberar a los padres
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Liberamos la memoria de la persona actual
    free(p);
}

// Función que devuelve un alelo aleatorio ('A', 'B' o 'O')
char random_allele(void)
{
    int r = rand() % 3; // Número aleatorio entre 0 y 2
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
