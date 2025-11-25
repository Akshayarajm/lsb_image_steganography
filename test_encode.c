/*
Name        :Akshaya Raj Mullasseril
Date        :11/11
Description :LSB IMAGE STEGANOGRAPHY
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    //uint img_size;

    if (argc < 2)
    {
        printf("Usage: %s -e <.bmp_file> <.text_file> [output file]\n-d <.bmp_file> [output file]", argv[0]);
        return 1;
    }

    if(check_operation_type(argv) == e_encode)
    {
        if (read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
            do_encoding(&encInfo);
            printf("## Encoding Done Successfully ##\n");
        }
        else
        {
            printf("Argument validation failed.\n");
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            do_decoding(&decInfo);
            printf("## Decoding Done Successfully ##\n");
        }
        else
        {
            printf("Argument validation failed.\n");
        }
    
    }
    else if(check_operation_type(argv) == e_unsupported)
    {
        printf("Arguments are not in correct order or enough\n");
    }
    else
    {
        printf("Not enough arguments\n");
    }


    //Test get_image_size_for_bmp
    /*img_size = get_image_size_for_bmp(encInfo.fptr_src_imagr);
    printf("INFO: Image size = %u\n", img_size);*/

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
        return e_unsupported;
   
   
}

