#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) // checked ###############
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) // checked ##################
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) // checked
{
    int size = (candidate_count * (candidate_count - 1) / 2);
    pair_count = 0;

    for (int i = 0; i < candidate_count - 1; i++) // condition de < candidate_count or size or pair_count
    {
        for (int j = i + 1; j < candidate_count; j++) // this is set like this so it analizes over the zero diagonal
        {
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else
            {
                continue;
            }
        }
    }
    return;
}

void merge(int low, int mid, int high) // checked
{
    // i = left, j = right, k = new array index
    int i = low, j = mid + 1, k = 0;
    int size = high - low + 1; // plus 1 because the indexes start in 0
    pair merged_array[size]; // since they're indexes, to get the size you do that operation
    while (i <= mid && j <= high) // maybe <=
    {
        if (preferences[pairs[i].winner][pairs[i].loser] > preferences[pairs[j].winner][pairs[j].loser])
        {
            merged_array[k].winner = pairs[i].winner;
            merged_array[k].loser = pairs[i].loser;
            i++;
            k++;
        }
        else
        {
            merged_array[k].winner = pairs[j].winner;
            merged_array[k].loser = pairs[j].loser;
            j++;
            k++;
        }
    }
    for (; i <= mid; i++) // for when the left part is longer than the right one
    {
        merged_array[k].winner = pairs[i].winner;
        merged_array[k].loser = pairs[i].loser;
        k++;
    }
    for (; j <= high; j++) // for when the right part is longer than the left one
    {
        merged_array[k].winner = pairs[j].winner;
        merged_array[k].loser = pairs[j].loser;
        k++;
    }

    // put the right order in the original array
    for (int l = low; l <= high; l++) // maybe <=
    {
        pairs[l].winner = merged_array[l].winner;
        pairs[l].loser = merged_array[l].loser;
    }
}

void merge_sort(int low, int high) // checked
{
    // if the lower index is smaller than the higher one. Otherwise, it's sorted
    if (low < high)
    {
        int mid = (low + high) / 2;
        merge_sort(low, mid); // sort the left part
        merge_sort(mid + 1, high); // sort the right part
        merge(low, mid, high); // call merge funtion
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) // checked
{
    // merge sort maybe with recursion ??
    if (pair_count < 2) // if there is only one pair, there's no need to sort
    {
        return;
    }

    // call merge_sort function
    merge_sort(0, pair_count - 1); // pair_count - 1 is to use it as an index
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int p = 0, ind = 0; //

    while (p < pair_count)
    {
        for (int i = 0; i < candidate_count - 1; i++) // column(i)
        {
            for (int j = i + 1; j < candidate_count; j++) // row(j)
            {
                if (pairs[p].winner == i && pairs[p].loser == j) //&& i != j )
                {
                    locked[i][j] = true;
                    p++;
                    ind++;
                }
                else if (pairs[p].winner == j && pairs[p].loser == i)
                {
                    locked[j][i] = true;
                    p++;
                    ind++;
                }
                else if (p >= pair_count)
                {
                    break;
                }
                else
                {
                    ind--;
                    continue;
                }
            }
            if (p >= pair_count)
            {
                break;
            }
        }
    }
    if (ind > 0) // has to be 0 or less
    {
        locked[pairs[pair_count - 1].winner][pairs[pair_count - 1].loser] = false;
    }
    printf("%i %i %i %i\n", ind, locked[0][0], locked[1][0], locked[2][0]);
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = 0;

    for (int i = 0; i < candidate_count; i++) // I'll treat this loop as columns (i)
    {
        for (int j = 0; j < candidate_count; j++) // I'll treat this loop as rows (j)
        {
            if (locked[j][i] == false)
            {
                winner++;
            }
            else
            {
                continue;
            }
        }
        if (winner == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        winner = 0;
    }
    // printf("error with print winner\n"); // debugger ***********
}