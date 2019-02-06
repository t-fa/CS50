from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height >= 0 and height < 24:
        break
    else:
        print("Height must be a positive integer less than 24.")

width = height + 1
spaces = width - 2
hashes = width - spaces

for i in range(height):
    print(" " * spaces, end="")
    spaces -= 1
    print("#" * hashes, end="")
    hashes += 1
    print()
