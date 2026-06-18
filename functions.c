#include "mp3_header.h"

int validate_cla(int argc, char *argv[], struct MP3 *mp3)
{
    // No arguments
    if (argc == 1)
    {
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE : To view  : ./a.out -v mp3filename\n");
        printf("        To edit  : ./a.out -e -t/-a/-A/-m/-y/-c \"text\" mp3filename\n");
        printf("        For help : ./a.out --help\n");
        return 0;
    }

    // Help option
    if (strcmp(argv[1], "--help") == 0)
    {
        printf("------------------------------->HELP<-----------------------------------------\n");
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("\t\t2.1. -t -> to edit song title\n");
        printf("\t\t2.2. -a -> to edit artist name\n");
        printf("\t\t2.3. -A -> to edit album name\n");
        printf("\t\t2.4. -y -> to edit year\n");
        printf("\t\t2.5. -m -> to edit content\n");
        printf("\t\t2.6. -c -> to edit comment\n");
        printf("------------------------------------------------------------------------------\n");
        return 0;
    }

    // VIEW OPTION
    if (!strcmp(argv[1], "-v"))
    {
        if (argc != 3)
        {
            printf("ERROR: Invalid arguments for viewing\n");
            printf("USAGE: ./a.out -v file.mp3\n");
            return 0;
        }

        mp3->mp3_filename = argv[2];   // assign filename
        return VIEW;
    }

    // EDIT OPTION
    if (!strcmp(argv[1], "-e"))
    {
        if (argc < 5)
        {
            printf("ERROR: Invalid arguments for editing\n");
            printf("USAGE: ./a.out -e -t/-a/-A/-y/-m/-c \"text\" file.mp3\n");
            return 0;
        }

        mp3->edit_option = argv[2];    
        mp3->new_data = argv[3];       // ✅ correct variable name
        mp3->mp3_filename = argv[4];   

        return EDIT;
    }

    // Invalid option
    printf("ERROR: Invalid option\n");
    printf("Use ./a.out --help for usage\n");

    return 0;
}


// 🔁 Convert endian format (big ↔ little)
void toggle_endianess(struct MP3 *mp3, int *size)
{
    char *ptr = (char *)size;

    char temp;

    temp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = temp;

    temp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = temp;
}