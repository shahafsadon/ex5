#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for default values, added increments, and maximum size limits
#define DEFAULT_VALUE 0
#define PLUS_DEFAULT 1
#define MAX_CHAR 256

// Define a structure for a Song with title, artist, year, lyrics, and streams
typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

// Define a structure for a Playlist containing an array of songs and the number of songs
typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

// Define a structure for Spotify, which contains an array of playlists and the number of playlists
typedef struct Spotify {
    Playlist **playlist;
    int sizeOfPlaylist;
} Spotify;

// Function declarations
Playlist *createPlaylist();
int getNumberFromUser();
Song *createSong();
void freeSong(Song *song);
void deleteSongFromPlaylist(Playlist *playlist, int songIndex);
void playSong(Song *song);
void playPlaylist(Playlist *playlist);
void freePlaylist(Playlist *playlist);
void removePlaylist(Spotify *spotify, Playlist *playlist, int playListIndex);
void freeSpotify(Spotify *spotify);
void printMainMenu();
void printPlaylistMenu(char *name, int firstTime);
void sortPlaylist(Playlist *playlist);
void addSongToPlaylist(Playlist *playlist, Song *song);
void addPlaylist(Spotify *spotify);
int choosePlaylist(Spotify *spotify);
void showSongs(Song **songsList, int size);
void displayPlaylistMenu(Playlist *playlist);



// Main function to handle user interaction with the Spotify application
int main() {
    Spotify *spotify = NULL;          // Initialize a pointer for Spotify (start with NULL)
    int userChoice = DEFAULT_VALUE;   // Initialize user's menu choice to 0 (default value)

    // Main loop to continuously display the menu and handle user actions
    do {
        printMainMenu();               // Display the main menu
        userChoice = getNumberFromUser();  // Get user's choice from input

        // Switch case to handle different user choices
        switch (userChoice) {
            case 1: {
                int result = DEFAULT_VALUE;  // Initialize result for playlist selection
                do {
                    result = choosePlaylist(spotify);  // Prompt user to choose a playlist
                    if (result != -PLUS_DEFAULT) {  // If a valid playlist is chosen
                        Playlist *playlist = spotify->playlist[result]; // Get selected playlist
                        displayPlaylistMenu(playlist); // Display menu for selected playlist
                    }
                } while (result != -PLUS_DEFAULT);  // Continue until a valid playlist is selected

                break;
            }
            case 2: {
                // If no Spotify data exists, allocate memory for Spotify object
                if (spotify == NULL) {
                    spotify = (Spotify *) calloc(PLUS_DEFAULT, sizeof(Spotify));
                }
                addPlaylist(spotify);  // Call function to add a new playlist
                break;
            }
            case 3: {
                // Allow the user to choose a playlist and remove it
                int result = choosePlaylist(spotify);
                if (result != -PLUS_DEFAULT) {
                    Playlist *playlist = spotify->playlist[result];
                    removePlaylist(spotify, playlist, result);  // Remove selected playlist
                }
                break;
            }
            case 4: {
                // Free the memory allocated for Spotify and exit
                if (spotify != NULL) {
                    freeSpotify(spotify);  // Free all memory used by Spotify
                }
                printf("Goodbye!");  // Print a goodbye message
                break;
            }
        }
    } while (userChoice != 4);  // Continue until the user selects to exit (choice 4)

    return PLUS_DEFAULT;  // Return 1 as a successful exit
}
// This function creates a new playlist by getting the playlist name from the user.
Playlist *createPlaylist() {
    char input[MAX_CHAR - PLUS_DEFAULT] = {DEFAULT_VALUE};
    Playlist *result = (Playlist *) calloc(PLUS_DEFAULT, sizeof(Playlist));
    printf("Enter playlist's name:\n");
    scanf("%[^\n]", input);
    getchar();
    int len = strlen(input);
    if (len > DEFAULT_VALUE && input[len - PLUS_DEFAULT] == '\r') {
        input[len - PLUS_DEFAULT] = '\0'; // Remove the '\r' if present
    }
    result->name = (char *) calloc(strlen(input) + PLUS_DEFAULT, sizeof(char));
    strcpy(result->name, input);
    return result;
}

// This function retrieves an integer input from the user.
int getNumberFromUser() {
    int number = DEFAULT_VALUE;
    scanf("%d", &number);
    getchar();
    getchar();
    return number;
}

