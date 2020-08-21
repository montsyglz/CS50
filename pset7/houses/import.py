# Houses sql

from cs50 import SQL
import csv
import sys

db = SQL("sqlite:///students.db")

# check command line argument
if len(sys.argv) != 2:
    print(sys.argc)
    print("Add a csv file")
    sys.exit()

# open csv file in dict
with open(sys.argv[1], "r") as students:

    # create dict reader
    reader = csv.DictReader(students)

    # iterate over csv file
    for row in reader:

        # parse name per row
        names = row["name"].split(" ")
        if len(names) == 2:
            names.insert(1, None)

        # insert students into students table
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                   names[0], names[1], names[2], row["house"], row["birth"])