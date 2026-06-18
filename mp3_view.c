#include "mp3_header.h"

void view(struct MP3 *mp3)
{
    char header[4];

    // Hardcoded file (as you want)
    mp3->org_mp3_fptr = fopen("sample.mp3", "rb");

    if (mp3->org_mp3_fptr == NULL)
    {
        printf("Error : unable to open file\n");
        return;
    }

    // Read ID3 header
    fread(header, 1, 3, mp3->org_mp3_fptr);
    header[3] = '\0';

    if (strcmp(header, "ID3") == 0)
    {
        printf("Yes, it is an MP3 file\n");

        print_tag_data(mp3->org_mp3_fptr, "TIT2"); // Title
        print_tag_data(mp3->org_mp3_fptr, "TPE1"); // Artist
        print_tag_data(mp3->org_mp3_fptr, "TALB"); // Album
        print_tag_data(mp3->org_mp3_fptr, "TYER"); // Year
        print_tag_data(mp3->org_mp3_fptr, "TCON"); // Genre
        print_tag_data(mp3->org_mp3_fptr, "COMM"); // Comment

        fclose(mp3->org_mp3_fptr);
    }
    else
    {
        printf("It is not an MP3 file\n");
        fclose(mp3->org_mp3_fptr);
    }
}


// ---------------- PRINT TAG FUNCTION ----------------

void print_tag_data(FILE *org_mp3_fptr, char *tag)
{
    char frame[5];
    int size;
    char data[100];

    rewind(org_mp3_fptr);

    // Skip ID3 header
    fseek(org_mp3_fptr, 10, SEEK_SET);

    while (fread(frame, 1, 4, org_mp3_fptr) == 4)
    {
        frame[4] = '\0';

        // Read size
        fread(&size, 4, 1, org_mp3_fptr);

        // Convert endian
        toggle_endianess(NULL, &size);

        // Skip flags + encoding
        fseek(org_mp3_fptr, 3, SEEK_CUR);

        if (strcmp(frame, tag) == 0)
        {
            fread(data, 1, size - 1, org_mp3_fptr);
            data[size - 1] = '\0';

            tag_to_name(frame);
            printf(" : %s\n", data);

            break;
        }
        else
        {
            // Skip this frame
            fseek(org_mp3_fptr, size - 1, SEEK_CUR);
        }
    }
}


// ---------------- TAG NAME FUNCTION ----------------

void tag_to_name(char *tag)
{
    if (!strcmp(tag, "TIT2"))
    {
        printf("Title");
    }
    else if (!strcmp(tag, "TPE1"))
    {
        printf("Artist");
    }
    else if (!strcmp(tag, "TALB"))
    {
        printf("Album");
    }
    else if (!strcmp(tag, "TYER"))
    {
        printf("Year");
    }
    else if (!strcmp(tag, "TCON"))
    {
        printf("Music");
    }
    else if (!strcmp(tag, "COMM"))
    {
        printf("Comment");
    }
}