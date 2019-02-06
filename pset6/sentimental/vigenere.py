from cs50 import get_string
import sys

while True:
    if len(sys.argv) != 2:
        print("Error! Specify key in command line argument.")
        sys.exit(1)
    if not sys.argv[1].isalpha():
        print("Error: Command line argument must be alphabetical.")
        sys.exit(1)
    plaintext = get_string("plaintext: ")
    if plaintext:
        break

print("ciphertext: ", end="")
key = sys.argv[1]
key_index = 0

for c in plaintext:

    # iterating through cipher
    key_index_wrap = key_index % len(key)
    k = key[key_index_wrap]
    if k.isupper() == True:
        k = ord(k)
        k -= 65
    elif k.islower() == True:
        k = ord(k)
        k -= 97

    # plaintext ciphering
    if c.isalpha() == True:
        if c.isupper() == True:
            ascii = ord(c)
            ascii -= 65
            c = (ascii + k) % 26
            c += 65
            c = chr(c)
            key_index += 1
            print(c, end="")
        if c.islower() == True:
            ascii = ord(c)
            ascii -= 97
            c = (ascii + k) % 26
            c += 97
            c = chr(c)
            key_index += 1
            print(c, end="")
    else:
        print(c, end="")

print()
