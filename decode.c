#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    if(argc == 3 || argc == 4)
    {
        if(strstr(argv[2], ".bmp"))
        {
            decInfo->enc_image_fname = argv[2];
            if(argc == 4)
            {
                strcpy(decInfo->secret_fname, argv[3]);
                return e_success;
            }
            else
            {
                strcpy(decInfo->secret_fname, "sec");
                printf("Output File not mentioned. Creating sec.txt as default\n");
                return e_success;
            }
        }
        else
        {
            printf("Source file is not .bmp\n");
            return e_failure;
        }
    }
    else
    {
        printf("Arguments are not passed as required\n./a.out -d <encoded file name> <secret file name(optional)");
        return e_failure;
    }
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("## Decoding Procedure Started ##\n");
    printf("Opening required files\n");
    if(open_image_file(decInfo) == e_success)
    {
        printf("Done. Opened all required files\n");
        if(skip_bmp_header(decInfo) == e_success)
        {
            printf("Decoding Magic String Signature\n");
            if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
            {
                printf("Done\n");
                printf("Decoding Output File Extenstion\n");
                if(decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("Done\n");
                    printf("Decoding File Size\n");
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Done\n");
                        printf("Decoding File Data\n");
                        if(decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Done\n");
                            return e_success;
                        }
                        else
                        {
                            printf("Not Done\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Not Done\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Not Done\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Not Done\n");
                return e_failure;
            }
        }
        else
        {
            printf("Not Done\n");
            return e_failure;
        }
    }
    else
    {
        printf("Not Done\n");
        return e_failure;
    }
}

/* Get File pointers for i/p and o/p files */
Status open_image_file(DecodeInfo *decInfo)
{
    decInfo->fptr_enc_image = fopen(decInfo->enc_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_enc_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->enc_image_fname);

    	return e_failure;
    }
    else
        printf("Opened steged_img.bmp\n");
    return e_success;
}

/* skip bmp image header */
Status skip_bmp_header(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_enc_image, 54, SEEK_SET);
    return e_success;
}

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char arr[3] = {0};
    char data[8];
    
    for(int i = 0; i < 2; i++)
    {
        fread(data, 1, 8, decInfo->fptr_enc_image);
        decode_byte_from_lsb(&arr[i], data);
    }
    arr[2] = '\0';
    if(strcmp(MAGIC_STRING, arr) == 0)
        return e_success;
    return e_failure;
}

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int num = 0;
    char data[32];
    
    fread(data, 1, 32, decInfo->fptr_enc_image);
    decode_int_to_lsb(&num, data);
    char arr[8];
    
    for(int i = 0; i < num; i++)
    {
        fread(arr, 1, 8, decInfo->fptr_enc_image);
        decode_byte_from_lsb(&decInfo->extn_secret_file[i], arr);
    }
    
    decInfo->extn_secret_file[num] = '\0';
    decInfo->size_secret_file = 0;
    strcat(decInfo->secret_fname, decInfo->extn_secret_file);
    return e_success;

}

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char data[32];
    fread(data, 1, 32, decInfo->fptr_enc_image);
    decode_int_to_lsb(&decInfo->size_secret_file, data);
    return e_success;

}

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    printf("Opened txt file\n");
    char ch;
    char arr[8];
    for(int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(arr, 1, 8, decInfo->fptr_enc_image);
        decode_byte_from_lsb(&ch, arr);
        fwrite(&ch, 1, 1, decInfo->fptr_secret);
    }

}

/* decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char * data, char *image_buffer)
{
    *data = 0;
    for(int i = 0; i < 8; i++)
    {
        *data = (*data) | ((image_buffer[i] & 1) << (7 - i));
    }
    return e_success;
}

/* decode a int into LSB of image data array */
Status decode_int_to_lsb(int * file_size, char * file_buffer)
{
    for (int i = 0; i < 31; i++)
    {
        *file_size = (*file_size) | ((file_buffer[i] & 1) << (31 - i));
    }
    return e_success;
}