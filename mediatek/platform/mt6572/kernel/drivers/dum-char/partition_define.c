#include <linux/module.h>
#include "partition_define.h"
/* Y220-U05 NAND layout (matches stock C482B206SP05 /proc/mtd and /proc/dumchar_info) */
struct excel_info PartInfo[PART_NUM]={
			{"preloader",524288,0x0, NAND, 0,USER},
			{"pro_info",1048576,0x80000, NAND, 0,USER},
			{"nvram",3145728,0x180000, NAND, 0,USER},
			{"protect_f",3145728,0x480000, NAND, 0,USER},
			{"protect_s",3145728,0x780000, NAND, 0,USER},
			{"seccfg",262144,0xa80000, NAND, 0,USER},
			{"uboot",524288,0xac0000, NAND, 0,USER},
			{"bootimg",6291456,0xb40000, NAND, 0,USER},
			{"recovery",6291456,0x1140000, NAND, 0,USER},
			{"sec_ro",262144,0x1740000, NAND, 0,USER},
			{"misc",786432,0x1780000, NAND, 0,USER},
			{"logo",1048576,0x1840000, NAND, 0,USER},
			{"expdb",2097152,0x1940000, NAND, 0,USER},
			{"fat",17825792,0x1b40000, NAND, 0,USER},
			{"android",229638144,0x2c40000, NAND, 0,USER},
			{"cache",69206016,0x10740000, NAND, 0,USER},
			{"usrdata",180617216,0x14940000, NAND, 0,USER},
			{"bmtpool",40,0xFFFF0028, NAND, 0,USER},
 };
EXPORT_SYMBOL(PartInfo);

#ifdef  MTK_EMMC_SUPPORT
struct MBR_EBR_struct MBR_EBR_px[MBR_COUNT]={
	{"mbr", {1, 2, 3, 4, }},
	{"ebr1", {5, 6, 7, }},
};

EXPORT_SYMBOL(MBR_EBR_px);
#endif
