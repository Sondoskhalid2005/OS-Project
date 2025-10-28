/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//
//==================================
// [1] GET PAGE VA:
//==================================
__inline__ uint32 to_page_va(struct PageInfoElement *ptrPageInfo)
{
	//Get start VA of the page from the corresponding Page Info pointer
	int idxInPageInfoArr = (ptrPageInfo - pageBlockInfoArr);
	return dynAllocStart + (idxInPageInfoArr << PGSHIFT);
}
//////////////////////////////////////////////////////////////////////////////////////
// [2] LOG 2 FUNCTION I GOT FROM UTILITIES AND MODIFIED
inline unsigned int log2_ceil(unsigned int x) {
	if (x <= 1) return 1;
	//int power = 2;
	int bits_cnt = 0 ; /// modified here, it was = 2 i made it = 0
	x--;
	while (x >>= 1) {
		//power <<= 1;
		bits_cnt++ ;
	}
	return bits_cnt;
}

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
bool is_initialized = 0;
void initialize_dynamic_allocator(uint32 daStart, uint32 daEnd)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert(daEnd <= daStart + DYN_ALLOC_MAX_SIZE);
		is_initialized = 1;
	}
	//==================================================================================
	//==================================================================================
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #1 initialize_dynamic_allocator
	
	dynAllocStart =daStart;
	dynAllocEnd = daEnd;
	uint32 daSize;
	ROUNDUP_POWERTWO(daEnd - daStart, daSize);
	int pages_no = daSize /PAGE_SIZE;
	LIST_INIT(&freePagesList); 
	uint32 start_address = dynAllocStart;
	struct PageInfoElement * PIOptr;
    for (int i = 0; i < pages_no; i++)
	{
		get_page(start_address);
		pageBlockInfoArr[i].block_size=0;
		pageBlockInfoArr[i].num_of_free_blocks=0;
		PIOptr = &pageBlockInfoArr[i];
		///NO TRUE MAPPING YET ! pageBlockInfoArr[n]=> daStart+ n * PAGE_SIZE;
		LIST_INSERT_TAIL(&freePagesList, PIOptr);
		start_address+=PAGE_SIZE;
	}
	for (int i = 0; i < 9; i++)
	{
		LIST_INIT(&freeBlockLists[i]);
	}
	
  
	//Comment the following line
	// panic("initialize_dynamic_allocator() Not implemented yet");

}

//===========================
// [2] GET BLOCK SIZE:
//===========================
__inline__ uint32 get_block_size(void *va)
{
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #2 get_block_size
	//Your code is here
	//Comment the following line
	panic("get_block_size() Not implemented yet");
}

//===========================
// 3) ALLOCATE BLOCK:
//===========================
void *alloc_block(uint32 size)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert(size <= DYN_ALLOC_MAX_BLOCK_SIZE);
	}
	//==================================================================================
	//==================================================================================
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #3 alloc_block
	//Your code is here
	ROUNDUP_POWERTWO(size,size); // if it's already a power, do nothing. save result back in same variable
	uint32 sizetoIndex = (log2_ceil(size) -3) ;////// TAKE CARE TO NOT GO ARRAY OF O BOUNDARY
	uint32 pagetoIndex;
	uint32 va;
	int done =0;
	do
	{if (!LIST_EMPTY(&freeBlockLists[sizetoIndex]))
	{
		struct BlockElement* last_element = LIST_LAST(&freeBlockLists[sizetoIndex]);
		LIST_REMOVE(&freeBlockLists[sizetoIndex],last_element);
		pageBlockInfoArr[pagetoIndex].num_of_free_blocks--;
		va = dynAllocStart+ pagetoIndex * PAGE_SIZE; //sth idk how to dastart
		//va =to_page_va(last_element) ???OR THISooooooooooooooooooooooo
		done =1;
		break;

	}
	else if (!LIST_EMPTY(&freePagesList))
	{
		uint32 NUM_OF_BLOCKS = PAGE_SIZE/size;

		struct PageInfoElement* last_element = LIST_LAST(&freePagesList);
		// doc says is7abo from page allocator?????
		uint32 new_page_va = to_page_va(last_element);
		uint32 new_page_index = (new_page_va / PAGE_SIZE);// we don't add one bc zero based.
		LIST_REMOVE(&freePagesList,last_element);
		pageBlockInfoArr[new_page_index].block_size= size;
		pageBlockInfoArr[new_page_index].num_of_free_blocks= NUM_OF_BLOCKS;
		uint32 block_address = new_page_va;
		for (int i = 0; i < NUM_OF_BLOCKS; i++)
		{ 
		struct BlockElement * block = block_address;
		LIST_INSERT_TAIL(&freeBlockLists[sizetoIndex],block);	
		block_address+=size;
		}

		continue;
	}
	else 
	{
		if (size == DYN_ALLOC_MAX_BLOCK_SIZE)// reached limit in search? means no free block no free page nothing
		{break;}
		size++; //so it can be rounded up to next 2 power x
		ROUNDUP_POWERTWO(size,size);
		continue; 
	}}
	while (done == 0);
	if (done ==1)
	return va;
	else 
	panic("can not allocate memory! dynalloc memory is full!");

	//Comment the following line
	// panic("alloc_block() Not implemented yet");

	//TODO: [PROJECT'25.BONUS#1] DYNAMIC ALLOCATOR - block if no free block
}

//===========================
// [4] FREE BLOCK:
//===========================
void free_block(void *va)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert((uint32)va >= dynAllocStart && (uint32)va < dynAllocEnd);
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #4 free_block
	//Your code is here
	//Comment the following line
	panic("free_block() Not implemented yet");
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//===========================
// [1] REALLOCATE BLOCK:
//===========================
void *realloc_block(void* va, uint32 new_size)
{
	//TODO: [PROJECT'25.BONUS#2] KERNEL REALLOC - realloc_block
	//Your code is here
	//Comment the following line
	panic("realloc_block() Not implemented yet");
}
