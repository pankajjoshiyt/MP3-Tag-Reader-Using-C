#include "mp3_header.h"

void edit(struct MP3 *mp3)
{
    FILE *src, *dest;
    char frame[5];
    int size;
    char ch;
    char tag[5];

    // Map CLI option
    if (!strcmp(mp3->edit_option, "-t")) strcpy(tag, "TIT2");
    else if (!strcmp(mp3->edit_option, "-a")) strcpy(tag, "TPE1");
    else if (!strcmp(mp3->edit_option, "-A")) strcpy(tag, "TALB");
    else if (!strcmp(mp3->edit_option, "-y")) strcpy(tag, "TYER");
    else if (!strcmp(mp3->edit_option, "-m")) strcpy(tag, "TCON");
    else if (!strcmp(mp3->edit_option, "-c")) strcpy(tag, "COMM");
    else
    {
        printf("Invalid edit option\n");
        return;
    }

    src = fopen(mp3->mp3_filename, "rb");
    dest = fopen("temp.mp3", "wb");

    if (!src || !dest)
    {
        printf("File open error\n");
        return;
    }

    // Copy header
    char buffer[10];
    fread(buffer, 1, 10, src);
    fwrite(buffer, 1, 10, dest);

    while (fread(frame, 1, 4, src) == 4)
    {
        frame[4] = '\0';

        fread(&size, 4, 1, src);
        toggle_endianess(mp3, &size);

        if (size <= 0)
            break;

        char flags[2];
        fread(flags, 1, 2, src);

        fwrite(frame, 1, 4, dest);

        if (strcmp(frame, tag) == 0)
        {
            int new_size = strlen(mp3->new_data) + 1;

            int temp = new_size;
            toggle_endianess(mp3, &temp);

            fwrite(&temp, 4, 1, dest);
            fwrite(flags, 1, 2, dest);

            ch = 0;
            fwrite(&ch, 1, 1, dest);

            fwrite(mp3->new_data, 1, new_size - 1, dest);

            if (size > 0)
                fseek(src, size, SEEK_CUR);
        }
        else
        {
            int temp = size;
            toggle_endianess(mp3, &temp);

            fwrite(&temp, 4, 1, dest);
            fwrite(flags, 1, 2, dest);

            for (int i = 0; i < size; i++)
            {
                fread(&ch, 1, 1, src);
                fwrite(&ch, 1, 1, dest);
            }
        }
    }

    while (fread(&ch, 1, 1, src))
    {
        fwrite(&ch, 1, 1, dest);
    }

    fclose(src);
    fclose(dest);

    remove(mp3->mp3_filename);
    rename("temp.mp3", mp3->mp3_filename);

    printf("Tag updated successfully!\n");
}