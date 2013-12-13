#include "x86.h"

void addr_translation(uint8_t target, uint8_t rw_type, uint32_t vaddr) {
	uint32_t laddr, paddr;
	if(segment_translation(target, vaddr, &laddr) == false) {
		// '#' here is to distinguish this output message from the actual "Segmentation fault"
		printf("# Segmentation fault\n");	
		return;
	}

	if(page_translation(rw_type, laddr, &paddr) == false) {
		// '#' here is to distinguish this output message from the actual "Segmentation fault"
		printf("# Segmentation fault\n");
		return;
	}

	printf("0x%08x --> 0x%08x\n", vaddr, paddr);
}

void try_load_selector(struct SegmentSelector *seg_reg, uint16_t selector) {
	if(load_selector(seg_reg, selector) == false) {
		// '#' here is to distinguish this output message from the actual "Segmentation fault"
		printf("# Segmentation fault\n");	
		return;
	}
	printf("change DS to 0x%04x\n", selector);
}

void execute_user_code() {
	FILE *fp = fopen("user-code", "r");
	assert(fp);
	char access_target, rw_type;
	uint32_t vaddr;
	while( fscanf(fp, "%x %c %c", &vaddr, &access_target, &rw_type) != EOF ) {
		switch(rw_type) {
			case 'r':
			case 'w': 
				assert(access_target == 'c' || access_target == 'd' || access_target == 's');
				addr_translation(access_target, rw_type, vaddr);
				break;
			case 'l': try_load_selector(&ds, vaddr); break;
			case '*': printf("an interrupt comes!\n"); do_irq(); break;
			case '#': printf("finish handling interrupt!\n"); do_irq_finish(); break;
			default: assert(0);
		}
	}

	fclose(fp);
}

void execute_code() {
	/* when we are here, we are still in kernel. */

	printf("start kernel initialization...\n");
	/* execute some kernel code */
	addr_translation('c', 'r', 0xc0100000);
	addr_translation('c', 'r', 0xc0100004);
	addr_translation('c', 'r', 0xc0100008);

	/* ...... */

	/* access kernel data */
	addr_translation('d', 'r', 0xc0101000);
	addr_translation('d', 'w', 0xc0101004);
	/* try a NULL pointer! */
	addr_translation('d', 'r', (uint32_t)NULL);

	/* ...... */

	/* access kernel stack */
	addr_translation('s', 'r', 0xc0102000);
	addr_translation('s', 'w', 0xc0102004);

	/* kernel initialization finish, wait for an interrupt */
	/* ...... */

	/* an interrupt comes! */
	do_irq();
	/* handle the interrupt */
	addr_translation('c', 'r', 0xc0110000);
	addr_translation('d', 'r', 0xc0111000);

	/* ...... */

	/* finish handling the interrupt, now we will go to the user mode! */
	do_irq_finish();
	
	printf("start user code...\n");
	execute_user_code();
}

int main() {
	init_segment();
	init_page();

	/* this is the initialization of the variable. */
	current_process = 0;	

	/* we are still in kernel, since all segment we use are in ring 0. */
	
	/* here we go! */
	execute_code();

	printf("simulation end!\n");

	return 0;
}
