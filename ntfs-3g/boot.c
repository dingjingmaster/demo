/*************************************************************************
> FileName: boot.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Aug 2024 09:42:44 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#include "layout.h"


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入要读取的NTFS设备\n");
        return -1;
    }

    // 检测设备是否存在
    

    int fr = 0;
    NTFS_BOOT_SECTOR* bs = NULL;

    printf ("NTFS Boot Sector size: %d\n", sizeof(NTFS_BOOT_SECTOR));

    bs = (NTFS_BOOT_SECTOR*) g_malloc0 (sizeof(NTFS_BOOT_SECTOR));
    if (!bs) {
        printf ("malloc error!\n");
        goto end;
    }

    fr = open (argv[1], O_RDONLY);
    if (fr < 0) {
        printf("open error\n");
        goto end;
    }

    if (sizeof(NTFS_BOOT_SECTOR) != read(fr, bs, sizeof(NTFS_BOOT_SECTOR))) {
        printf("read boot error\n");
        goto end;
    }

#define _OF(x) \
    (0), (sizeof(x) - 1)

#define _OFFSET(x) offsetof(NTFS_BOOT_SECTOR, x)
#define _OFFSET_PARAM(x) offsetof(BIOS_PARAMETER_BLOCK, x)

#define _OF_L(tis, param) \
    _OFFSET(tis), _OFFSET(tis) + sizeof(param) - 1

#define _OF_PARAM_L(tis, param) \
    _OFFSET(bpb) + _OFFSET_PARAM(tis), _OFFSET(bpb) + _OFFSET_PARAM(tis) + sizeof(param) - 1

    printf("[0x%03X - 0x%03X] jump                                    : 0x%03X      // 告知BIOS在启动时候到哪里执行启动代码\n", _OF(bs->jump), (u8)(*(bs->jump)));
    printf("[0x%03X - 0x%03X] Magic                                   : 0x%03X // 标识NTFS的特殊标记\n", _OF_L(oem_id, bs->oem_id), bs->oem_id);
    printf("[0x%03X - 0x%03X] <BIOS_PARAMETER_BLOCK>\n", _OF_L(bpb, bs->bpb));
    printf("  [0x%03X - 0x%03X]   bytes_per_sector                    :0x%03X(%-3d) // 每个扇区字节数(磁盘以扇区为基本单位读写数据，NTFS组织结构也以扇区为单位) \n", _OF_PARAM_L(bytes_per_sector, bs->bpb.bytes_per_sector), bs->bpb.bytes_per_sector, bs->bpb.bytes_per_sector);
    printf("  [0x%03X - 0x%03X]   sectors_per_cluster                 :0x%03X(%-3d) // 每个簇包含的扇区数，NTFS存储文件以簇为单位，一个簇由多个连续扇区组成\n", _OF_PARAM_L(sectors_per_cluster, bs->bpb.sectors_per_cluster), bs->bpb.sectors_per_cluster, bs->bpb.sectors_per_cluster);
    printf("  [0x%03X - 0x%03X]   reserved_sectors                    :0x%03X(%-3d) // NTFS启动分区中被保留不用的扇区数量\n", _OF_PARAM_L(reserved_sectors, bs->bpb.reserved_sectors), bs->bpb.reserved_sectors, bs->bpb.reserved_sectors);
    printf("  [0x%03X - 0x%03X]   fats                                :0x%03X(%-3d) // fats(File Allocation Table) 文件系统中文件分配表的数量。跟踪文件分配情况。\n", _OF_PARAM_L(fats, bs->bpb.fats), bs->bpb.fats, bs->bpb.fats);
    printf("  [0x%03X - 0x%03X]   root_entries                        :0x%03X(%-3d) // 根目录可容纳的最大项目数。根目录很特殊，它存储了整个文件系统的顶层目录和文件。早期根目录是一个固定大小的结构，因此这个参数很重要，新NTFS中，根目录不再是固定大小，而是动态可扩展。\n", _OF_PARAM_L(root_entries, bs->bpb.root_entries), bs->bpb.root_entries, bs->bpb.root_entries);
    printf("  [0x%03X - 0x%03X]   sectors                             :0x%03X(%-3d) // 整个NTFS启动分区包含的扇区数量。\n", _OF_PARAM_L(sectors, bs->bpb.sectors), bs->bpb.sectors, bs->bpb.sectors);
    printf("  [0x%03X - 0x%03X]   media_type                          :0x%03X(%-3d) // 用来描述文件系统所在的存储介质类型：(0xF0,软盘;0xF8,硬盘;0xF9,软盘或硬盘;0xFA软盘或软驱)\n", _OF_PARAM_L(media_type, bs->bpb.media_type), bs->bpb.media_type, bs->bpb.media_type);
    printf("  [0x%03X - 0x%03X]   sectors_per_fat                     :0x%03X(%-3d) // 描述每个Master File Table(MFT)副本所占用的扇区数量。\n", _OF_PARAM_L(sectors_per_fat, bs->bpb.sectors_per_fat), bs->bpb.sectors_per_fat, bs->bpb.sectors_per_fat);
    printf("  [0x%03X - 0x%03X]   sectors_per_track                   :0x%03X(%-3d) // 用来描述每个磁道（track）上包含的扇区数量。\n", _OF_PARAM_L(sectors_per_track, bs->bpb.sectors_per_track), bs->bpb.sectors_per_track, bs->bpb.sectors_per_track);
    printf("  [0x%03X - 0x%03X]   heads                               :0x%03X(%-3d) // 每个磁柱包含的磁头数量。\n", _OF_PARAM_L(heads, bs->bpb.heads), bs->bpb.heads, bs->bpb.heads);
    printf("  [0x%03X - 0x%03X]   hidden_sectors                      :0x%03X(%-3d) // 开始扇区之前隐藏的扇区数量（开始扇区之前，可能会有一些隐藏扇区，这些扇区包含启动代码、分区表等关键系统信息）。这个参数作用是方便操作系统在访问NTFS时候，能正确定位文件系统的起始位置。\n", _OF_PARAM_L(hidden_sectors, bs->bpb.hidden_sectors), bs->bpb.hidden_sectors, bs->bpb.hidden_sectors);
    printf("  [0x%03X - 0x%03X]   large_sectors                       :0x%03X(%-3d) // 描述每个扇区的大小(以字节为单位)。这个参数让操作系统访问NTFS文件系统时候能正确理解和处理每个扇区的数据。\n", _OF_PARAM_L(large_sectors, bs->bpb.large_sectors), bs->bpb.large_sectors, bs->bpb.large_sectors);
    printf("[0x%03X - 0x%03X] physical_drive                          :0x%03X // 设备类型(0x00,软盘;0x80,硬盘)\n", _OF_L(physical_drive, bs->physical_drive), bs->physical_drive);
    printf("[0x%03X - 0x%03X] current_head                            :0x%03X // 描述当前正在使用的磁头编号。知道了当前磁头编号，可以精确计算数据所在的磁道和磁头位置，从而高效执行磁盘读写操作。对与SSD此参数不再重要。\n", _OF_L(current_head, bs->current_head), bs->current_head);
    printf("[0x%03X - 0x%03X] extended_boot_signature                 :0x%03X // 用来指示是否存在额外的引导扇区信息。NTFS的boot分区中，除了标准的BIOS引导扇区外，还可能存在额外的引导代码和信息。如果这个值为 0x80 或 0xAA55 表示存在额外的引导扇区信息。这些额外的引导扇区信息可能包括启动代码、分区表、GUID分区表(GPT)等关键系统信息。如果这个值被设置了，就要继续解析引导扇区信息，以确保文件系统能够正确启动和挂载。\n", _OF_L(extended_boot_signature, bs->extended_boot_signature), bs->extended_boot_signature);
    printf("[0x%03X - 0x%03X] reserved2                               :0x%03X // 保留字段，用于未来可能的扩展。\n", _OF_L(reserved2, bs->reserved2), bs->reserved2);
    printf("[0x%03X - 0x%03X] number_of_sectors                       :0x%03X(%-3d) // 主分区本身的扇区数量。????\n", _OF_L(number_of_sectors, bs->number_of_sectors), bs->number_of_sectors, bs->number_of_sectors);
    printf("[0x%03X - 0x%03X] mft_lcn                                 :0x%03X(%-3d) // 主文件表(MFT)的逻辑集群号。NTFS使用一种主文件表的特殊数据结构来跟踪和管理所有文件和文件夹。MFT也是一个文件，存储在NTFS分区中。mft_lcn参数记录MFT文件在NTFS分区中的起始逻辑集群号。逻辑集群号(LCN)用来表示文件或数据在NTFS分区中的位置是一种方式。它指定了文件/数据在分区中从哪个集群开始存储。操作系统在访问NTFS文件系统时候，需要先定位到MFT文件的位置，然后才能查找和访问其它文件。\n", _OF_L(mft_lcn, bs->mft_lcn), bs->mft_lcn, bs->mft_lcn);
    printf("[0x%03X - 0x%03X] mftmirr_lcn                             :0x%03X(%-3d)\n", _OF_L(mftmirr_lcn, bs->mftmirr_lcn), bs->mftmirr_lcn, bs->mftmirr_lcn);
    printf("[0x%03X - 0x%03X] clusters_per_mft_record                 :0x%03X(%-3d) // 描述了每个文件表(MFT)记录所占用的集群数量。在NTFS中，所有文件和目录的元数据信息都存储在主文件表(MFT)中。MFT是一个特殊文件，它由许多固定大小的记录组成。每个记录都代表了一个文件或目录。这个参数用来记录每个MFT记录所占用的cluster数量。clusters是NTFS中最小的存储单元，通常为4KB或更大。由于MFT记录的大小通常超过一个cluster，因此需要使用多个cluster来存储单个MFT记录（这个值如果是负数则需要特殊处理：1 << -xx，其中xx表示此值）。\n", _OF_L(clusters_per_mft_record, bs->clusters_per_mft_record), bs->clusters_per_mft_record, bs->clusters_per_mft_record);
    printf("[0x%03X - 0x%03X] reserved0[3]                            :0x%03X%03X%03X\n", _OF_L(reserved0, bs->reserved0), bs->reserved0[0], bs->reserved0[1], bs->reserved0[2]);
    printf("[0x%03X - 0x%03X] clusters_per_index_record               :0x%03X(%-3d) // 描述了每个索引记录所占用的cluster数量。NTFS中使用索引结构来存储和管理文件夹（目录）中的文件和子目录信息。这些索引数据存储在NTFS分区中的索引记录中。此参数用来指定每个索引记录所占用的cluster数量。cluster是NTFS中最小的存储单元，通常为4KB或更大。\n", _OF_L(clusters_per_index_record, bs->clusters_per_index_record), bs->clusters_per_index_record, bs->clusters_per_index_record);
    printf("[0x%03X - 0x%03X] reserved1[3]                            :0x%03X%03X%03X\n", _OF_L(reserved1, bs->reserved1), bs->reserved1[0], bs->reserved1[1], bs->reserved1[2]);
    printf("[0x%03X - 0x%03X] volume_serial_number                    :0x%03X(%-3d) // 表示卷/分区的序列号。NTFS中每个分区都有一个唯一的序列号，这个序列号用来唯一标识每个NTFS分区。\n", _OF_L(volume_serial_number, bs->volume_serial_number), bs->volume_serial_number, bs->volume_serial_number);
    printf("[0x%03X - 0x%03X] checksum                                :0x%03X(%-3d) // 存储文件系统元数据的完整性校验值，用来检验这些数据是否完整。这些元数据结构包括：主文件表(MFT)、索引缓存、卷信息等\n", _OF_L(checksum, bs->checksum), bs->checksum, bs->checksum);
    printf("[0x%03X - 0x%03X] <bootstrap>\n", _OF_L(bootstrap, bs->bootstrap));
    printf("[0x%03X - 0x%03X] end_of_sector_marker                    :0x%03X\n", _OF_L(end_of_sector_marker, bs->end_of_sector_marker), bs->end_of_sector_marker);
    printf("\n");


end:
    if (bs)     g_free(bs);
    if (fr)     close(fr);

    return 0;
}
