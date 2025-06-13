#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool only_digits(string s);              // SOLO NUMEROS
int string_to_int(string s);             // NUMERO ENTERO
char rotate(char c, int k);             // ROTACION CODIGO CESAR

int main(int argc, string argv[])     //ALMACENA EL NOMBRE DEL CODIGO Y EL NO. DE ROTACION

{
    if (argc != 2)                  //COMO AQUI ARRIBA SON 2 NOMBRE Y NO.
    {
        printf("usage ./caesar key\n");     //PARA INDICAR CUAL VA A SER LA CLAVE
        return 1;
    }
    if (!only_digits(argv[1]))                //FUNCION DE ARRIBA SOLO DIGITOS NO.
    {
        printf("usage ./caesar key\n");        // PIDE NUEVAMENTE INDICAR CUAL VA A SER LA CLAVE
        return 1;
    }
    int key = string_to_int(argv[1]);               //CONVIERTE LA CLAVE EN UN NUMERO ENTERO Y LO ALMACENA EN KEY
    string plaintext = get_string("plaintext: ");           //PIDE LAS LETRAS A DEZIFRAR EJM: A SERIA B CON CLAVE 1
    printf("ciphertext: ");                             //ESTE SOLO LO IMPRIME OSEA B CON LO DE ARRIBA
    for (int i = 0, n = strlen(plaintext); i < n; i++)          //BUCLE PARA HACER LAS ROTACIONES
    {
        printf("%c", rotate(plaintext[i], key));        //LLAMA ROTATE PARA CON LA CLAVE DEZIFRAR EL CODIGO
    }
    printf("\n");
    return 0;
}
bool only_digits(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return false;
        }
    }
    return true;
}
int string_to_int(string s)
{
    int result = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        result = result * 10 + (s[i] - '0');
    }
    return result;
}
char rotate(char c, int k)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (c - 'A' + k) % 26 + 'A';
    }
    else if (c >= 'a' && c <= 'z')
    {
        return (c - 'a' + k) % 26 + 'a';
    }
    else
    {
        return c;
    }
}
