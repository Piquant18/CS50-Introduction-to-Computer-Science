import csv
import sys

def main():

    if len(sys.argv) != 3:
        print("Uso: python dna.py database.csv sequence.txt")
        sys.exit(1)

    database = []
    with open(sys.argv[1], newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            for key in row:
                if key != "name":
                    row[key] = int(row[key])
            database.append(row)                    

    with open(sys.argv[2], 'r') as file:
        sequence = file.read().strip()

    str_keys = list(database[0].keys())[1:]

    str_counts = {str_: longest_match(sequence, str_) for str_ in str_keys}

    for person in database:
        if all(person[str_] == str_counts[str_] for str_ in str_keys):
            print(person["name"])
            return
    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    for i in range(sequence_length):

        count = 0

        while sequence[i + count * subsequence_length: i + (count + 1) * subsequence_length] == subsequence:
            count += 1
        longest_run = max(longest_run, count)

    return longest_run

if __name__ == "__main__":
    main()
