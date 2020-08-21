# Prints double half pyramids

from cs50 import get_int

# prompt the user to enter the height of the pyramids, from 1 to 8. Re prompt if it goes out of bounds
while (True):
    h = get_int('Height: ')  # h = int(input()) is another way of inputing integers
    if 1 <= h <= 8:
        break

# print the pyramids
for i in range(h):  # loop for a row ex: 0123
    for j in range(h - (i + 1)):  # ex: h=4 i=0, 4-1=3 , 012 (spaces)
        print(' ', end='')
    for k in range(i + 1):  # prints the # of the left
        print('#', end='')
    print('  ', end='')  # prints 2 spaces
    for l in range(i + 1):  # prints the # of the right
        print('#', end='')
    print()