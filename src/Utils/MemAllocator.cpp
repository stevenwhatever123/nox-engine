#include <Utils/MemAllocator.h>
#include <Utils/Utils.h>
#include <cassert>

using NoxEngineUtils::Logger;

using namespace NoxEngine;

u8* NoxEngine::align_ptr(u8* ptr, i32 align_to) {

	if(align_to%2 != 0) {
		LOG_DEBUG("align(%d) to must be a power of 2", align_to);
		exit(-1);
	}


	u64 ptr_number = (u64)ptr;

	i32 remaining = align_ptr_needed_bytes(ptr, align_to);
	if( remaining != 0 ) {
		ptr_number += remaining;
	}


	return (u8*)ptr_number;;
}

i32 NoxEngine::align_ptr_needed_bytes(u8* ptr, i32 align_to) {

	if(align_to%2 != 0) {
		LOG_DEBUG("align(%d) to must be a power of 2", align_to);
		assert(false);
	}

	u64 ptr_number = (u64)ptr;
	i32 remaining = ptr_number%align_to;

	return remaining;
}


// PermanentMemAllocator class
PermanentMemAllocator::PermanentMemAllocator() {
	_size = 0;
	_capacity = NoxEngineUtils::MemUtils::MBytes(INITIAL_PERM_MEM_SIZE_MEG);
	_data = (u8*)calloc(1, _capacity );

	_allocation_capacity = 0;
	_allocation_count = 0;
	_allocations = (PermanentMemAllocator::Allocations*)calloc(PERM_MEM_ALLOC_MAX_COUNT, sizeof(PermanentMemAllocator::Allocations));
}

// size is in bytes
u8* PermanentMemAllocator::allocate(i32 size) {

	u8 *data_to_return = (u8*)_data + _size;
	i32 bytes_to_alignment = align_ptr_needed_bytes(data_to_return, DEFAULT_PTR_ALIGNMENT);

	if((size + _size + bytes_to_alignment) <= _capacity) {
		data_to_return += bytes_to_alignment;
		_size += size + bytes_to_alignment;

		_allocations[_allocation_count].ptr = data_to_return;
		_allocations[_allocation_count].size = size;
		_allocation_count++;

		return data_to_return;
	}

	LOG_DEBUG("Out of Permanent memory");

	return NULL;
}

void PermanentMemAllocator::deallocate(void *ptr) {
	LOG_DEBUG("Permanent Memory allocator doesn't have a deallocator");
	assert(false);
}

PermanentMemAllocator::Allocations* PermanentMemAllocator::getAllocations() const {
	return _allocations;
}


// Scratch Memory Allocator
ScratchMemAllocator::ScratchMemAllocator() {
	_size = NoxEngineUtils::MemUtils::MBytes(INITIAL_SCARTCH_MEM);
	_data = (u8*)calloc(1, _size);
}

// size is in bytes
u8* ScratchMemAllocator::allocate(i32 size) {

	if(size < _size) {
		memset(_data, 0, _size);
		return _data;
	}

	LOG_DEBUG("Size (%d) is bigger than scratch memory size %d", size, _size);
	return NULL;

}

StackMemAllocator::StackMemAllocator() {
	_size = 0;
	_capacity = NoxEngineUtils::MemUtils::KBytes(INITIAL_STACK_MEM);
	_data = (u8*)calloc(1, _capacity);
	memset(_data, 0, _capacity);
}

u8* StackMemAllocator::allocate(i32 size) {
	uptr data_to_return = (uptr)_data + _size;

	i32 needed_bytes = align_ptr_needed_bytes((u8*)data_to_return, DEFAULT_PTR_ALIGNMENT);

	if((data_to_return + needed_bytes) >= ((uptr)_data + _capacity)) {
		LOG_DEBUG("Stack Allocator is full");
		return NULL;
	}

	data_to_return += needed_bytes;

	_size += size + needed_bytes;
	_allocs.push_back(StackAllocation{data_to_return, size, needed_bytes});

	_last_alloc = data_to_return;

	return (u8*)data_to_return;
}

void StackMemAllocator::free(u8 *allocated_mem) {

	uptr data_to_free = (uptr)allocated_mem;

	if(data_to_free != _last_alloc) {
		LOG_DEBUG("Memory being freed is not the latest allocated, no changes.");
		return;
	}

	auto& allocated = _allocs.back();

	_size -= (allocated.size + allocated.padding);

	_allocs.pop_back();

	if(_allocs.size() == 0) {
		_last_alloc = 0;
	} else {
		_last_alloc = _allocs.back().ptr_allocated;
	}
}