// This function creates a new song by getting its details (title, artist, year, and lyrics) from the user.
Song *createSong() {
    char inputTitle[MAX_CHAR] = {DEFAULT_VALUE};
    char inputArtist[MAX_CHAR] = {DEFAULT_VALUE};
    char inputLyrics[MAX_CHAR] = {DEFAULT_VALUE};
    Song *result = (Song *) calloc(PLUS_DEFAULT, sizeof(Song));
    printf("Enter song's details\n");
    printf("Title:\n");
    scanf("%[^\n]", inputTitle);
    getchar();
    int len = strlen(inputTitle);
    if (len > DEFAULT_VALUE && inputTitle[len - PLUS_DEFAULT] == '\r') {
        inputTitle[len - PLUS_DEFAULT] = '\0'; // Remove the '\r' if present
    }
    result->title = (char *) calloc(strlen(inputTitle) + PLUS_DEFAULT, sizeof(char));
    strcpy(result->title, inputTitle);
    printf("Artist:\n");
    scanf("%[^\n]", inputArtist);
    getchar();
    int len2 = strlen(inputArtist);
    if (len2 > DEFAULT_VALUE && inputArtist[len2 - PLUS_DEFAULT] == '\r') {
        inputArtist[len2 - PLUS_DEFAULT] = '\0'; // Remove the '\r' if present
    }
    result->artist = (char *) calloc(strlen(inputArtist) + PLUS_DEFAULT, sizeof(char));
    strcpy(result->artist, inputArtist);
    printf("Year of release:\n");
    result->year = getNumberFromUser();
    printf("Lyrics:\n");
    scanf("%[^\n]", inputLyrics);
    getchar();
    int len3 = strlen(inputLyrics);
    if (len3 > 0 && inputLyrics[len3 - PLUS_DEFAULT] == '\r') {
        inputLyrics[len3 - PLUS_DEFAULT] = '\0'; // Remove the '\r' if present
    }
    result->lyrics = (char *) calloc(strlen(inputLyrics) + PLUS_DEFAULT, sizeof(char));
    strcpy(result->lyrics, inputLyrics);
    result->streams = DEFAULT_VALUE;
    return result;
}

// This function frees all dynamically allocated memory for a song.
void freeSong(Song *song) {
    if (song == NULL) {
        return;
    }
    // Free all dynamically allocated fields
    if (song->title != NULL) {
        free(song->title);
    }
    if (song->artist != NULL) {
        free(song->artist);
    }
    if (song->lyrics != NULL) {
        free(song->lyrics);
    }
    free(song);
}

// This function deletes a song from a playlist by its index and shifts the remaining songs.
void deleteSongFromPlaylist(Playlist *playlist, int songIndex) {
    if (playlist == NULL || playlist->songsNum == DEFAULT_VALUE) {
        return;
    }
    if (songIndex < DEFAULT_VALUE || songIndex >= playlist->songsNum) {
        return;
    }
    // Delete the song
    freeSong(playlist->songs[songIndex]);
    // Shift the songs in the array to fill the deleted song's position
    for (int i = songIndex; i < playlist->songsNum - PLUS_DEFAULT; i++) {
        playlist->songs[i] = playlist->songs[i + PLUS_DEFAULT];
    }
    // Decrease the song count
    playlist->songsNum--;
    // Reallocate memory for the new size
    playlist->songs = (Song **) realloc(playlist->songs, (playlist->songsNum * sizeof(Song *)));
     if (playlist->songs == NULL && playlist->songsNum > DEFAULT_VALUE) {
         exit(PLUS_DEFAULT);
     }
    printf("Song deleted successfully.\n");
}
// This function plays a song, displaying its title and lyrics, and increments the stream count.
void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);  // Display the song's title
    printf("$ %s $\n", song->lyrics);  // Display the song's lyrics
    song->streams++;  // Increment the song's stream count
}

// This function plays all songs in the playlist.
void playPlaylist(Playlist *playlist) {
    for (int i = DEFAULT_VALUE; i < playlist->songsNum; i++) {
        Song *song = playlist->songs[i];
        playSong(song);  // Play each song in the playlist
    }
}

// This function frees all memory associated with a playlist, including its songs and name.
void freePlaylist(Playlist *playlist) {
    for (int i = DEFAULT_VALUE; i < playlist->songsNum; i++) {
        freeSong(playlist->songs[i]);  // Free each song in the playlist
    }
    free(playlist->name);  // Free the memory allocated for the playlist name
    free(playlist->songs);  // Free the memory allocated for the songs array
    free(playlist);  // Free the memory allocated for the playlist itself
}

