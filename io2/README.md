# Лабораторная работа 2

**Название:Сюй Юаньюань** "Разработка драйверов блочных устройств"

**Цель работы:** получить знания и навыки разработки драйверов блочных устройств для операционной системы Linux.

## Описание функциональности драйвера

### Use this utility to format disk partitions, create virtual disks, and test data transmission speed
## One 20MB primary partition and one extended partition containing three 10MB logical partitions.
## Use

## Инструкция по сборке

1. Просмотр разделов диска  
     sudo fdisk -l
     sudo fdisk -l | grep /dev/
     lsblk
2. Скорость передачи разделов диска при копировании файлов
    sudo dd  if=/dev/zero of=/data/file1_2 bs =1G count=2 oflag=direct
3. moddpprobe ext4
4. lsmod | grep ext4
5. sudo mmkfs.ext4 /dev/sdc



## Инструкция пользователя

1. write test program
2. Read and write the 
in the test program.
3. gcc test.c; ./a.out

## Примеры использования

1) Using register_ Blkdev() creates a block device
2) blk_ init_ Queue () allocates an application queue using and assigns the application queue processing function
3) Use alloc_ Disk() allocates a gendisk structure
4) Sets the members of the gendisk structure
->4.1) set member parameters (major, first_minor, disk_name, FOPS)
->4.2) set the queue member to be equal to the previously allocated application queue
->4.3) pass set_ Capacity() sets the capacity member equal to the number of sectors
5) Use kzalloc () to get the cache address and use it as the sector
6) Use add_ Disk() registers the gendisk structure
3.2 in the processing function of application queue
1) While recycling ELV_ next_ Request() gets each unprocessed request in the request queue
2) Use RQ_ data_ Dir() to obtain the read-write command flag of each application. 0 (read) indicates read and 1 (write) indicates write
3) Use memcp () to read or write sectors (CACHE)
4) Use end_ Request() to end each request obtained
3.3 in exit function
1) Use put_ Disk () and del_ Gendisk() to log out and release the gendisk structure
2) Use kfree() to free the disk sector cache
3) Using BLK_ cleanup_ Queue() clears the application queue in memory
4) Using unregister_ Blkdev () unload block device
