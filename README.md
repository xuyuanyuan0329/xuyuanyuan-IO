# Лабораторная работа 1

**Сюй Юаньюань р3304с:** "Разработка драйверов символьных устройств"

**Цель работы:** Write a character device driver that meets the requirements

## Описание функциональности драйвера

When writing a string to character device /dev/var5, count the number of letters and keep it consistent with the content of /proc/var5

## Инструкция по сборке

1. Enter the source program directory
2. execute make
3. execute insmod var5.ko
4. View device number: 
	cat /proc/devices | grep var5 
5. Create a device node in the /dev directory according to the device number
    ls | grep var5  ===> var5 

## Инструкция пользователя

1. write test program
2. Read and write the /dev/var5 device driver file in the test program.
3. gcc test.c; ./a.out

## Примеры использования

test file in the code directory:

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    char buf[4096] = {0};
    char line[512] = {0};
    fgets(line, 512, stdin);    
    line[strlen(line)-1] = '\0';
    FILE* fp = fopen("/dev/var5", "r+");
    if(fp == NULL)
    {
        printf("open var5 failed!!!\n");
        return -1;
    }
    
    fwrite(line, strlen(line), 1, fp);
    fseek(fp,0,SEEK_SET);
    fread(buf, sizeof(buf), 1, fp);
    printf("%s\n", buf);
    fclose(fp);
    return 0;
}