// This function removes a playlist from Spotify and shifts the remaining playlists.
void removePlaylist(Spotify *spotify, Playlist *playlist, int playListIndex) {
    freePlaylist(playlist);  // Free the playlist memory
    // Shift the playlists in the array to fill the deleted playlist's position
    for (int i = playListIndex; i < spotify->sizeOfPlaylist - PLUS_DEFAULT; i++) {
        spotify->playlist[i] = spotify->playlist[i + PLUS_DEFAULT];
    }
    // Decrease the playlist count
    spotify->sizeOfPlaylist--;
    // Reallocate memory for the new size
    spotify->playlist = (Playlist **) realloc(spotify->playlist, spotify->sizeOfPlaylist * sizeof(Playlist *));
    printf("Playlist deleted.\n");  // Inform the user that the playlist was deleted
}

// This function frees all memory associated with Spotify, including all playlists.
void freeSpotify(Spotify *spotify) {
    for (int i = DEFAULT_VALUE; i < spotify->sizeOfPlaylist; i++) {
        freePlaylist(spotify->playlist[i]);  // Free each playlist in Spotify
    }
    free(spotify->playlist);  // Free the memory allocated for the playlists array
    free(spotify);  // Free the memory allocated for the Spotify object
}

// This function prints the main menu of Spotify with options for the user.
void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void printPlaylistMenu(char *name, int firstTime) {
    // If it's the first time, display the playlist name
    if (firstTime) {
        printf("playlist %s:\n", name);
    }
    // Print the usual menu options when the playlist is not empty
    printf("\t1. Show Playlist\n");
    printf("\t2. Add Song\n");
    printf("\t3. Delete Song\n");
    printf("\t4. Sort\n");
    printf("\t5. Play\n");
    printf("\t6. exit\n");
}
// This function sorts the songs in a playlist based on user-selected criteria (year, streams, or title).
void sortPlaylist(Playlist *playlist) {
    printf("choose:\n");
    printf("1. sort by year\n");
    printf("2. sort by streams - ascending order\n");
    printf("3. sort by streams - descending order\n");
    printf("4. sort alphabetically\n");
    int choice = getNumberFromUser();  // Get the user's sorting choice
    if (choice < PLUS_DEFAULT || choice > 4) {  // Validate the choice
        choice = 4;  // Default to alphabetical sort if the choice is invalid
    }
    // Perform bubble sort based on the chosen criterion
    for (int i = DEFAULT_VALUE; i < playlist->songsNum - PLUS_DEFAULT; i++) {
        for (int j = DEFAULT_VALUE; j < playlist->songsNum - i - PLUS_DEFAULT; j++) {
            int swap = DEFAULT_VALUE;
            switch (choice) {
                case 1:  // Sort by year
                    if (playlist->songs[j]->year > playlist->songs[j + PLUS_DEFAULT]->year) {
                        swap = PLUS_DEFAULT;
                    }
                    break;
                case 2:  // Sort by streams - ascending order
                    if (playlist->songs[j]->streams > playlist->songs[j + PLUS_DEFAULT]->streams) {
                        swap = PLUS_DEFAULT;
                    }
                    break;
                case 3:  // Sort by streams - descending order
                    if (playlist->songs[j]->streams < playlist->songs[j + PLUS_DEFAULT]->streams) {
                        swap = PLUS_DEFAULT;
                    }
                    break;
                case 4:  // Sort alphabetically by title
                    if (strcmp(playlist->songs[j]->title, playlist->songs[j + PLUS_DEFAULT]->title) > 0) {
                        swap = PLUS_DEFAULT;
                    }
                    break;
            }
            if (swap) {
                Song *temp = playlist->songs[j];
                playlist->songs[j] = playlist->songs[j + PLUS_DEFAULT];
                playlist->songs[j + PLUS_DEFAULT] = temp;  // Swap the songs
            }
        }
    }
    printf("sorted\n");  // Notify the user that the playlist has been sorted
}
// This function adds a song to a playlist by dynamically resizing the playlist's song array and appending the song.
void addSongToPlaylist(Playlist *playlist, Song *song) {
    if (playlist->songsNum == DEFAULT_VALUE) {
        // Allocate memory for the first song
        playlist->songs = (Song **) calloc((PLUS_DEFAULT), sizeof(Song *));
    } else {
        // Reallocate memory for more songs
        playlist->songs = (Song **) realloc(playlist->songs, sizeof(Song*)
            * (playlist->songsNum + PLUS_DEFAULT));
    }
    playlist->songs[playlist->songsNum++] = song;  // Add the song to the playlist and increment the song count
}

