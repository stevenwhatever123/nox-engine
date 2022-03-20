#pragma once
#define DEFAULT_PTR_ALIGNMENT 16

#define INITIAL_PERM_MEM_SIZE_MEG 1024
#define PERM_MEM_ALLOC_MAX_COUNT 1024

#define INITIAL_SCARTCH_MEM 2

#include <Types.h>
#include <Singleton.h>

namespace NoxEngine {

	
	class PermanentMemAllocator : public Singleton<PermanentMemAllocator> {
		friend class Singleton<PermanentMemAllocator>;
		public:
			struct Allocations {
				void *ptr;
				i64 size;
			};

			u8* allocate(i64 size_bytes);
			void deallocate(void *ptr);
			Allocations* getAllocations() const;
			inline i64 getAllocationsCount() { return _allocation_count; };

		protected:

			PermanentMemAllocator();

			u8* _data;
			i64 _size; // in bytes
			i64 _capacity;

			Allocations* _allocations;
			i64 _allocation_count; // in allocation byte
			i64 _allocation_capacity;

		private:
			u8* align_ptr(u8* ptr, i32 align_to);
			i32 align_ptr_needed_bytes(u8* ptr, i32 align_to);

	};


	class ScratchMemAllocator : public Singleton<ScratchMemAllocator> {
		friend class Singleton<ScratchMemAllocator>;
		public:
			u8* allocate(i64 size);
		protected:
			ScratchMemAllocator();
			u8* _data;
			i64 _size; // in bytes
		
	};


}
