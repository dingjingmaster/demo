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
    (0), (sizeof(x))

#define _OFFSET(x) offsetof(NTFS_BOOT_SECTOR, x)

#define _OF_L(tis, param) \
    _OFFSET(tis), _OFFSET(tis) + sizeof(param)

    printf("[0x%03X - 0x%03X] <jump>\n", _OF(bs->jump));
    printf("[0x%03X - 0x%03X] Magic                                   : 0x%X\n", _OF_L(oem_id, bs->oem_id), bs->oem_id);
    printf("[0x%03X - 0x%03X] <BIOS_PARAMETER_BLOCK>\n", _OF_L(bpb, bs->bpb));
    printf("[0x%03X - 0x%03X] 设备类型(0x00,floppy; 0x80,hard disk)   :0x%X\n", _OF_L(physical_drive, bs->physical_drive), bs->physical_drive);
    printf("[0x%03X - 0x%03X] current_head                            :0x%X\n", _OF_L(current_head, bs->current_head), bs->current_head);
    printf("[0x%03X - 0x%03X] extended_boot_signature                 :0x%X\n", _OF_L(extended_boot_signature, bs->extended_boot_signature), bs->extended_boot_signature);
    printf("[0x%03X - 0x%03X] reserved2                               :0x%X\n", _OF_L(reserved2, bs->reserved2), bs->reserved2);
    printf("[0x%03X - 0x%03X] number_of_sectors                       :0x%X(%d)\n", _OF_L(number_of_sectors, bs->number_of_sectors), bs->number_of_sectors, bs->number_of_sectors);
    printf("[0x%03X - 0x%03X] mft_lcn                                 :0x%X(%d)\n", _OF_L(mft_lcn, bs->mft_lcn), bs->mft_lcn, bs->mft_lcn);
    printf("[0x%03X - 0x%03X] mftmirr_lcn                             :0x%X(%d)\n", _OF_L(mftmirr_lcn, bs->mftmirr_lcn), bs->mftmirr_lcn, bs->mftmirr_lcn);
    printf("[0x%03X - 0x%03X] clusters_per_mft_record                 :0x%X(%d)\n", _OF_L(clusters_per_mft_record, bs->clusters_per_mft_record), bs->clusters_per_mft_record, bs->clusters_per_mft_record);
    printf("[0x%03X - 0x%03X] reserved0[3]                            :0x%X%X%X\n", _OF_L(reserved0, bs->reserved0), bs->reserved0[0], bs->reserved0[1], bs->reserved0[2]);
    printf("[0x%03X - 0x%03X] clusters_per_index_record               :0x%X(%d)\n", _OF_L(clusters_per_index_record, bs->clusters_per_index_record), bs->clusters_per_index_record, bs->clusters_per_index_record);
    printf("[0x%03X - 0x%03X] reserved1[3]                            :0x%X%X%X\n", _OF_L(reserved1, bs->reserved1), bs->reserved1[0], bs->reserved1[1], bs->reserved1[2]);
    printf("[0x%03X - 0x%03X] volume_serial_number                    :0x%X(%d)\n", _OF_L(volume_serial_number, bs->volume_serial_number), bs->volume_serial_number, bs->volume_serial_number);
    printf("[0x%03X - 0x%03X] checksum                                :0x%X(%d)\n", _OF_L(checksum, bs->checksum), bs->checksum, bs->checksum);
    printf("[0x%03X - 0x%03X] <bootstrap>\n", _OF_L(bootstrap, bs->bootstrap));
    printf("[0x%03X - 0x%03X] end_of_sector_marker                    :0x%X\n", _OF_L(end_of_sector_marker, bs->end_of_sector_marker), bs->end_of_sector_marker);


end:
    if (bs)     g_free(bs);
    if (fr)     close(fr);

    return 0;
}
