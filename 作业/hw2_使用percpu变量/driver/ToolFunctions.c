#include <linux/string.h>
#include "ToolFunctions.h"

void MEM_PRINT(unsigned long addr, unsigned long size)
{
	unsigned long i, j;
	char AddrBuf[64];
	char DataBuf[32];
	unsigned char *p = (unsigned char *)addr;

	for(i = 0; i < size; )
	{
		snprintf(AddrBuf, 64, "%lx:\t", (unsigned long)p);

		for(j = 0; j < 8; j++)
		{  
			if(i + j >= size)
				break;

			if(*(p+j) <= 0xf)
				snprintf(DataBuf + j * 3, 32 - j * 3, "0%x ", *(p+j));
			else
				snprintf(DataBuf + j * 3, 32 - j * 3, "%x ", *(p+j));
		}

		strcat(AddrBuf, DataBuf);

		printk("%s\n", AddrBuf);

		i = i + 8;
		p = p + 8;
	}
}
