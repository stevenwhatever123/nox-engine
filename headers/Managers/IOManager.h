#pragma once

#include <Core/Types.h>
#include <Managers/Singleton.h>
#include <Utils/MemAllocator.h>


namespace NoxEngine {

	// backed by StackMemAllocator will be immediately when out of scope
	struct TempResourceData {
		u8 *data;
		i32 size;

		~TempResourceData() {
			StackMemAllocator::Instance()->free(data);
		}

	};

	// Backed by PermMemAllocator
	struct PermResourceData {
		u8 *data;
		i32 size;
	};


	class IOManager : public Singleton<IOManager> {
		friend class Singleton<IOManager>;
		public:
			PermResourceData ReadEntireFilePerm(String filename);
			TempResourceData ReadEntireFileTemp(String filename);
			String PickFile(const char* extension_filters = "");

		protected:
			 IOManager(){};
			~IOManager(){};

	};

}
