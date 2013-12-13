#ifndef __CPU_H__
#define __CPU_H__

/* the Control Register 3 (physical address of page directory) */
struct CR3 {
	uint32_t pad0                : 3;
	uint32_t page_write_through  : 1;
	uint32_t page_cache_disable  : 1;
	uint32_t pad1                : 7;
	uint32_t page_directory_base : 20;
};

#endif
