/*************************************************************************
> FileName: layout.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Aug 2024 09:44:41 AM CST
 ************************************************************************/
#ifndef _LAYOUT_H
#define _LAYOUT_H

#include "types.h"

#define magicNTFS const_cpu_to_le64 (0x202020205346544e) /* "NTFS    " */
#define NTFS_SB_MAGIC 0x5346544e /* 'NTFS' */

/*
 * Location of bootsector on partition:
 *    The standard NTFS_BOOT_SECTOR is on sector 0 of the partition.
 *    On NT4 and above there is one backup copy of the boot sector to
 *    be found on the last sector of the partition (not normally accessible
 *    from within Windows as the bootsector contained number of sectors
 *    value is one less than the actual value!).
 *    On versions of NT 3.51 and earlier, the backup copy was located at
 *    number of sectors/2 (integer divide), i.e. in the middle of the volume.
 */

/**
 * struct BIOS_PARAMETER_BLOCK - BIOS parameter block (bpb) structure.
 */
typedef struct
{
    le16 bytes_per_sector;                      /* Size of a sector in bytes. */
    u8 sectors_per_cluster;                     /* Size of a cluster in sectors. */
    le16 reserved_sectors;                      /* zero */
    u8 fats;                                    /* zero */
    le16 root_entries;                          /* zero */
    le16 sectors;                               /* zero */
    u8 media_type;                              /* 0xf8 = hard disk */
    le16 sectors_per_fat;                       /* zero */
    /*0x0d*/ le16 sectors_per_track;            /* Required to boot Windows. */
    /*0x0f*/ le16 heads;                        /* Required to boot Windows. */
    /*0x11*/ le32 hidden_sectors;               /* Offset to the start of the partition relative to the disk in sectors. Required to boot Windows. */
    /*0x15*/ le32 large_sectors;                /* zero */
    /* sizeof() = 25 (0x19) bytes */
} __attribute__ ((__packed__)) BIOS_PARAMETER_BLOCK;

/**
 * struct NTFS_BOOT_SECTOR - NTFS boot sector structure.
 */
typedef struct
{
    u8 jump[3];                                 /* Irrelevant (jump to boot up code).*/
    le64 oem_id;                                /* Magic "NTFS    ". */
    /*0x0b*/ BIOS_PARAMETER_BLOCK bpb;          /* See BIOS_PARAMETER_BLOCK. */
    u8 physical_drive;                          /* 0x00 floppy, 0x80 hard disk */
    u8 current_head;                            /* zero */
    u8 extended_boot_signature;                 /* 0x80 */
    u8 reserved2;                               /* zero */
    /*0x28*/ sle64 number_of_sectors;           /* Number of sectors in volume. Gives maximum volume size of 2^63 sectors. Assuming standard sector size of 512 bytes, the maximum byte size is approx. 4.7x10^21 bytes. (-; */
    sle64 mft_lcn;                              /* Cluster location of mft data. */
    sle64 mftmirr_lcn;                          /* Cluster location of copy of mft. */
    s8 clusters_per_mft_record;                 /* Mft record size in clusters. */
    u8 reserved0[3];                            /* zero */
    s8 clusters_per_index_record;               /* Index block size in clusters. */
    u8 reserved1[3];                            /* zero */
    le64 volume_serial_number;                  /* Irrelevant (serial number). */
    le32 checksum;                              /* Boot sector checksum. */
    /*0x54*/ u8 bootstrap[426];                 /* Irrelevant (boot up code). */
    le16 end_of_sector_marker;                  /* End of bootsector magic. Always is 0xaa55 in little endian. */
    /* sizeof() = 512 (0x200) bytes */
} __attribute__ ((__packed__)) NTFS_BOOT_SECTOR;


#endif
