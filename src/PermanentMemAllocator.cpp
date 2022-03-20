#include <PermanentMemAllocator.h>
#include <Utils.h>
#include <cstdio>
#include <cassert>

using NoxEngineUtils::Logger;

namespace NoxEngine {

	PermanentMemAllocator::PermanentMemAllocator() {
		_size = 0;
		_capacity = NoxEngineUtils::MemUtils::MBytes(INITIAL_PERM_MEM_SIZE_MEG);
		_data = (u8*)calloc(1, _capacity );

		_allocation_capacity = 0;
		_allocation_count = 0;
		_allocations = (PermanentMemAllocator::Allocations*)calloc(INITIAL_ALLOC_COUNT, sizeof(PermanentMemAllocator::Allocations));
	}

	// size is in bytes
	void* PermanentMemAllocator::allocate(i64 size) {

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

		Logger::debug("Out of Permanent memory");

		return NULL;
	}

	void PermanentMemAllocator::deallocate(void *ptr) {
		Logger::debug("Permanent Memory allocator doesn't have a deallocator");
		assert(false);
	}

	PermanentMemAllocator::Allocations* PermanentMemAllocator::getAllocations() const {
		return _allocations;
	}

	u8* PermanentMemAllocator::align_ptr(u8* ptr, i32 align_to) {

		if(align_to%2 != 0) {
			Logger::debug("align(%d) to must be a power of 2", align_to);
			assert(false);
		}


		u64 ptr_number = (u64)ptr;

		i32 remaining = align_ptr_needed_bytes(ptr, align_to);
		if( remaining != 0 ) {
			ptr_number += remaining;
		}


		return (u8*)ptr_number;;
	}

	i32 PermanentMemAllocator::align_ptr_needed_bytes(u8* ptr, i32 align_to) {

		if(align_to%2 != 0) {
			Logger::debug("align(%d) to must be a power of 2", align_to);
			assert(false);
		}

		u64 ptr_number = (u64)ptr;
		i32 remaining = ptr_number%align_to;

		return remaining;
	}


}
