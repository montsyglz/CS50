# Print students from given house

#IMP NOTE TO SELF don't know why in check50, with range(0, len(argv[1])) works, but here, I have to
#use range(0, len(argv[1]), 5), because otherwise it prints five times

from cs50 import SQL
import sys

db = SQL("sqlite:///students.db")

# check command line arguments
if len(sys.argv) != 2:
    print("Enter house name")
    sys.exit()

# query database for the house selected & sorted alph by last names, then by first name
students = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])

for i in range(0, len(students)):
    # print each students names and birth year
    p = students[i]
    first = p["first"]
    last = p["last"]
    birth = p["birth"]
    # if they have no middle name
    if p["middle"] == None:
        print(f"{first} {last}, born {birth}")
    else:
        mid = p["middle"]
        print(f"{first} {mid} {last}, born {birth}")