// This function adds a new playlist to Spotify by dynamically resizing the playlist array
// and appending the new playlist.
void addPlaylist(Spotify *spotify) {
    Playlist *newPlayList = createPlaylist();  // Create a new playlist
    if (spotify->playlist == NULL) {
        // Allocate memory for the first playlist
        spotify->playlist = (Playlist **) calloc(PLUS_DEFAULT, sizeof(Playlist *));
    } else {
        // Reallocate memory for more playlists
        spotify->playlist = (Playlist **) realloc(spotify->playlist, sizeof(Playlist *) *
            (spotify->sizeOfPlaylist + PLUS_DEFAULT));
    }
    // Add the new playlist to Spotify and increment the playlist count
    spotify->playlist[spotify->sizeOfPlaylist++] = newPlayList;
}

// This function allows the user to choose a playlist from the list of available playlists.
int choosePlaylist(Spotify *spotify) {
    printf("Choose a playlist:\n");

    int numberOfPlaylists = DEFAULT_VALUE;
    if (spotify != NULL) {
        numberOfPlaylists = spotify->sizeOfPlaylist;
        for (int i = DEFAULT_VALUE; i < numberOfPlaylists; i++) {
            printf("\t%d. %s\n", i + PLUS_DEFAULT, spotify->playlist[i]->name);  // Display the playlists
        }
    }
    printf("\t%d. Back to main menu\n", numberOfPlaylists + PLUS_DEFAULT);
    int choice = getNumberFromUser();  // Get the user's playlist choice
    if (choice == numberOfPlaylists + PLUS_DEFAULT) {
        return -PLUS_DEFAULT;  // Return -1 to indicate going back to the main menu
    }
    return choice - PLUS_DEFAULT;  // Return the selected playlist index
}

// This function displays a list of songs, showing their title, artist, year, and streams.
void showSongs(Song **songsList, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d.  Title: %s\n", i + PLUS_DEFAULT, songsList[i]->title);  // Display song title
        printf("    Artist: %s\n", songsList[i]->artist);  // Display song artist
        printf("    Released: %d\n", songsList[i]->year);  // Display song release year
        printf("    Streams: %d\n", songsList[i]->streams);  // Display song streams
    }
}
// This function displays the playlist menu and handles user interactions for various playlist options
// like playing songs, adding, deleting, sorting, and playing the entire playlist.
void displayPlaylistMenu(Playlist *playlist) {
    int choice = DEFAULT_VALUE;
    int firstTime = PLUS_DEFAULT;
    do {
        printPlaylistMenu(playlist->name, firstTime);  // Display the menu for the playlist
        firstTime = DEFAULT_VALUE;
        choice = getNumberFromUser();  // Get the user's choice
        switch (choice) {
            case 1: {
                showSongs(playlist->songs, playlist->songsNum);  // Display songs in the playlist
                int songIndex;
                do {
                    printf("choose a song to play, or 0 to quit:\n");
                    songIndex = getNumberFromUser();  // Get the song index to play
                    if (songIndex != DEFAULT_VALUE) {
                        playSong(playlist->songs[songIndex - PLUS_DEFAULT]);  // Play the selected song
                    }
                } while (songIndex != DEFAULT_VALUE);  // Repeat until user chooses to quit
                break;
            }
            case 2: {
                Song *song = createSong();  // Create a new song
                addSongToPlaylist(playlist, song);  // Add the song to the playlist
                break;
            }
            case 3: {
                showSongs(playlist->songs, playlist->songsNum);  // Display songs in the playlist
                printf("choose a song to delete, or 0 to quit:\n");
                int songIndex = getNumberFromUser();  // Get the song index to delete
                if (songIndex != DEFAULT_VALUE) {
                    deleteSongFromPlaylist(playlist, songIndex - PLUS_DEFAULT);  // Delete the selected song
                }
                break;
            }
            case 4: {
                sortPlaylist(playlist);  // Sort the playlist
                break;
            }
            case 5: {
                playPlaylist(playlist);  // Play all songs in the playlist
                break;
            }
            case 6: {
                break;  // Exit the playlist menu
            }
        }
    } while (choice != 6);  // Repeat until the user chooses to exit the menu
}
