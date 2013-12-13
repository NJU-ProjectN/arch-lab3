#include "x86.h"

uint32_t current_process;

extern void set_selector(struct SegmentSelector *, uint32_t, uint32_t);

extern struct PageDirectoryEntry upagedir[NR_PROCESS][NR_PDE_ENTRY];
extern void load_cr3(uint32_t);

static void iret() {
	set_selector(&cs, RPL_USER, SEG_USER_CODE);
	set_selector(&ss, RPL_USER, SEG_USER_DATA);
}

static void schedule() {
	current_process = !current_process;
	load_cr3((uint32_t)upagedir[current_process]);
}

void do_irq() {
	set_selector(&ss, RPL_KERNEL, SEG_KERNEL_DATA);
	set_selector(&cs, RPL_KERNEL, SEG_KERNEL_CODE);
	/* now the system is in kernel mode */
}

void do_irq_finish() {
	schedule();
	iret();
	/* now the system is in user mode */
}
