# Program that determines whether a provided credit card number is valid according to Luhn’s algorithm

from cs50 import get_int
import math


def main():
    card = get_int('n: ')  # input the card number
    length = len(str(card))  # len function doesn't work on integers
    dig = algorithm(card, length)  # calls the function of the luhn's alg, returns first two dig or 0
    card_type(card, dig, length)  # prints the card or invalids


def algorithm(card, length):  # check algorithm
    # sum of *2
    half = int(length / 2)  # int rounds the number down
    div = 100  # remainder of 100 gives you the 2nd to last digit
    add = 0  # initialize de sum
    for i in range(half):
        n = int((card % div) / (div / 10)) * 2  # this operation gives me a specific digit, depending on the div
        crd1 = n / 2  # at the end of the loop, it'll be one of the first two digits
        n2 = 0  # only used when the dig*2 has two digits
        if n >= 10:  # if it has two digits
            n2 = int(n % 10)  # second digit
            n = int(n / 10)  # first digit
        add += n + n2
        n = 0  # i think this isn't needed
        div *= 100  # so the next loop ot takes the digit two spaces after

    # sum of the rest
    half = math.ceil(length / 2)  # ceil rounds the number up
    div = 10  # changes it to take the other missing digits
    for i in range(half):
        n = int((card % div) / (div / 10))
        add += n
        div *= 100
        crd2 = n  # one of the two first digits of the card
    if add % 10 == 0:  # means it is valid, it ends in 0
        if length % 2 == 0:  # if the card is even
            check = (crd1 * 10) + crd2
        else:  # if the card is odd
            check = crd1 + (crd2 * 10)
    else:  # not valid so value is 0
        check = 0

    return check


def card_type(card, dig, length):
    # check the length and two first numbers to see which card it is
    if length == 15 and dig in [34, 37]:
        print('AMEX')
    elif length == 16 and dig in [51, 52, 53, 54, 55]:
        print('MASTERCARD')
    elif length in [13, 16] and int(dig / 10) == 4:
        print('VISA')
    else:
        print('INVALID')


if __name__ == "__main__":
    main()