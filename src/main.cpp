#define GLEW_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <Utils.h>
#include <EventManager.h>
#include <PermanentMemAllocator.h>

using NoxEngineUtils::Logger;
using NoxEngine::EventManager;
using NoxEngine::PermanentMemAllocator;


void init() {
	auto *perm_mem_alloc = PermanentMemAllocator::Instance();
	auto *perm_mem_alloc2 = PermanentMemAllocator::Instance();

	Logger::debug("%p == %p", perm_mem_alloc, perm_mem_alloc2);


	void* mem = perm_mem_alloc->allocate(1024);
	void* mem1 = perm_mem_alloc->allocate(1024);

	auto* allocations = perm_mem_alloc->getAllocations();
	const i64 allocation_count = perm_mem_alloc->getAllocationsCount();

	Logger::debug("allocated: %d", allocation_count);
	for(i32 i = 0; i < allocation_count; i++) {
		Logger::debug("Allocation %d: ptr: %p - size: %lld", i, allocations[i].ptr, allocations[i].size);
	}



}

int main(int argc, char** argv) {
	init();

	return 0;
}
