#include "x86.h"

/* kernel space page tables */
struct PageDirectoryEntry kpagedir[NR_PDE_ENTRY] align_to_page;
struct PageTableEntry kpagetable[USER_MEM_HIGH / PAGE_SIZE] align_to_page;

/* user space page tables */
struct PageDirectoryEntry upagedir[NR_PROCESS][NR_PDE_ENTRY] align_to_page;
struct PageTableEntry upagetable[(USER_MEM_HIGH - USER_MEM_LOW) / PAGE_SIZE] align_to_page;

/* CR3 register */
struct CR3 cr3;

void load_cr3(uint32_t pdb) {
	/* make CR3 to be the page directory base*/
	*(uint32_t*)&cr3 = 0;
	cr3.page_directory_base = (pdb) >> 12;
}

void read_kpage(char *filename) {
	FILE *fp = fopen(filename, "r");
	int i;
	for(i = 0; i < NR_PDE_ENTRY; i ++) {
		uint32_t kpagetable_offset;
		fscanf(fp, "%u %03x\n", &kpagetable_offset,  (uint32_t*)&kpagedir[i]);
		if(kpagedir[i].present)
			kpagedir[i].page_frame = (uint32_t)(kpagetable + kpagetable_offset) >> 12;
	}

	for(i = 0; i < USER_MEM_HIGH / PAGE_SIZE; i ++) {
		fscanf(fp, "%08x\n", (uint32_t*)&kpagetable[i]);
	}
	fclose(fp);
}

void read_upage(char *filename) {
	FILE *fp = fopen(filename, "r");
	int i;
	for(i = 0; i < NR_PDE_ENTRY * NR_PROCESS; i ++) {
		uint32_t upagetable_offset;
		uint8_t flag;
		fscanf(fp, "%c %u %03x\n", &flag, &upagetable_offset,  (uint32_t*)&upagedir[0][i]);
		if(upagedir[0][i].present) {
			if(flag == 'u')
				upagedir[0][i].page_frame = (uint32_t)(upagetable + upagetable_offset) >> 12;
			else if(flag == 'k') {
				upagedir[0][i].page_frame = (uint32_t)(kpagetable + upagetable_offset) >> 12;
			}
			else
				assert(0);
		}
		else
			*(uint32_t*)&upagedir[0][i] = 0;
	}

	for(i = 0; i < (USER_MEM_HIGH - USER_MEM_LOW) / PAGE_SIZE; i ++) {
		fscanf(fp, "%08x\n", (uint32_t*)&upagetable[i]);
	}
	fclose(fp);
}

/* read page tables from files */
void init_page() {
	read_kpage("kernel-pagetable");
	read_upage("user-pagetable");

	load_cr3((uint32_t)kpagedir);
}

bool page_translation(uint8_t rw_type, uint32_t laddr, uint32_t *paddr) {
	/* implement this function to perform page translation, as well as page-level protection */

	return false;
}
