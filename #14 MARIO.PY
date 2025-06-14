def get_height():
    while True:
        try:
            height = int(input("Height: "))

            if 1<= height <= 8:
                return height

            else:
                print("Please enter a number between 1 and 8.")

        except ValueError:
            print("Invalid input, Please enter a number between 1 and 8.")

def print_pyramid(height):
    for i in range(1, height + 1):
        print(" " * (height - i), end="")
        print("#" * i)

def main():
    height = get_height()
    print_pyramid(height)

if __name__ == "__main__":
    main()
