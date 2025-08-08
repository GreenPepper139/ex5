#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

//some recommendations for functions, you may implement however you want

char* readString() {
    char *str = NULL;
    int size = 0;
    char c;
    while ((c = getchar()) != '\n') {
        size++;
        str = (char*)realloc(str, size + 1);
        if (!str) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        str[size - 1] = c;
    }
    if (str) {
        str[size] = '\0';
    }
    else {
        str = (char*)malloc(1);
        if (!str) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        str[0] = '\0';
    }
    return str;
}
 
// Free the memory for the song to be deleted
void freeSong(Song* song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void deleteSong(Playlist* playlist) {
    for(int i = 0; i < playlist->songsNum; i++) {
        printf("%d. Title: %s\n", i + 1, playlist->songs[i]->title);
        printf("   Artist: %s\n", playlist->songs[i]->artist);
        printf("   Released: %d\n", playlist->songs[i]->year);
        printf("   Streams: %d\n", playlist->songs[i]->streams);
        printf("\n");
    }
    printf("choose a song to delete, or 0 to quit:\n");
    int choice;
    scanf(" %d", &choice);
    getchar();
    if (choice == 0) {
        return;
    }
    choice--;
    freeSong(playlist->songs[choice]);
    // Shift the remaining songs
    for (int i = choice; i < playlist->songsNum - 1; i++) {
        playlist->songs[i] = playlist->songs[i + 1];
    }
    playlist->songsNum--;
    playlist->songs = (Song**)realloc(playlist->songs, playlist->songsNum * sizeof(Song*));
    if (playlist->songsNum > 0 && playlist->songs == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    printf("Song deleted successfully.\n");
}

void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);
    printf("$ %s $\n", song->lyrics);
    song->streams++;
}

void freePlaylist(Playlist* playlist) {
    for (int i = 0; i < playlist->songsNum; i++) {
        freeSong(playlist->songs[i]);
    }
    free(playlist->name);
    free(playlist->songs);
    free(playlist);
}

void printPlaylistsMenu() {
    printf("Please Choose:\n"); 
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}

void sortPlaylist(Playlist *playlist) {
    int choice;
    printf("choose:\n");
    printf("\t1. sort by year\n\t2. sort by streams - ascending order\n"
           "\t3. sort by streams - descending order\n\t4. sort alphabetically\n");
    scanf(" %d", &choice);
    getchar();
    switch (choice) {
        case 1: {
            // Sort by year using bubble sort
            for (int i = 0; i < playlist->songsNum - 1; i++) {
                for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                    if (playlist->songs[j]->year > playlist->songs[j + 1]->year) {
                        // Swap songs
                        Song* temp = playlist->songs[j];
                        playlist->songs[j] = playlist->songs[j + 1];
                        playlist->songs[j + 1] = temp;
                    }
                }
            }
            break;
        }
        case 2: {
            // Sort by streams ascending
            for (int i = 0; i < playlist->songsNum - 1; i++) {
                for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                    if (playlist->songs[j]->streams > playlist->songs[j + 1]->streams) {
                        Song* temp = playlist->songs[j];
                        playlist->songs[j] = playlist->songs[j + 1];
                        playlist->songs[j + 1] = temp;
                    }
                }
            }
            break;
        }
        case 3: {
            // Sort by streams descending
            for (int i = 0; i < playlist->songsNum - 1; i++) {
                for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                    if (playlist->songs[j]->streams < playlist->songs[j + 1]->streams) {
                        Song* temp = playlist->songs[j];
                        playlist->songs[j] = playlist->songs[j + 1];
                        playlist->songs[j + 1] = temp;
                    }
                }
            }
            break;
        }
        default: {
            // Sort alphabetically by title
            for (int i = 0; i < playlist->songsNum - 1; i++) {
                for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                    if (strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0) {
                        Song* temp = playlist->songs[j];
                        playlist->songs[j] = playlist->songs[j + 1];
                        playlist->songs[j + 1] = temp;
                    }
                }
            }
            break;
        }
    }
    printf("sorted\n");
}

