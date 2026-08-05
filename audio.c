#include "audio.h"
#include <stdio.h>
#include <stdlib.h>

void play_audio_async(const char *file_path) {
    char command[512];
    snprintf(command, sizeof(command), "mpg123 -q ./%s &", file_path);
    system(command);
}

void play_sound_sequence(const char *file1, const char *file2) {
    char command1[512];
    char command2[512];

    printf("Playing first sound...\n");
    snprintf(command1, sizeof(command1), "mpg123 -q ./%s", file1);
    system(command1);

    printf("Playing second sound...\n");
    snprintf(command2, sizeof(command2), "mpg123 -q ./%s &", file2);
    system(command2);
}