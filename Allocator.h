#pragma once

#include"Preprocessor.h"

#include<cassert>
#include<cstdlib>
#include<memory>

#ifdef ESCAPIST_HAS_NAMESPACE
namespace Escapist
{
#endif

	template<typename T>
	class Allocator final
	{
		using SysAllocator = typename std::allocator<T>;

	public:
		static void Allocate(T*& ptr, SizeType capacity) {
			ptr = (T*)::malloc(capacity);
			assert(ptr);
		}
		static T* Allocate(SizeType capacity) {
			T* ptr((T*)::malloc(capacity));
			assert(ptr);
			return ptr;
		}
		static void TypedAllocate(T*& ptr, SizeType capacity) {
			ptr = (T*)::malloc(capacity * sizeof(T));
			assert(ptr);
		}
		static T* TypedAllocate(SizeType capacity) {
			T* ptr((T*)::malloc(capacity * sizeof(T)));
			assert(ptr);
			return ptr;
		}
		static void Reallocate(T*& ptr, SizeType capacity) {
			assert(ptr);
			ptr = (T*)::realloc(ptr, capacity);
			assert(ptr);
		}
		static T*& ReallocateNew(T*& ptr, SizeType capacity) {
			T* newptr((T*)::realloc(ptr, capacity));
			assert(newptr);
			return newptr;
		}
		static void TypedReallocate(T*& ptr, SizeType capacity) {
			ptr = (T*)::realloc(ptr, capacity * sizeof(T));
			assert(ptr);
		}
		static T* TypedReallocateNew(T*& ptr, SizeType capacity) {
			T* newptr((T*)::realloc(ptr, capacity * sizeof(T)));
			assert(ptr);
			return newptr;
		}
		static void Deallocate(T*& ptr) {
			//if (ptr) {
				::free((void*)ptr);
				ptr = nullptr;
			//}
		}
		static void Free(T*& ptr) {
			//if (ptr) {
				::free((void*)ptr);
				ptr = nullptr;
			//}
		}
		static void DefaultConstruct(T* const& ptr) {
			new(ptr)T();
		}
		static void CopyConstruct(T* const& ptr, const T& value) {
			new(ptr)T(value);
		}
		template<typename... Parameters>
		static void ParameterConstruct(T* const& ptr, Parameters&&... params) {
			new(ptr)T(std::forward<Parameters>(params)...);
		}
		static void Destroy(T* const& ptr) {
			ptr->~T();
		}
	};

#ifdef ESCAPIST_HAS_NAMESPACE
}
#endif