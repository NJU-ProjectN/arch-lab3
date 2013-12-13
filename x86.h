#ifndef __X86_H__
#define __X86_H__

#include "types.h"
#include "cpu.h"
#include "memory.h"
#include <assert.h>
#include <stdio.h>

/* define access targets */
#define ACCESS_CODE		'c'
#define ACCESS_DATA		'd'
#define ACCESS_STACK	's'

/* define read, write types */
#define ACCESS_READ		'r'
#define ACCESS_WRITE	'w'

/* assume there are 2 processes in the system */
#define NR_PROCESS 2

extern struct SegmentSelector cs, ds, ss;

void init_segment();
void init_page();

bool load_selector(struct SegmentSelector *, uint16_t);
bool segment_translation(uint8_t, uint32_t, uint32_t *);
bool page_translation(uint8_t, uint32_t, uint32_t *);

inline uint32_t read_cpl();
extern uint32_t current_process;

void do_irq();
void do_irq_finish();


#endif

