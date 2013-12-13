#include "x86.h"

/* Global Descriptor Table */
static struct SegmentDescriptor gdt[NR_SEGMENTS];

struct SegmentSelector cs,	/* code segment register */
					   ds,	/* data segment register */
					   ss;	/* stack segment register */

static void set_segment(struct SegmentDescriptor *ptr, uint32_t pl, uint32_t type) {
	ptr->limit_15_0  = 0xFFFF;
	ptr->base_15_0   = 0x0;
	ptr->base_23_16  = 0x0;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = pl;
	ptr->present = 1;
	ptr->limit_19_16 = 0xF;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 1;
	ptr->base_31_24  = 0x0;
}

/* this is used for initialization and do_irq(), do_irq_finish(). DON'T use it in your code directly, because it does not provide protection */
void set_selector(struct SegmentSelector *seg_reg, uint32_t pl, uint32_t index) {
	seg_reg->request_privilege_level = pl;
	seg_reg->table_indicator = 0;
	seg_reg->descriptor_index = index;
}

void init_segment() {
	int i;
	for (i = 0; i < sizeof(gdt); i ++) {
		((uint8_t*)gdt)[i] = 0;
	}
	set_segment(&gdt[SEG_KERNEL_CODE], DPL_KERNEL, SEG_EXECUTABLE | SEG_READABLE);
	set_segment(&gdt[SEG_KERNEL_DATA], DPL_KERNEL, SEG_WRITABLE );
	set_segment(&gdt[SEG_USER_CODE], DPL_USER, SEG_EXECUTABLE | SEG_READABLE);
	set_segment(&gdt[SEG_USER_DATA], DPL_USER, SEG_WRITABLE );

	set_selector(&cs, RPL_KERNEL, SEG_KERNEL_CODE);
	set_selector(&ds, RPL_KERNEL, SEG_KERNEL_DATA);
	set_selector(&ss, RPL_KERNEL, SEG_KERNEL_DATA);
}

/* you may use this function to get CPL */
inline uint32_t read_cpl() {
	return gdt[cs.descriptor_index].privilege_level;
}

bool load_selector(struct SegmentSelector *seg_reg, uint16_t selector) {
	/* implement this function to perform segment-level protection */

	return false;
}

bool segment_translation(uint8_t access_target, uint32_t vaddr, uint32_t *laddr) {
	/* implement this function to perform segment translation */
	
	return false;
}