int main() {
    int choice, numOfPlaylists = 0;
    Playlist** playlists = NULL;
    do {
        printPlaylistsMenu();
        scanf(" %d", &choice);
        getchar();
        switch(choice) {
            case 1: {
                // until user choose to exit
                while(1) {
                    int choice2;
                    printf("Choose a playlist:\n");
                    for (int i = 0; i < numOfPlaylists; i++) {
                        printf("\t%d. %s\n", i + 1, playlists[i]->name);
                    }
                    printf("\t%d. Back to main menu\n", numOfPlaylists + 1);
                    scanf(" %d", &choice2);
                    getchar();
                    if(choice2 == numOfPlaylists + 1) {
                        break;
                    }
                    choice2--;
                    if(choice2 < 0 || choice2 > numOfPlaylists) {
                        printf("Invalid option\n");
                        break;
                    }
                    int choice3;
                    printf("playlist %s:\n", playlists[choice2]->name);
                    do {
                        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. "
                            "exit\n");
                        scanf(" %d", &choice3);
                        getchar();
                        switch (choice3) {
                            case 1: {
                                for(int i = 0; i < playlists[choice2]->songsNum; i++) {
                                    printf("%d. Title: %s\n", i + 1, playlists[choice2]->songs[i]->title);
                                    printf("   Artist: %s\n", playlists[choice2]->songs[i]->artist);
                                    printf("   Released: %d\n", playlists[choice2]->songs[i]->year);
                                    printf("   Streams: %d\n", playlists[choice2]->songs[i]->streams);
                                    printf("\n");
                                }
                                while(1) {
                                    printf("choose a song to play, or 0 to quit:\n");
                                    int choice4;
                                    scanf(" %d", &choice4);
                                    getchar();
                                    if(choice4 == 0) {
                                        break;
                                    }
                                    choice4--;
                                    playSong(playlists[choice2]->songs[choice4]);
                                }
                                break;
                            }
                            case 2: {
                                printf("Enter song's details\n");
                                playlists[choice2]->songs = (Song**)realloc(playlists[choice2]->songs,
                                    (playlists[choice2]->songsNum + 1) * sizeof(Song*));
                                if (playlists[choice2]->songs == NULL) {
                                    printf("Memory allocation failed\n");
                                    exit(1);
                                }
                                // Allocate the new song structure
                                playlists[choice2]->songs[playlists[choice2]->songsNum] = (Song*)malloc(sizeof(Song));
                                if (playlists[choice2]->songs[playlists[choice2]->songsNum] == NULL) {
                                    printf("Memory allocation failed\n");
                                    exit(1);
                                }
                                printf("Title:\n");
                                playlists[choice2]->songs[playlists[choice2]->songsNum]->title = readString();
                                printf("Artist:\n");
                                playlists[choice2]->songs[playlists[choice2]->songsNum]->artist = readString();
                                printf("Year of release:\n");
                                scanf(" %d", &playlists[choice2]->songs[playlists[choice2]->songsNum]->year);
                                getchar();
                                printf("Lyrics:\n");
                                playlists[choice2]->songs[playlists[choice2]->songsNum]->lyrics = readString();
                                // Initialize streams to 0
                                playlists[choice2]->songs[playlists[choice2]->songsNum]->streams = 0;
                                // Increment song count
                                playlists[choice2]->songsNum++;
                                break;
                            }
                            case 3: {
                                deleteSong(playlists[choice2]);
                                break;
                            }
                            case 4: {
                                sortPlaylist(playlists[choice2]);
                                break;
                            }
                            case 5: {
                                for(int i = 0; i < playlists[choice2]->songsNum; i++) {
                                    playSong(playlists[choice2]->songs[i]);
                                }
                                break;
                            }
                            case 6:
                                break;
                            default:
                                printf("Invalid option\n");
                        }
                    } while(choice3 != 6);
                }
                break;
            }
            case 2: {
                printf("Enter playlist's name:\n");
                playlists = (Playlist**)realloc(playlists, (numOfPlaylists + 1) * sizeof(Playlist*));
                if (playlists == NULL) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                playlists[numOfPlaylists] = (Playlist*)malloc(sizeof(Playlist));
                if (playlists[numOfPlaylists] == NULL) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                playlists[numOfPlaylists]->name = readString();
                playlists[numOfPlaylists]->songs = NULL;
                playlists[numOfPlaylists]->songsNum = 0;
                numOfPlaylists++;
                break;
            }
            case 3: {
                printf("Choose a playlist:\n");
                for (int i = 0; i < numOfPlaylists; i++) {
                    printf("\t%d. %s\n", i + 1, playlists[i]->name);
                }
                printf("\t%d. Back to main menu\n", numOfPlaylists + 1);
                int choice2;
                scanf(" %d", &choice2);
                getchar();
                if(choice2 == numOfPlaylists + 1) {
                    break;
                }
                choice2--;
                if(choice2 < 0 || choice2 > numOfPlaylists) {
                    printf("Invalid option\n");
                    break;
                }
                freePlaylist(playlists[choice2]);
                numOfPlaylists--;
                for (int j = choice2; j < numOfPlaylists; j++) {
                    playlists[j] = playlists[j + 1];
                }
                playlists = (Playlist**)realloc(playlists, numOfPlaylists * sizeof(Playlist*));
                if (numOfPlaylists > 0 && playlists == NULL) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                printf("Playlist deleted.\n");
                break;
            }
            case 4:
                break;
            default:
                printf("Invalid option\n");
        }
    } while(choice != 4);
    for (int i = 0; i < numOfPlaylists; i++) {
        freePlaylist(playlists[i]);
    }
    free(playlists);
    printf("Goodbye!\n");  
}
