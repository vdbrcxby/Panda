
/*******************************************************************************
 *
 *      @name   : hd.c
 *
 *      @author : Yukang Chen (moorekang@gmail.com)
 *      @date   : 2012-05-28 21:51:02
 *
 *      @brief  :
 *
 *******************************************************************************/

#include <asm.h>
#include <system.h>
#include <string.h>
#include <hd.h>

//static	u8 hdbuf[SECTOR_SIZE * 2];
static int wait_for_ready(void)
{
    int retries = 100000;
    while( --retries && (inb(HD_STAT) & 0xc0) != 0x40)
        ;
    return retries;
}

void hd_interupt_handler(void)
{
    wait_for_ready();
    return ;
}


void do_hd_cmd(struct hd_cmd* cmd) {
    wait_for_ready();
    outb(0x3F6, 0); //interrupt
    /* Activate the Interrupt Enable (nIEN) bit */
	outb(0x1F1, cmd->feature);
	outb(0x1F2,  cmd->count);
	outb(0x1F3,  cmd->lba_low);
	outb(0x1F4,  cmd->lba_mid);
	outb(0x1F5, cmd->lba_high);
	outb(0x1F6,   cmd->device);
	/* Write the command code to the Command Register */
	outb(0x1F7,     cmd->command);
}

#if 0
static void print_identify_info(u16* hdinfo) {
	int i;
    unsigned k;
	char s[64];

	struct iden_info_ascii {
		int idx;
		int len;
		char * desc;
	} iinfo[] = {{10, 20, "HD SN"}, /* Serial number in ASCII */
                 {27, 40, "HD Model"} /* Model number in ASCII */ };

	for (k = 0; k < sizeof(iinfo)/sizeof(iinfo[0]); k++) {
		char * p = (char*)&hdinfo[iinfo[k].idx];
		for (i = 0; i < iinfo[k].len/2; i++) {
			s[i*2+1] = *p++;
			s[i*2] = *p++;
		}
		s[i*2] = 0;
		printk("%s: %s\n", iinfo[k].desc, s);
	}

	int capabilities = hdinfo[49];
	printk("LBA supported: %s\n",
	       (capabilities & 0x0200) ? "Yes" : "No");

	int cmd_set_supported = hdinfo[83];
	printk("LBA48 supported: %s\n",
	       (cmd_set_supported & 0x0400) ? "Yes" : "No");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	printk("sectors: %d HD size: %d MB\n", sectors, sectors * 512 / 1000000);
}
#endif

void init_hd()
{
    /* get the number of divers, from the BIOS data area */
    u8* num_of_driver = (u8*)(0x475);
    printk("NrDrivers: %d\n", (*num_of_driver) );

    struct hd_cmd cmd;
    cmd.device = MAKE_DEVICE_REG(0, 0, 0);
    do_hd_cmd(&cmd);
    wait_for_ready();
    //port_read(0x1F0, hdbuf, SECTOR_SIZE);
    //print_identify_info((u16*)hdbuf);
    
    //irq_install_handler(14, (isq_t)(&hd_interupt_handler));
}



