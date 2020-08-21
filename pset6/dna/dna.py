# Program to check for a match of dna

import csv
import sys


def main():
    fcsv = sys.argv[1]  # data csv file
    ftxt = sys.argv[2]  # sequence txt file
    data, seq = read_files(fcsv, ftxt)  # call read_files() function
    values = longest_run(data, seq)  # call longest_run() function and send sequence
    compare(data, values)  # call compare() function, sending a list or something with the longest run of each str


def read_files(data, seq):  # Open csv file and dna sequence, and read contents into memory
    # open csv file and read
    with open(data, "r") as csvfile:
        data_read = csv.DictReader(csvfile)
        ord_data = list(data_read)

    # open txt file and read
    f = open(seq, "r")
    seq_read = f.read()

    # close files
    f.close()
    csvfile.close()

    return ord_data, seq_read


def longest_run(data, seq):  # for each STR, compute the longest run of consecutive repeats in the dna
    header = list(data[0].keys())  # stores the str of the STRs
    values = [None] * (len(header) - 1)  # header - 1 because I don't want to count the name
    check = [None]  # will store all the occurences in the dna sequence
    for j in range(1, len(header)):  # repeat depending on how mant STRs you're comparing
        strdna = header[j]  # stores one of the STR
        x = 0  # variable that'll store ocurrences
        for i in range(len(seq)):  # repeats until the str sequence is finished
            if seq[i: i + len(strdna)] == strdna:  # if it mathces the str, adds one
                x += 1
            elif check[0] == None and x != 0 and seq[i: i + len(strdna)] != strdna and seq[i - len(strdna): i] == strdna:
                check[0] = x
                x = 0
            elif seq[i: i + len(strdna)] != strdna and x != 0 and seq[i - len(strdna): i] == strdna:  # if the past was match, add x to list
                check.append(x)
                x = 0
            else:
                continue
        maxi = check[0]
        for k in range(1, len(check)):  # checks the max ocurrences of str
            if check[k] > maxi:
                maxi = check[k]
        values[j - 1] = maxi  # stores the result in a list
        check = [None]
    return values


def compare(data, values):  # compare the str counts to the ones on the csv file
    for i in range(len(data)):  # the number of people being evaluated
        tmp = list(data[i].values())  # stores the results of occurrances of STR
        for j in range(len(values)):  # repeats num of STRs there are
            if int(tmp[j + 1]) == values[j]:  # compares the result to the person
                val = True
            else:
                val = False
                break
        if val == True:
            print(f"{tmp[0]}")
            return 1
    print("No match")
    return 0


if __name__ == "__main__":
    main()