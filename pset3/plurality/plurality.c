#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
    return 0;
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int index_winner = 0; // the index of the latest with the highest # of votes
    int num_winners = 1; // the number of winners

    // Array that stores the names of the winners
    string name_winner[candidate_count];
    name_winner[0] = candidates[0].name;

    // Loop that checks the winner (linear search)
    for (int i = 1; i < candidate_count; i++)
    {
        if (candidates[index_winner].votes < candidates[i].votes)
        {
            name_winner[0] = candidates[i].name; // Name added to winners Array
            num_winners = 1;
            index_winner = i;
        }
        else if (candidates[index_winner].votes == candidates[i].votes)
        {
            num_winners++; // If it enters the condition, it means there is more than one winner
            name_winner[num_winners - 1] = candidates[i].name; // Name added to winners Array
        }
        else
        {
            continue;
        }
    }

    // Repeats depending on the number of winners
    for (int j = 0; j < num_winners; j++)
    {
        printf("%s\n", name_winner[j]); // Prints the name
    }
    return;
}

