#pragma once

#include <Core/Types.h>
#include <Managers/Singleton.h>
#include <Utils/MemAllocator.h>
#include <string>


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
			PermResourceData ReadEntireFilePerm(std::string filename);
			TempResourceData ReadEntireFileTemp(std::string filename);
			std::string PickFile(const char* extension_filters = "");

		protected:
			 IOManager(){};
			~IOManager(){};

	};

}
