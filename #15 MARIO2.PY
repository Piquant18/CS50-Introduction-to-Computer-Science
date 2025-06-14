def obtener_altura():
    while True:
        try:
            altura = int(input("Altura: "))

            if 1 <= altura <= 8:
                return altura
            else:
                print("Ingresar un numero del 1 al 8")

        except ValueError:
            print("Entrada no valida, ingresar un numero del 1 al 8")

def imprimir_piramide(altura):
    for i in range(1, altura + 1):
        print(" " * (altura - i), end="")
        print("#" * i, end="")
        print("  ", end="")
        print("#" * i)

def main():
    altura = obtener_altura()
    imprimir_piramide(altura)

if __name__ == "__main__":
    main()
