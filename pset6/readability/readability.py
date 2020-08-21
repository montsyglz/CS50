# Program in python that computes the approximate grade level needed to comprehend some text

import re  # regex


def main():
    # input
    text = input('Text: ')

    # call function that counts sentences
    nsen = sentences(text)

    # call function that counts words and letters
    nwords, nletters = words_letters(text)

    # call function that prints the grade
    grade(nsen, nwords, nletters)


def sentences(text):  # counts the number of sentences
    n = 0  # variable that'll store the number of sentences
    for sen in re.split(r'[\.!?]', text):  # checks where there are .!? and splits
        n += 1
    return n - 1  # the function re.split returns one more


def words_letters(text):  # checks number of words and letters
    nw = 0  # counter of words
    nl = 0  # cuonter of letters
    upper = [x for x in range(65, 91)]  # list with values of A-Z ascii
    lower = [y for y in range(97, 123)]  # list with values of a-z ascii
    for word in text.split():  # splits the text where there are spaces
        nw += 1
        for i in range(len(word)):  # using the word that was split
            if ord(word[i]) in upper:  # if the char is upper case
                nl += 1
            elif ord(word[i]) in lower:  # if the char is lower case
                nl += 1
    return nw, nl  # returns both counters


def grade(s, w, l):
    L = (l * 100) / w
    S = (s * 100) / w
    index = (0.0588 * L) - (0.296 * S) - 15.8  # Luhn's Algorithm
    if index < 1:
        print('Before Grade 1')
    elif index > 16:
        print('Grade 16+')
    else:
        print(f'Grade {round(index)}')


if __name__ == "__main__":
    main()