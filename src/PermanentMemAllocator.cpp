#include <PermanentMemAllocator.h>
#include <Utils.h>
#include <cstdio>

namespace NoxEngine {

	PermanentMemAllocator::PermanentMemAllocator() {
		_size = NoxEngineUtils::MemUtils::MBytes(INITIAL_PERM_MEM_SIZE_MEG);
		_data = (void*)calloc(1, _size );
		_capacity = 0;
	}

	void* PermanentMemAllocator::allocate(i64 size) {
		return NULL;
	}

	void PermanentMemAllocator::deallocate(void *ptr) {

	}

}
