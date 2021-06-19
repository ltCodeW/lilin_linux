#include "ToolFunctions.h"

#define LINE_BUFFER_LENGTH 64

void MEM_PRINT(unsigned long addr, unsigned long size)
{
	unsigned long i, j, k;
	char buf[LINE_BUFFER_LENGTH];
	char *q;
	unsigned char *p = (unsigned char *)addr;

	for (i = 0; i < size;)
	{
	    	q = buf;
		k = snprintf(q, LINE_BUFFER_LENGTH, "%lx:\t", (unsigned long)p);
		q = q + k;

		for (j = 0; j < 8; j++)
		{
			if (i + j >= size)
				break;

			if (*(p + j) <= 0xf)
			    	k = snprintf(q, LINE_BUFFER_LENGTH - (q - buf), "0%X ", *(p + j));
			else
				k = snprintf(q, LINE_BUFFER_LENGTH - (q - buf), "%X ", *(p + j));

			q = q + k;
		}

		printk("%s", buf);

		i = i + 8;
		p = p + 8;
	}
}
