#include <cstdio>
int main(){
	char gdtr[10]={};
	gdtr[9]=0xFF;
	gdtr[8]=0xFF;
	gdtr[7]=0xFE;
	gdtr[6]=0x00;
	gdtr[5]=0x00;
	gdtr[4]=0x00;
	gdtr[3]=0x10;
	gdtr[2]=0x00;
	gdtr[1]=0x00;
	gdtr[0]=0x7F;
	unsigned long gdt_addr_fromGDTR = 0;
	int i=9;
	for(;i!=2;i--){
	gdt_addr_fromGDTR |=gdtr[i];
	gdt_addr_fromGDTR <<= 8;
	printf(" 0x%lx:0x%lx\n",gdtr[i],gdt_addr_fromGDTR);
	}
	gdt_addr_fromGDTR |=gdtr[i];
	printf(" gdt_addr_fromGDTR = 0x%lx\n",gdt_addr_fromGDTR);
}