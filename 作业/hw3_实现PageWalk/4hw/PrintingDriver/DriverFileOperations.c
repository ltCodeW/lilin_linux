#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <asm/atomic.h>
#include <linux/rwsem.h>
#include <asm/uaccess.h>
#include <asm/desc.h>
#include <asm/page.h>
#include <asm/special_insns.h>

#include "DriverFileOperations.h"
#include "DriverMain.h"
#include "ToolFunctions.h"
#include "IoCtlSupport.h"

int DriverOpen(struct inode *pslINode, struct file *pslFileStruct)
{
	DEBUG_PRINT(DEVICE_NAME ": open invoked, do nothing\n");
	return 0;
}

int DriverClose(struct inode *pslINode, struct file *pslFileStruct)
{
	DEBUG_PRINT(DEVICE_NAME ": close invoked, do nothing\n");
	return 0;
}

ssize_t DriverRead(struct file *pslFileStruct, char __user *pBuffer, size_t nCount, loff_t *pOffset)
{
	DEBUG_PRINT(DEVICE_NAME ": read invoked, do nothing\n");
	return 0;
}


void trace_addr(unsigned long addr){
	unsigned long ulCR3;//
	unsigned long ulPML4TPhysAddr;//
	unsigned long ulPDPTPhysAddr;//
	unsigned long ulPDTPhysAddr;
	unsigned long ulPTPhysAddr;
	unsigned long ulPagePhysAddr;
	
	

	unsigned long ulPML4TIndex= (addr>>39)&0x1FFUL;
	unsigned long ulPDPTIndex= (addr>>30)&0x1FFUL;
	unsigned long ulPDTIndex= (addr>>21)&0x1FFUL;
	unsigned long ulPTTIndex = (addr>>12)&0x1FFUL;

	unsigned long ulPML4TE;
	unsigned long ulPDPTE;
	unsigned long ulPDE;
	unsigned long ulPTE;
	unsigned long ulcontent;
	
	
	
	//64byte 4kb page
	unsigned long physical_idx_4k = addr&0xFFFUL;
	//64byte 2mb page
	unsigned long physical_idx_2m = addr &0x1FFFFFUL;
	//64byte 1gb page
	unsigned long physical_idx_1g = addr&0x3FFFFFFFUL;
	
	unsigned long pysical2linear = 0xffff880000000000UL;

	asm volatile("mov %%cr3, %0\n\t" : "=r" (ulCR3), "=m" (__force_order));
	//虽然是64位系统，但物理地址最长实现目前是52位
	//不能假定未来不会扩展到64位
	ulPML4TPhysAddr = ulCR3&0xfffffffffffff000UL;
	ulPML4TE =  *((unsigned long *)(__va(ulPML4TPhysAddr + ulPML4TIndex * 8)));
	DEBUG_PRINT(DEVICE_NAME " ulPML4TE  0x%lx\n", ulPML4TE);
	DEBUG_PRINT(DEVICE_NAME " ulPML4TE R/W  %ld\n", (ulPML4TE>>1)&1);
	DEBUG_PRINT(DEVICE_NAME " ulPML4TE  P %ld\n", ulPML4TE&1);
	//各级页表项最高位，其实指示了对应的页面是否不准执行，即是否能让RIP指向其中
	//最高位显然就不是地址的一部分
	ulPDPTPhysAddr = ulPML4TE&0x7ffffffffffff000UL;
	ulPDPTE = *((unsigned long *)(__va(ulPDPTPhysAddr + ulPDPTIndex * 8)));
	DEBUG_PRINT(DEVICE_NAME " ulPDPTE  0x%lx\n", ulPDPTE);
	DEBUG_PRINT(DEVICE_NAME " ulPDPTE R/W  %ld\n", (ulPDPTE>>1)&1);
	DEBUG_PRINT(DEVICE_NAME " ulPDPTE  P %ld\n", ulPDPTE&1);
	if( (ulPDPTE>>7) & 1){//PDPTE ps 1,1gb page
		ulPagePhysAddr = ulPDPTE & 0x7FFFFFFFFFEUL;
		ulcontent  = *((unsigned long *)(__va(ulPagePhysAddr + physical_idx_1g)));
		DEBUG_PRINT(DEVICE_NAME "-----physical addr  %p--------\n",__va(ulPagePhysAddr + physical_idx_1g));
		DEBUG_PRINT(DEVICE_NAME "-----content  0x%lx--------\n",ulcontent);
	}else{ //PDPTE ps 0,2kb/4kb page
		//ulPDTPhysAddr = ulPDPTE&0xFFFFFFFFFF000UL;
		ulPDTPhysAddr = ulPDPTE&0x7ffffffffffff000UL;
		ulPDE =  *((unsigned long *)(__va(ulPDTPhysAddr + ulPDTIndex * 8)));
		DEBUG_PRINT(DEVICE_NAME " ulPDE R/W  %ld\n", (ulPDE>>1)&1);
		DEBUG_PRINT(DEVICE_NAME " ulPDE  P %ld\n", ulPDE&1);
		if((ulPDE >> 7)&1){//PDE ps 1 ,2mb page
			ulPagePhysAddr =  ulPDE & 0x7FFFFFFFFFE00000UL;
			ulcontent  = *((unsigned long *)(__va(ulPagePhysAddr + physical_idx_2m)));
			DEBUG_PRINT(DEVICE_NAME "-----physical addr  %p--------\n",__va(ulPagePhysAddr + physical_idx_2m));
			DEBUG_PRINT(DEVICE_NAME "-----content  0x%lx--------\n",ulcontent);

		}else{//PDE ps 0 ,4kb page
			//ulPTPhysAddr = ulPDE&0xFFFFFFFFFF000UL;
			ulPTPhysAddr = ulPDE&0x7ffffffffffff000UL;
			ulPTE = *((unsigned long *)(__va(ulPTPhysAddr + ulPTTIndex * 8)));
			DEBUG_PRINT(DEVICE_NAME " ulPDE R/W  %ld\n", (ulPTE>>1)&1);
			DEBUG_PRINT(DEVICE_NAME " ulPDE  P %ld\n", ulPTE&1);
			
			//ulPagePhysAddr =  ulPTE&0xFFFFFFFFFF000UL;
			ulPagePhysAddr = ulPTE&0x7ffffffffffff000UL;
			ulcontent  = *((unsigned long *)(__va(ulPagePhysAddr + physical_idx_4k)));
			DEBUG_PRINT(DEVICE_NAME "-----physical addr  %p--------\n",__va(ulPagePhysAddr + physical_idx_4k));
			DEBUG_PRINT(DEVICE_NAME "-----content  0x%lx--------\n",ulcontent);
		}
	
	}
	
}
ssize_t DriverWrite(struct file *pslFileStruct, const char __user *pBuffer, size_t nCount, loff_t *pOffset)
{	
	DEBUG_PRINT(DEVICE_NAME ": write invoked, do nothing\n");

	char gdtr[10] = {0};
	asm volatile ("sgdt %0\n\t" : "=m"(gdtr) : :);
	//gdtr high 32 byte is GDT table base;low 16 byte is limit
	
	//get unsigned long type gdt addr from gdtr
	unsigned long gdt_addr_fromGDTR = gdtr[9];
	int i=9;
	for(;i!=2;i--){
	gdt_addr_fromGDTR |=gdtr[i];
	gdt_addr_fromGDTR <<= 8;
	}
	gdt_addr_fromGDTR |=gdtr[i];
	DEBUG_PRINT(DEVICE_NAME " gdt_addr_fromGDTR = 0x%lx\n", gdt_addr_fromGDTR);
	DEBUG_PRINT(DEVICE_NAME " gdt_addr_fromGDTR trace\n");
	trace_addr(gdt_addr_fromGDTR);
	

	unsigned int gs_base_high, gs_base_low;
	asm volatile ("mov $0xc0000101, %%ecx \n\t"
		"rdmsr \n\t" : "=d"(gs_base_high), "=a"(gs_base_low) : : "%ecx", "memory");

	unsigned long gs_base = ((unsigned long)gs_base_high << 32) + gs_base_low;

	unsigned long gdt_offset = (unsigned long)&gdt_page;
	
	unsigned long gdt_addr_fromGS = gs_base + gdt_offset;
	DEBUG_PRINT(DEVICE_NAME " gdt_addr_fromGS = 0x%lx\n", gdt_addr_fromGS);
	DEBUG_PRINT(DEVICE_NAME " gdt_addr_fromGS trace\n");
	trace_addr(gdt_addr_fromGS);

	return 0;
}

long DriverIOControl(struct file *pslFileStruct, unsigned int uiCmd, unsigned long ulArg)
{
	DEBUG_PRINT(DEVICE_NAME ": ioctl invoked, do nothing\n");
	return 0;
}

int DriverMMap(struct file *pslFileStruct, struct vm_area_struct *pslVirtualMemoryArea)
{
	DEBUG_PRINT(DEVICE_NAME ": mmap invoked, do nothing\n");
	return 0;
}
