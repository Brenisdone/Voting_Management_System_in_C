#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CANDIDATES 5
#define MAX_VOTERS 10

// Global structures
typedef struct {
    char name[50];
    int votes;
} Candidate;

Candidate candidates[MAX_CANDIDATES];
char votedIDs[MAX_VOTERS][11];
int numCandidates, numVoters, votedCount = 0, next = 0;
int nota_votes = 0;

// --- Utility ---
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int isValidVoterID(char id[]) {
    if (strlen(id) != 4) return 0;
    if (!isalpha(id[0])) return 0;
    for (int i = 1; i < 4; i++)
        if (!isdigit(id[i])) return 0;
    return 1;
}

int isDuplicateID(char id[]) {
    for (int i = 0; i < votedCount; i++)
        if (strcmp(votedIDs[i], id) == 0) return 1;
    return 0;
}

// --- Input Functions ---
void inputCandidates() {
    while (1) {
        printf("Enter number of candidates (max %d): ", MAX_CANDIDATES);

        if (scanf("%d", &numCandidates) != 1) {
            printf("Invalid input. Enter numbers only.\n");
            clear_input_buffer();
            continue;
        }

        if (numCandidates < 1 || numCandidates > MAX_CANDIDATES) {
            printf("Invalid number. Try again.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();
        break;
    }

    printf("Enter candidate names (spaces allowed):\n");
    for (int i = 0; i < numCandidates; i++) {
        printf("Candidate %d: ", i + 1);
        fgets(candidates[i].name, sizeof(candidates[i].name), stdin);
        candidates[i].name[strcspn(candidates[i].name, "\n")] = '\0';
        candidates[i].votes = 0;
    }
}

void inputVoters() {
    while (1) {
        printf("Enter number of voters (max %d): ", MAX_VOTERS);

        if (scanf("%d", &numVoters) != 1) {
            printf("Invalid input. Enter numbers only.\n");
            clear_input_buffer();
            continue;
        }

        if (numVoters < 1 || numVoters > MAX_VOTERS) {
            printf("Invalid number. Try again.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();
        break;
    }
}

// --- Core Function ---
void castVote() {
    int age;

    printf("\n--- Casting Vote for Voter %d ---\n", next + 1);

    // AGE INPUT SAFE
    while (1) {
        printf("Enter Age: ");
        if (scanf("%d", &age) != 1) {
            printf("Invalid age. Numbers only.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        break;
    }

    if (age < 18) {
        printf("Not eligible (age < 18).\n");
        next++;
        return;
    }
    if (age > 150) {
        printf("Age unrealistic. Skipping.\n");
        next++;
        return;
    }

    // VOTER ID SAFE
    char voterID[20];
    while (1) {
        printf("Enter Voter ID: ");
        scanf("%s", voterID);
        clear_input_buffer();

        if (!isValidVoterID(voterID)) {
            printf("Invalid ID! Must be 1 letter + 3 digits.\n");
            continue;
        }
        if (isDuplicateID(voterID)) {
            printf("Duplicate voter!\n");
            next++;
            return;
        }
        break;
    }

    // CHOICE SAFE
    printf("\nSelect candidate:\n");
    for (int i = 0; i < numCandidates; i++)
        printf("%d. %s\n", i + 1, candidates[i].name);

    printf("%d. NOTA\n", numCandidates + 1);

    int choice;
    while (1) {
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Numbers only.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == numCandidates + 1) {
            nota_votes++;
            break;
        }
        if (choice >= 1 && choice <= numCandidates) {
            candidates[choice - 1].votes++;
            break;
        }
        printf("Invalid choice.\n");
    }

    strcpy(votedIDs[votedCount], voterID);
    votedCount++;
    next++;

    printf("Vote cast successfully!\n");
}

void viewResults() {
    printf("\n--- Voting Results ---\n");
    for (int i = 0; i < numCandidates; i++)
        printf("%s : %d votes\n", candidates[i].name, candidates[i].votes);

    printf("NOTA : %d votes\n", nota_votes);
}

void viewTurnout() {
    printf("\n--- Voter Turnout ---\n");
    printf("Total Registered Voters : %d\n", numVoters);
    printf("Total Votes Cast        : %d\n", votedCount);
}

// --- MAIN ---
int main() {
    inputCandidates();
    inputVoters();

    // VOTER LOOP SAFE
    for (int i = 0; i < numVoters; i++) {
        int want;

        while (1) {
            printf("\nVoter %d: Want to vote? (1=Yes, 0=No): ", i + 1);
            if (scanf("%d", &want) != 1) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (want == 1) {
                castVote();
                break;
            }
            if (want == 0) {
                printf("Voter %d skipped.\n", i + 1);
                next++;
                break;
            }
            printf("Invalid choice.\n");
        }
    }

    // MENU SAFE
    while (1) {
        int choice;
        printf("\n--- Menu ---\n");
        printf("2. View Results\n");
        printf("3. View Turnout\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 2: viewResults(); break;
            case 3: viewTurnout(); break;
            case 4: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
