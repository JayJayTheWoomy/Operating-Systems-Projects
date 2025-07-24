#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_BASEBALL_PLAYERS 36
#define NUM_FOOTBALL_PLAYERS 44
#define NUM_RUGBY_PLAYERS 60

pthread_mutex_t fieldMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t baseballCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t footballCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t rugbyCond = PTHREAD_COND_INITIALIZER;

int baseballPlayers = 0;
int footballPlayers = 0;
int rugbyPlayers = 0;

void* player_function(void* arg) {
    int player_id = *((int*)arg);
    int sport = player_id % 3 + 1; // Assign sports: 1 for baseball, 2 for football, 3 for rugby

    // Simulate player-specific logic
    char* sl = (sport == 1) ? "BB" : (sport == 2) ? "FB" : "RB";
   
    pthread_mutex_lock(&fieldMutex);

    // Team Safety
    if ((sport == 1 && footballPlayers > 0) || (sport == 1 && rugbyPlayers > 0) ||
        (sport == 2 && baseballPlayers > 0) || (sport == 2 && rugbyPlayers > 0) ||
        (sport == 3 && baseballPlayers > 0) || (sport == 3 && footballPlayers > 0)) {
        pthread_mutex_unlock(&fieldMutex);
        printf("%s %d waits for team safety.\n",sl, player_id);
        sleep(rand() % 3 + 1); // Simulate some delay
        player_function(arg); // Retry
        return NULL;
    }

    // Maximum Parallelism
    if ((sport == 3 && rugbyPlayers >= 30) ||
        (sport == 1 && baseballPlayers >= 18) ||
        (sport == 2 && footballPlayers >= 22)) {
        pthread_mutex_unlock(&fieldMutex);
        printf("%s %d waits for maximum parallelism.\n",sl, player_id);
        sleep(rand() % 3 + 1); // Simulate some delay
        player_function(arg); // Retry
        return NULL;
    }

    // Join the field
    switch (sport) {
        case 1:
            baseballPlayers++;
            if (baseballPlayers == 18) {
                printf("Baseball game starts!\n");
                pthread_cond_broadcast(&baseballCond);
            }
            break;
        case 2:
            footballPlayers++;
            if (footballPlayers == 22) {
                printf("Football game starts!\n");
                pthread_cond_broadcast(&footballCond);
            }
            break;
        case 3:
            rugbyPlayers++;
            if (rugbyPlayers == 2) {
                printf("Rugby game starts!\n");
                pthread_cond_broadcast(&rugbyCond);
            }
            break;
    }

    pthread_mutex_unlock(&fieldMutex);

    // Simulate player-specific logic
    char* sport_label = (sport == 1) ? "BB" : (sport == 2) ? "FB" : "RB";
    printf("%s %d\n", sport_label, player_id);
    
    // Impliments Fairness
    sleep(rand() % 5 + 1); // Simulate game time

    pthread_mutex_lock(&fieldMutex);

    // Leave the field
    switch (sport) {
        case 1:
            baseballPlayers--;
            if (baseballPlayers == 0) {
                printf("Baseball game ends!\n");
            }
            break;
        case 2:
            footballPlayers--;
            if (footballPlayers == 0) {
                printf("Football game ends!\n");
            }
            break;
        case 3:
            rugbyPlayers--;
            if (rugbyPlayers == 0) {
                printf("Rugby game ends!\n");
            }
            break;
    }

    pthread_mutex_unlock(&fieldMutex);

    return NULL;
}

int main() {
    srand((unsigned int)time(NULL));

    pthread_t players[NUM_BASEBALL_PLAYERS + NUM_FOOTBALL_PLAYERS + NUM_RUGBY_PLAYERS];

    // Create threads for players
    for (int i = 0; i < NUM_BASEBALL_PLAYERS + NUM_FOOTBALL_PLAYERS + NUM_RUGBY_PLAYERS; i++) {
        int* player_id = malloc(sizeof(int));
        *player_id = i + 1;
        pthread_create(&players[i], NULL, player_function, (void*)player_id);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_BASEBALL_PLAYERS + NUM_FOOTBALL_PLAYERS + NUM_RUGBY_PLAYERS; i++) {
        pthread_join(players[i], NULL);
    }

    return 0;
}
