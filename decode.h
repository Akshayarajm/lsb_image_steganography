#ifndef DECODE_H
#define DECODE_H
#include<string.h>
#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *enc_image_fname;
    FILE *fptr_enc_image;
    
    /* Secret File Info */
    char secret_fname[100];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    int file_extn_size;
    int size_secret_file;

} DecodeInfo;


/* Decoding function prototype */


/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int, char *argv[], DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_image_file(DecodeInfo *decInfo);

/* skip bmp image header */
Status skip_bmp_header(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode a byte into LSB of image data array */
Status decode_byte_from_lsb(char * data, char *image_buffer);

/* decode a int into LSB of image data array */
Status decode_int_to_lsb(int * file_size, char * file_buffer);

#endif
