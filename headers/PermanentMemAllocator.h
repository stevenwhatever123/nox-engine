#pragma once
#define INITIAL_PERM_MEM_SIZE_MEG 1024

#include <MemoryManager.h>

namespace NoxEngine {
	class PermanentMemAllocator : public Singleton<PermanentMemAllocator> {
		friend class Singleton<PermanentMemAllocator>;
		public:
			void *allocate(i64 size_bytes);
			void deallocate(void *ptr);

		protected:
			struct Allocations {
				void *ptr;
				i32 size;
			};

			PermanentMemAllocator();
			std::vector<Allocations> _allocations;
			void* _data;
			i64 _size; // in bytes
			i64 _capacity;

	};
}
