from cs50 import get_float

def main():
    coins = [25, 10, 5, 1]  # en centavos
    total_coins = 0

    while True:
        change = get_float("Change owed: ")
        if change >= 0:
            break

    cents = round(change * 100)  # Redondear para evitar problemas de punto flotante

    for coin in coins:
        while cents >= coin:  
            cents -= coin 
            total_coins += 1

    print(total_coins)

if __name__ == "__main__":
    main() 
