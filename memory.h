#ifndef __MEMORY_H__
#define __MEMORY_H__

#define DPL_KERNEL              0
#define DPL_USER                3

#define RPL_KERNEL              0
#define RPL_USER                3

#define SEG_WRITABLE            0x2
#define SEG_READABLE            0x2
#define SEG_EXECUTABLE          0x8

#define NR_SEGMENTS             5
#define SEG_KERNEL_CODE         1 
#define SEG_KERNEL_DATA         2
#define SEG_USER_CODE			3 
#define SEG_USER_DATA			4

#define USER_MEM_HIGH	(64 * 1024 * 1024)
#define USER_MEM_LOW	(16 * 1024 * 1024)
#define KOFFSET			0xc0000000

/* 32bit x86 uses 4KB page size */
#define PAGE_SIZE                  4096
#define NR_PDE_ENTRY               1024
#define NR_PTE_ENTRY               1024
#define PD_SIZE                    ((NR_PTE_ENTRY) * (PAGE_SIZE))

/* force the data to be aligned with page boundary.
 *    statically defined page tables uses this feature. */
#define align_to_page              __attribute((aligned(PAGE_SIZE)))

/* the 32bit Page Directory(first level page table) data structure */
struct PageDirectoryEntry {
	uint32_t present             : 1;
	uint32_t read_write          : 1; 
	uint32_t user_supervisor     : 1;
	uint32_t page_write_through  : 1;
	uint32_t page_cache_disable  : 1;
	uint32_t accessed            : 1;
	uint32_t pad0                : 6;
	uint32_t page_frame          : 20;
};

/* the 32bit Page Table Entry(second level page table) data structure */
struct PageTableEntry {
	uint32_t present             : 1;
	uint32_t read_write          : 1;
	uint32_t user_supervisor     : 1;
	uint32_t page_write_through  : 1;
	uint32_t page_cache_disable  : 1;
	uint32_t accessed            : 1;
	uint32_t dirty               : 1;
	uint32_t pad0                : 1;
	uint32_t global              : 1;
	uint32_t pad1                : 3;
	uint32_t page_frame          : 20;
};

/* the 64bit segment descriptor */
struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
};

/* the 16bit segment selector */
struct SegmentSelector {
	uint32_t request_privilege_level	: 2;
	uint32_t table_indicator			: 1;
	uint32_t descriptor_index			: 13;
};


#endif
