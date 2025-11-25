#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

 /* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    if(argc == 4 || argc == 5)
    {
        if(strstr(argv[2], ".bmp"))
        {
            encInfo->src_image_fname = argv[2];
            if(strstr(argv[3], ".txt"))
            {
                encInfo->secret_fname = argv[3];
                strcpy(encInfo->extn_secret_file, ".txt");

                if(argc == 5)
                {
                    if(strstr(argv[4], ".bmp"))
                    {
                        encInfo->stego_image_fname = argv[4];
                        return e_success;
                    }
                    else
                    {
                        printf("Destination file is not .bmp\n");
                        return e_failure;
                    }
                }
                else
                {
                    encInfo->stego_image_fname = "stego.bmp";
                    printf("Output File not mentioned. Creating steged_img.bmp as default\n");
                    return e_success;
                }
            }
            else
            {
                printf("Secret file is not .txt\n");
                return e_failure;
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
        printf("Not enough arguments\n");
        return e_failure;
    }
}


Status open_files(EncodeInfo *encInfo)
{
    printf("Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    else
        printf("Opened SkeletonCode/beautiful.bmp\n");



    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    else
        printf("Opened secret.txt\n");


    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    else
        printf("Opened %s\n", encInfo->stego_image_fname);

    // No failure return e_success
    return e_success;
}



Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("Done\n## Encoding Procedure Started ##\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("Copying Image Header\n");
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                printf("Done\n");
                printf("Encoding Magic String Signature\n");
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("Done\n");
                    if(encode_secret_file_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                    {
                        printf("Encoding secret.txt File Extenstion\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("Done\n");
                            printf("Encoding secret.txt File Size\n");
                            if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("Done\n");
                                printf("Encoding secret.txt File Data\n");
                            
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Done\n");
                                    printf("Copying Left Over Data\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
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

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    uint magicstring = strlen(MAGIC_STRING); // = strlen(encInfo->secret_data);
    uint extension = strlen(encInfo->extn_secret_file);
    encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("Checking for secret.txt size\n");
    if(encInfo->size_secret_file)
        printf("Done. Not Empty\n");
    else
        printf("Empty\n");
    uint encoding_things = (magicstring + extension + sizeof(int) + encInfo -> size_secret_file) * 8;
    printf("Checking for SkeletonCode/beautiful.bmp capacity to handle secret.txt\n");
    if(encoding_things <= encInfo->image_capacity)
    {
        printf("Done. Found OK\n");
        return e_success;
    }
    else
    {
        printf("Not enough size\n");
        return e_failure;
    }
}

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    char arr[60];
    fread(&arr, 1, 54, fptr_src_image);
    fwrite(&arr, 1, 54, fptr_dest_image);
    return e_success;
}

/* Get file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/* Encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char file_buffer[32];
    fread(file_buffer,32,1,encInfo-> fptr_src_image);
    encode_int_to_lsb(file_size,file_buffer);
    fwrite(file_buffer,32,1,encInfo -> fptr_stego_image);
    return e_success;
}

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for(int i = 0; i < size; i++)
    {
        fread(image_buffer, 1, 8, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        fwrite(image_buffer, 1, 8, fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & ~(1)) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image((char *)file_extn, strlen(file_extn),
                         encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_int_to_lsb(int file_size, char * file_buffer)
{
    for (int i = 0; i < 31; i++)
    {
        file_buffer[i] = (file_buffer[i] & ~(1)) | ((file_size >> (31 - i)) & 1);
    }
    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char secret_file_data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(secret_file_data, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    if(encode_data_to_image(secret_file_data, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }

}


/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    fseek(fptr_src, 366, SEEK_SET);
    char ch;
    while((fread(&ch, 1, 1, fptr_src) == 1))
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}