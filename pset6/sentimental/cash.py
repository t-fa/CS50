from cs50 import get_float

while True:
    change = get_float("Change owed: ")
    if change > 0:
        break

coins = 0
c = round(change * 100)

while c > 0:
    if c > 24:
        c -= 25
        coins += 1
    if c < 25 and c > 9:
        c -= 10
        coins += 1
    if c < 10 and c > 4:
        c -= 5
        coins += 1
    if c < 5 and c > 0:
        c -= 1
        coins += 1

print(coins)
