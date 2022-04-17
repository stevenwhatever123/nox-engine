#pragma once
#define DEFAULT_PTR_ALIGNMENT 16

#define INITIAL_PERM_MEM_SIZE_MEG 2
#define PERM_MEM_ALLOC_MAX_COUNT 1024

#define INITIAL_SCARTCH_MEM 2
#define INITIAL_STACK_MEM 2

#include <vector>

#include <Types.h>
#include <Singleton.h>
#include <Utils.h>


/* TODO(sharo): StackMemAllocator should return an object that behaves like a pointer but can be destroyed
 * after it leaves a scope
 * 
 *
 *
 *
 *
 *
 *
 */


using NoxEngineUtils::Logger;

namespace NoxEngine {
	u8* align_ptr(u8* ptr, i32 align_to);
	i32 align_ptr_needed_bytes(u8* ptr, i32 align_to);
	
	class PermanentMemAllocator : public Singleton<PermanentMemAllocator> {
		friend class Singleton<PermanentMemAllocator>;
		public:
			struct Allocations {
				void *ptr;
				i64 size;
			};

			u8* allocate(i32 size_bytes);
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
	};


	class ScratchMemAllocator : public Singleton<ScratchMemAllocator> {
		friend class Singleton<ScratchMemAllocator>;
		public:
			u8* allocate(i32 size);
		protected:
			ScratchMemAllocator();
			u8* _data;
			i64 _size; // in bytes
		
	};

	class StackMemAllocator : public Singleton<StackMemAllocator> {
		friend class Singleton<StackMemAllocator>;

		struct StackAllocation {
			uptr ptr_allocated;
			i32 size;
			i32 padding;
		};

		public:
			u8* allocate(i32 size);
			void free(u8 *allocate_mem);
		protected:
			StackMemAllocator();
			~StackMemAllocator(){}

		private: 
			u8 *_data;
			i32 _size;
			i32 _capacity;
			uptr _last_alloc;
			std::vector<StackAllocation> _allocs;

	};

}
