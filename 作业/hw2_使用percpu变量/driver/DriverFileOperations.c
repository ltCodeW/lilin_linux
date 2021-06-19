#include "DriverFileOperations.h"
#include "ToolFunctions.h"
int sum=0;
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
	DEBUG_PRINT(DEVICE_NAME ": Read invoked,return sum %d\n",sum);
	/*
	if( !copy_to_user((void*)pBuffer,&sum,sizeof(sum))){
		DEBUG_PRINT(DEVICE_NAME ": write error!\n");
		return -1;
	}
	*/
	
	*pBuffer = sum;
	
	return 0;
}

ssize_t DriverWrite(struct file *pslFileStruct, const char __user *pBuffer, size_t nCount, loff_t *pOffset)
{
	
	DEBUG_PRINT(DEVICE_NAME ": Write invoked, get parameter\n");
	int* nums = (int* )pBuffer;
	sum  = nums[0] + nums[1];
	DEBUG_PRINT(DEVICE_NAME ": in reading ,finish add %d with %d\n",nums[0],nums[1]);
	return 0;
}

long DriverIOControl(struct file *pslFileStruct, unsigned int uiCmd, unsigned long ulArg)
{

	DEBUG_PRINT(DEVICE_NAME ": ioctl invoked, do nothing\n");

	return 0;;
}

int DriverMMap(struct file *pslFileStruct, struct vm_area_struct *pslVirtualMemoryArea)
{
	DEBUG_PRINT(DEVICE_NAME ": mmap invoked, do nothing\n");
	return 0;
}
