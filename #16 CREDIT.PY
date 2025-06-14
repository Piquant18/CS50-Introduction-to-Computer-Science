from cs50 import get_string

def luhn_algoritmo(numero_targeta):
    total = 0
    invertir_digitos = numero_targeta[::-1]

    for i, digito in enumerate(invertir_digitos):
        n = int(digito)

        if i % 2 == 1:
            n *= 2
            if n > 9:
                n -= 9

        total += n

    return total % 10 == 0

def tipo_tarjeta(numero_targeta):
    longitud = len(numero_targeta)
    primeros_dos_numeros = int(numero_targeta[:2])
    primer_un_numero = int(numero_targeta[0])

    if longitud == 15 and primeros_dos_numeros in [34, 37]:
        return "AMEX"

    if longitud == 16 and 51 <= primeros_dos_numeros <= 55:
        return "MASTERCARD"

    if longitud in [13, 16] and primer_un_numero == 4:
        return "VISA"

    return "INVALIDO"

def main():
    numero_targeta = get_string("Numero de tarjeta: ")

    if not numero_targeta.isdigit():
        print("INVALIDO")
        return

    if luhn_algoritmo(numero_targeta):
        print(tipo_tarjeta(numero_targeta))
    else:
        print("INVALIDO")

if __name__ == "__main__":
    main()
