from cs50 import get_string
import sys

while True:
    if len(sys.argv) != 2:
        print("Error: Must provide key as command line argument.")
        sys.exit(1)
    plaintext = get_string("plaintext: ")
    if plaintext:
        break

key = int(sys.argv[1])

print("ciphertext: ", end="")
for c in plaintext:
    if c.isalpha() == True:
        if c.isupper() == True:
            ascii = ord(c)
            ascii -= 65
            c = (ascii + key) % 26
            c += 65
            c = chr(c)
            print(c, end="")
        if c.islower() == True:
            ascii = ord(c)
            ascii -= 97
            c = (ascii + key) % 26
            c += 97
            c = chr(c)
            print(c, end="")
    else:
        print(c, end="")
print()
