#pragma once

#include"Preprocessor.h"

#include<cassert>
#include<cstring>
#include"Internal/ReferenceCount.h"
#include"Allocator.h"

namespace EscapistPrivate
{
	template<typename Char>
	class CharTrait {
		using SizeType = typename Escapist::SizeType;
	public:
		static void Copy(Char* dest, const Char* src, SizeType size) {
			::memcpy((void*)dest, (const void*)src, size * sizeof(Char));
		}
		static void Copy(Char* dest, SizeType capacity, const Char* src, SizeType size) {
			::memcpy_s((void*)dest, capacity * sizeof(Char), (const void*)src, size * sizeof(Char));
		}
		static void Move(Char* dest, const Char* src, SizeType size) {
			::memmove((void*)dest, (const void*)src, size * sizeof(Char));
		}
		static void SafeMove(Char* dest, SizeType capacity, const Char* src, SizeType size) {
			::memmove_s((void*)dest, capacity * sizeof(Char), (const void*)src, size * sizeof(Char));
		}
		static void Assign(Char* const dest, const Char& ch) {
			*dest = ch;
		}
		static void Fill(Char* dest, const Char& ch, SizeType count) {
			for (; count > 0; ++dest, --count) {
				Assign(dest, ch);
			}
		}
		static SizeType GetLength(const Char* src) {
			SizeType len(0);
			while (*src) {
				++len;
				++src;
			}
			return len;
		}
		static int Compare(const Char* left, const Char* right) {
			if (left && right) {
				if (left == right) {
					return 0;
				}
				while (*left && *right && *left == *right) {
					++left;
					++right;
				}
				return left - right;
			}
			return 0;
		}
		static int CompareN(const Char* left, const Char* right, SizeType size) {
			if (left && right) {
				if (!size || left == right) {
					return 0;
				}
				while (size && *left && *right && *left == *right) {
					++left;
					++right;
					--size;
				}
				return left - right;
			}
			return 0;
		}
		static int CompareNoCase(const Char* left, const Char* right) {

		}
		static SizeType IndexOf(const Char* data, const Char& ch) {

		}
		static SizeType IndexOf(const Char* data, const Char* target) {

		}
		static SizeType LastIndexOf(const Char* data, const Char& ch) {

		}
		static SizeType LastIndexOf(const Char* data, const Char* target) {

		}
		static const Char* FirstNotOf(const Char* data, const Char target) {
			if (data && target) {
				const Char* dataPos = data;
				while (*dataPos) {
					if (*dataPos != target) {
						return dataPos;
					}
					++dataPos;
				}
			}
			return nullptr;
		}
		static const Char* LastNotOf(const Char* data, const Char target) {
			if (data && target) {
				const Char* dataPos = data;
				const Char* lastPos = nullptr;
				while (*dataPos) {
					if (*dataPos != target) {
						lastPos = dataPos;
					}
					++dataPos;
				}
				return lastPos;
			}
			return nullptr;
		}
	};

	template<>
	class CharTrait<char> {
		using SizeType = typename Escapist::SizeType;
	public:
		static void Copy(char* dest, const char* src, SizeType size) {
			::memcpy((void*)dest, (const void*)src, size);
		}
		static void Copy(char* dest, SizeType capacity, const char* src, SizeType size) {
			::memcpy_s((void*)dest, capacity * sizeof(char), (const void*)src, size);
		}
		static void Move(char* dest, const char* src, SizeType size) {
			::memmove((void*)dest, (const void*)src, size);
		}
		static void SafeMove(char* dest, SizeType capacity, const char* src, SizeType size) {
			::memmove_s((void*)dest, capacity, (const void*)src, size);
		}
		static void Assign(char* const dest, const char& ch) {
			*dest = ch;
		}
		static void Fill(char* dest, const char& ch, SizeType count) {
			for (; count > 0; ++dest, --count) {
				Assign(dest, ch);
			}
		}
		static int Compare(const char* left, const char* right) {
			return ::strcmp(left, right);
		}
		static int CompareN(const char* left, const char* right, SizeType size) {
			return ::strncmp(left, right, size);
		}
		static int CompareNoCase(const char* left, const char* right) {
			return ::_stricmp(left, right);
		}
		static SizeType GetLength(const char* src) {
			return ::strlen(src);
		}
		static const char* IndexOf(const char* data, const char& ch) {
			return ::strchr(data, ch);
		}
		static const char* IndexOf(const char* data, const char* target) {
			return ::strstr(data, target);
		}
		static const char* LastIndexOf(const char* data, const char& ch) {
			return ::strrchr(data, ch);
		}
		static const char* LastIndexOf(const char* data, const char* target) {
			if (data && target) {
				const char* dataPos = data;
				const char* targetPos = target;
				const char* currentPos = nullptr;
				const char* lastPos = nullptr;
				while (*dataPos) {
					currentPos = dataPos;
					while (*dataPos && *targetPos && *dataPos == *targetPos) {
						++dataPos;
						++targetPos;
					}
					if (!(*targetPos)) {
						lastPos = currentPos;
					}
					targetPos = target;
					++dataPos;
				}
				return lastPos;
			}
			return nullptr;
		}
		static const char* FirstNotOf(const char* data, const char target) {
			if (data && target) {
				const char* dataPos = data;
				while (*dataPos) {
					if (*dataPos != target) {
						return dataPos;
					}
					++dataPos;
				}
			}
			return nullptr;
		}
		static const char* LastNotOf(const char* data, const char target) {
			if (data && target) {
				const char* dataPos = data;
				const char* lastPos = nullptr;
				while (*dataPos) {
					if (*dataPos != target) {
						lastPos = dataPos;
					}
					++dataPos;
				}
				return lastPos;
			}
			return nullptr;
		}

	};

	template<>
	class CharTrait<wchar_t> {
		using SizeType = typename Escapist::SizeType;
	public:
		static void Copy(wchar_t* dest, const wchar_t* src, SizeType size) {
			::memcpy((void*)dest, (const void*)src, size * sizeof(wchar_t));
		}
		static void Copy(wchar_t* dest, SizeType capacity, const wchar_t* src, SizeType size) {
			::memcpy_s((void*)dest, capacity * sizeof(wchar_t), (const void*)src, size * sizeof(wchar_t));
		}
		static void Move(wchar_t* dest, const wchar_t* src, SizeType size) {
			::memmove((void*)dest, (const void*)src, size * sizeof(wchar_t));
		}
		static void SafeMove(wchar_t* dest, SizeType capacity, const wchar_t* src, SizeType size) {
			::memmove_s((void*)dest, capacity * sizeof(wchar_t), (const void*)src, size * sizeof(wchar_t));
		}
		static void Assign(wchar_t* const dest, const wchar_t& ch) {
			*dest = ch;
		}
		static void Fill(wchar_t* dest, const wchar_t& ch, SizeType count) {
			for (; count > 0; ++dest, --count) {
				Assign(dest, ch);
			}
		}
		static int Compare(const wchar_t* left, const wchar_t* right) {
			return ::wcscmp(left, right);
		}
		static int CompareN(const wchar_t* left, const wchar_t* right, SizeType size) {
			return ::wcsncmp(left, right, size);
		}
		static int CompareNoCase(const wchar_t* left, const wchar_t* right) {
			return ::_wcsicmp(left, right);
		}
		static SizeType GetLength(const wchar_t* src) {
			return ::wcslen(src);
		}
		static const wchar_t* IndexOf(const wchar_t* data, const wchar_t& ch) {
			return ::wcschr(data, ch);
		}
		static const wchar_t* IndexOf(const wchar_t* data, const wchar_t* target) {
			return ::wcsstr(data, target);
		}
		static const wchar_t* LastIndexOf(const wchar_t* data, const wchar_t& ch) {
			return ::wcsrchr(data, ch);
		}
		static const wchar_t* LastIndexOf(const wchar_t* data, const wchar_t* target) {
			if (data && target) {
				const wchar_t* dataPos = data;
				const wchar_t* targetPos = target;
				const wchar_t* currentPos = nullptr;
				const wchar_t* lastPos = nullptr;
				while (*dataPos) {
					currentPos = dataPos;
					while (*dataPos && *targetPos && *dataPos == *targetPos) {
						++dataPos;
						++targetPos;
					}
					if (!(*targetPos)) {
						lastPos = currentPos;
					}
					targetPos = target;
					++dataPos;
				}
				return lastPos;
			}
			return nullptr;
		}

		static const wchar_t* FirstNotOf(const wchar_t* data, const wchar_t target) {
			if (data && target) {
				const wchar_t* dataPos = data;
				while (*dataPos) {
					if (*dataPos != target) {
						return dataPos;
					}
					++dataPos;
				}
			}
			return nullptr;
		}
		static const wchar_t* LastNotOf(const wchar_t* data, const wchar_t target) {
			if (data && target) {
				const wchar_t* dataPos = data;
				const wchar_t* lastPos = nullptr;
				while (*dataPos) {
					if (*dataPos != target) {
						lastPos = dataPos;
					}
					++dataPos;
				}
				return lastPos;
			}
			return nullptr;
		}
	};
}

#ifdef ESCAPIST_HAS_NAMESPACE
namespace Escapist {
#endif

	template<typename Char>
	class BasicString {
	private:
		static_assert(std::is_integral_v<Char>, "Characters must be Integer types");
		using ReferenceCount = typename EscapistPrivate::ReferenceCount;
		using CharTrait = typename EscapistPrivate::CharTrait<Char>;
		using Self = typename Escapist::BasicString<Char>;

		// To prevent frequent copy and reallocation, MinimumCapacity is to set the minimum capacity.
		static constexpr SizeType MinimumCapacity = (4 * sizeof(Char*)) / sizeof(Char);

		// If the input size + 1 (for null-termination) is larger than MC, the capacity is (size * 1.5)
		// Otherwise, it's MC.
		static inline constexpr SizeType Capacity(SizeType initialSize) {
			if (initialSize) { // However, if the input is zero, we don't need to allocate them.
				//++initialSize;
				if (initialSize + 1 > Self::MinimumCapacity) {
					return initialSize * 1.5;
				}
				else {
					return Self::MinimumCapacity;
				}
			}
			else {
				return 0;
			}
			// Does the constexpr effective in this function?
		}
		static inline constexpr SizeType MemorySize(const SizeType initialCapacity) {
			return sizeof(ReferenceCount*) + initialCapacity * sizeof(Char);
		}

		ReferenceCount** ref;
		Char* data;
		SizeType size;
		SizeType capacity;

		// Three steps:
		// 1. Allocate the new memory
		// 2. Assign the reference count pointer (mostly nullptr, except)
		// 3. Point data to (Char*)(ref + 1)
		void Allocate(ReferenceCount* const& initialRef = nullptr) {
			Allocator<ReferenceCount*>::Allocate(ref, Self::MemorySize(capacity));
			(*ref) = initialRef;
			data = (Char*)(ref + 1);
		}
		void Construct(const SizeType initialSize, ReferenceCount* const& initialRef = nullptr) {
			Self::Construct(initialSize, Self::Capacity(initialSize), initialRef);
		}
		void Construct(const SizeType initialSize, const SizeType initialCapacity, ReferenceCount* const& initialRef = nullptr) {
			size = initialSize;
			capacity = initialCapacity;
			Self::Allocate(initialRef);
			data[size] = Char(0);
		}
		void InitializeRef(const int&& initialValue) {
			Allocator<ReferenceCount>::TypedAllocate(*ref, 1);
			Allocator<ReferenceCount>::ParameterConstruct<const int&>(*ref, initialValue);
		}

		Char* GrowthAppend(const SizeType growthSize) {
			if (growthSize) {
				if (ref && data && size) {
					SizeType oldSize = size; // Store the current size for future use.
					if (*ref && (**ref).IsShared()) {
						// This means this objects share data with other object(s).
						// We need to detach from the shared data by allocating a new memory and copy.
						Char* const oldData = data;
						(**ref).DecrementRef(); // Detach from shared object at first. 
						Self::Construct(size + growthSize, nullptr); // Then create a new object like constructor.
						CharTrait::Copy(data, oldData, oldSize); // Copy from old object.
						// Now, this object doesn't relate to that shared object.
					}
					else {
						// Whatever the reference count is initialized, it's not shared with other object.
						// Therefore, we can directly manipulate the object-owned memory.
						size += growthSize; // We stored the oldSize.
						if (size >= capacity) {
							// Different to list, string needs a '\0' to show its termination.
							// We have to reserve for that zero, so '>='
							// *****************************************
							// This means the current memory cannot accommodate new data.
							// We need to create a new one and copy from the old one.
							capacity = Self::Capacity(size);
							ReferenceCount** oldRef = ref;
							Allocator<ReferenceCount*>::Reallocate(ref, Self::MemorySize(capacity));
							data[size] = Char(0);
						}
						else {
							// The reserved memory is large enough.
							// We just need to set 0.
							data[size] = Char(0);
						}
					}
					return data + oldSize;
				}
				else {
					// This means this object is null. Then we need to initialize that.
					Self::Construct(growthSize, nullptr);
					return data;
				}
			}
			else {
				// Invalid Parameter! No modification.
				return data + size;
			}
		}
		void GrowthPrepend(const SizeType growthSize) {
			if (growthSize) {
				if (ref && data && size) {
					SizeType oldSize = size;
					if (*ref && (**ref).IsShared()) {
						Char* const oldData = data;
						(**ref).DecrementRef();
						Self::Construct(size + growthSize, nullptr);
						CharTrait::Copy(data + growthSize, oldData, oldSize);
					}
					else {
						size += growthSize;
						if (size >= capacity) {
							capacity = Self::Capacity(size);
							ReferenceCount* const oldRef = *ref;
							Char* const oldData = data;
							Self::Construct(size, oldRef);
							CharTrait::Copy(data + growthSize, oldData, oldSize);
						}
						else {
							CharTrait::Move(data + growthSize, data, oldSize);
							data[size] = Char(0);
						}
					}
				}
				else {
					Self::Construct(growthSize, nullptr);
				}
			}
		}
		bool GrowthInsert(const SizeType growthIndex, const SizeType growthSize) {
			if (growthSize) {
				if (growthIndex >= size) {
					Self::GrowthAppend(growthSize);
				}
				else if (growthIndex == 0) {
					Self::GrowthPrepend(growthSize);
				}
				else {
					if (ref && data && size) {
						SizeType oldSize = size;
						if ((*ref) && (**ref).IsShared()) {
							Char* const oldData = data;
							(**ref).DecrementRef();
							Self::Construct(size + growthSize, nullptr);
							CharTrait::Copy(data, oldData, growthIndex);
							CharTrait::Copy(data + growthIndex + growthSize,
								oldData + growthIndex,
								oldSize - growthIndex);
						}
						else {
							size += growthSize;
							if (size >= capacity) {
								ReferenceCount* const oldRef = *ref;
								Char* const oldData = data;
								Self::Construct(size + growthSize, oldRef);
								CharTrait::Copy(data, oldData, growthIndex);
								CharTrait::Copy(data + growthIndex + growthSize,
									oldData + growthIndex,
									oldSize - growthIndex);
							}
							else {
								CharTrait::Move(data + growthIndex + growthSize,
									data + growthIndex,
									oldSize - growthIndex);
							}
						}
						return true;
					}
				}
			}
			return false;
		}

	public:
		BasicString() :ref(nullptr), data(nullptr), size(0), capacity(0) {

		}
		BasicString(SizeType initialCapacity) {
			if (initialCapacity) {
				Self::Construct(0, initialCapacity, nullptr);
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Char& ch) {
			Self::Construct(1, nullptr);
			CharTrait::Assign(data, ch);
		}
		BasicString(const Char& ch, SizeType count) {
			if (ch && count) {
				Self::Construct(count, nullptr);
				CharTrait::Fill(data, ch, count);
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Char* initialData) {
			if (initialData) {
				Self::Construct(CharTrait::GetLength(initialData), nullptr);
				CharTrait::Copy(data, initialData, size);
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Char* initialData, SizeType dataSize) {
			if (initialData && dataSize) {
				Self::Construct(dataSize, nullptr);
				CharTrait::Copy(data, initialData, size);
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Char* initialData, SizeType dataSize, SizeType dataOffset) {
			if (initialData && dataSize) {
				Self::Construct(dataSize + dataSize, nullptr);
				CharTrait::Copy(data + dataOffset, initialData, dataSize);
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Self& other) {
			if (other.ref && other.data && other.size) {
				::memcpy((void*)this, (const void*)&other, sizeof(Self));
				if (*ref) {
					(**ref).IncrementRef();
				}
				else {
					Self::InitializeRef(2);
				}
			}
			else {
				new(this)Self();
			}
		}
		BasicString(const Self& other, SizeType dataSize) {
			if (other.ref && other.data) {
				if (dataSize && other.size >= dataSize) {
					new(this)Self(other);
				}
				else {
					new(this)Self(other.data, dataSize);
				}
			}
			else {
				new(this)Self();
			}
		}

		~BasicString() {
			if (ref && data) {
				if (*ref && (**ref).IsShared()) {
					(**ref).DecrementRef();
					return;
				}
				else {
					Allocator<ReferenceCount>::Free(*ref);
				}
				Allocator<ReferenceCount*>::Free(ref);
			}
		}

		Char* GetData() {
			if (!IsEmptyOrNull()) {
				if (*ref && (**ref).IsShared()) {
					Char* const oldData = data;
					(**ref).DecrementRef();
					Self::Construct(size, nullptr);
					CharTrait::Copy(data, oldData, size);
				}
				return data;
			}
			return nullptr;
		}
		const Char* GetConstData() {
			return ref && data ? data : nullptr;
		}
		Char GetCharAt(const SizeType index)const {
			return ref && data && index < size ? data[size] : Char(0);
		}
		Self& SetCharAt(const SizeType index, const Char ch) {
			if (index < size && ch && !IsEmptyOrNull()) {
				if (*ref && (**ref).IsShared()) {
					Char* const oldData = data;
					(**ref).DecrementRef();
					Self::Construct(size, nullptr);
					CharTrait::Copy(data, oldData, size);
				}
				data[index] = ch;
			}
			return *this;
		}
		SizeType GetLength()const {
			return ref && size ? size : 0;
		}
		SizeType RefreshSize() {
			size = CharTrait::GetLength(data);
		}
		SizeType RefreshSize(const SizeType newSize) {
			size = newSize;
		}
		SizeType GetCapacity()const {
			return ref && size ? (*ref && (**ref).IsShared()) ? 0 : capacity : 0;
		}

		bool IsShared()const {
			return (ref) && (*ref) && ((**ref).IsShared());
		}
		bool IsSharingWith(const Self& other)const {
			return Self::IsShared() && ref == other.ref;
		}
		bool IsEmpty()const {
			return !(ref && data && size);
		}
		bool IsNull()const {
			return !(ref && data && capacity);
		}
		bool IsEmptyOrNull()const {
			return !(ref && data && size && capacity);
		}

		int CompareTo(const Char* other)const {
			return CharTrait::Compare(data, other);
		}
		int CompareTo(const Self& other)const {
			return CharTrait::Compare(data, other.data);
		}
		int CompareTo(const Char* other, const SizeType length)const {
			return CharTrait::CompareN(data, other, length);
		}
		int CompareTo(const Self& other, const SizeType length)const {
			return CharTrait::CompareN(data, other.data, length);
		}
		int CompareToNoCase(const Char* other)const {
			return CharTrait::CompareNoCase(data, other);
		}
		int CompareToNoCase(const Self& other)const {
			return CharTrait::CompareNoCase(data, other.data);
		}
		bool Equals(const Char* other)const {
			return Self::CompareTo(other) == 0;
		}
		bool Equals(const Self& other)const {
			return Self::CompareTo(other) == 0;
		}

		SizeType IndexOf(const Char target)const {
			if (target) {
				const Char* result = CharTrait::IndexOf(data, target);
				return result ? result - data : -1;
			}
			else {
				return size;
			}
		}
		SizeType IndexOf(const Char* target)const {
			if (target) {
				const Char* result = CharTrait::IndexOf(data, target);
				return result ? result - data : -1;
			}
			else {
				return size;
			}
		}
		SizeType LastIndexOf(const Char target)const {
			if (target) {
				const Char* result = CharTrait::LastIndexOf(data, target);
				return result ? result - data : -1;
			}
			else {
				return size;
			}
		}
		SizeType LastIndexOf(const Char* target)const {
			if (target) {
				const Char* result = CharTrait::LastIndexOf(data, target);
				return result ? result - data : -1;
			}
			else {
				return size;
			}
		}
		SizeType FirstNotOf(const Char target)const {
			if (target) {
				const Char* result = CharTrait::FirstNotOf(data, target);
				return result != data ? result - data : -1;
			}
			else {
				return size;
			}
		}
		SizeType LastNotOf(const Char target)const {
			if (target) {
				const Char* result = CharTrait::LastNotOf(data, target);
				return result != data ? result - data : -1;
			}
			else {
				return size;
			}
		}

		Self& Append(const Char& ch) {
			if (ch) { // Do I need to check whether the char is 0?
				CharTrait::Assign(Self::GrowthAppend(1), ch);
			}
			return *this;
		}
		Self& Append(const Char& ch, const SizeType count) {
			if (ch && count) {
				CharTrait::Fill(Self::GrowthAppend(count), ch, count);
			}
			return *this;
		}
		Self& Append(const Char* initialData) {
			if (initialData) {
				SizeType length = CharTrait::GetLength(initialData);
				CharTrait::Copy(Self::GrowthAppend(length), initialData, length);
			}
			return *this;
		}
		Self& Append(const Char* initialData, const SizeType dataSize) {
			if (initialData && dataSize) {
				CharTrait::Copy(Self::GrowthAppend(dataSize), initialData, dataSize);
			}
			return *this;
		}
		Self& Append(const Self& other) {
			if (!other.IsEmptyOrNull()) {
				CharTrait::Copy(Self::GrowthAppend(other.size), other.data, other.size);
			}
			return *this;
		}
		Self& Append(const Self& other, const SizeType dataSize) {
			if (dataSize && !other.IsEmptyOrNull()) {
				if (size >= other.size) {
					return Self::Append(other);
				}
				else {
					return Self::Append(other.data, dataSize);
				}
			}
			return *this;
		}

		Self& Prepend(const Char& ch) {
			if (ch) {
				Self::GrowthPrepend(1);
				CharTrait::Assign(data, ch);
			}
			return *this;
		}
		Self& Prepend(const Char& ch, const SizeType count) {
			if (ch && count) {
				Self::GrowthPrepend(count);
				CharTrait::Assign(data, ch);
			}
			return *this;
		}
		Self& Prepend(const Char* initialData) {
			if (initialData) {
				SizeType length = CharTrait::GetLength(initialData);
				Self::GrowthPrepend(length);
				CharTrait::Copy(data, initialData, length);
			}
			return *this;
		}
		Self& Prepend(const Char* initialData, const SizeType dataSize) {
			if (initialData && dataSize) {
				Self::GrowthPrepend(dataSize);
				CharTrait::Copy(data, initialData, dataSize);
			}
			return *this;
		}
		Self& Prepend(const Self& other) {
			if (other.IsEmptyOrNull()) {
				Self::GrowthPrepend(other.size);
				CharTrait::Copy(data, other.data, other.size);
			}
			return *this;
		}
		Self& Prepend(const Self& other, const SizeType dataSize) {
			if (other.IsEmptyOrNull()) {
				if (dataSize >= other.size) {
					return Self::Prepend(other);
				}
				else {
					return Self::Prepend(other.data, dataSize);
				}
			}
			return *this;
		}

		Self& Insert(const SizeType index, const Char& ch) {
			if (ch && Self::GrowthInsert(index, 1)) {
				CharTrait::Assign(data + index, ch);
			}
			return *this;
		}
		Self& Insert(const SizeType index, const Char& ch, const SizeType count) {
			if (ch && count && Self::GrowthInsert(index, count)) {
				CharTrait::Fill(data + index, ch, count);
			}
			return *this;
		}
		Self& Insert(const SizeType index, const Char* initialData) {
			if (initialData) {
				SizeType length = CharTrait::GetLength(initialData);
				if (Self::GrowthInsert(index, length)) {
					CharTrait::Copy(data + index, initialData, length);
				}
			}
			return *this;
		}
		Self& Insert(const SizeType index, const Char* initialData, const SizeType dataSize) {
			if (initialData && dataSize && Self::GrowthInsert(index, dataSize)) {
				CharTrait::Copy(data + index, initialData, dataSize);
			}
			return *this;
		}
		Self& Insert(const SizeType index, const Self& other) {
			if (!other.IsEmpty() && Self::GrowthInsert(index, other.size)) {
				CharTrait::Copy(data + index, other.data, other.size);
			}
			return *this;
		}
		Self& Insert(const SizeType index, const Self& other, const SizeType dataSize) {
			if (dataSize >= other.size) {
				return Self::Insert(index, other);
			}
			if (!other.IsEmpty() && Self::GrowthInsert(index, dataSize)) {
				CharTrait::Copy(data + index, other.data, dataSize);
			}
			return *this;
		}

		Self& Delete(const SizeType index, const SizeType count) {
			if (ref && data && size && index < size && count) {
				SizeType oldSize = size;
				if (*ref && (**ref).IsShared()) {
					Char* const oldData = data;
					(**ref).DecrementRef();
					Self::Construct(size - count);
					CharTrait::Copy(data, oldData, index);
					CharTrait::Copy(data + index, oldData + index + count, oldSize - index - count);
				}
				else {
					size -= count;
					CharTrait::Move(data + index, data + index + count, oldSize - index - count);
					data[size] = Char(0);
				}
			}
			return *this;
		}
		Self& Replace(const Char& oldCh, const Char& newCh) {
			if (oldCh != newCh && !IsEmptyOrNull()) {
				if (*ref && (**ref).IsShared()) {
					Char* const oldData = data;
					(**ref).DecrementRef();
					Self::Construct(size);
					for (SizeType pos = 0; pos < size; ++pos) {
						data[pos] = (oldData[pos] == oldCh) ? newCh : oldData[pos];
					}
				}
				else {
					for (SizeType pos = 0; pos < size; ++pos) {
						if (data[pos] == oldCh) {
							data[pos] = newCh;
						}
					}
				}
			}
			return *this;
		}
		Self& Replace(const Char* oldStr, const Char* newStr) {
			/*if (oldStr && newStr && !IsEmptyOrNull()) {
				SizeType oldStrLen = CharTrait::GetLength(oldStr);
				SizeType newStrLen = CharTrait::GetLength(newStr);
				const Char* pos = data;
				SizeType count = 0;
				while (pos = CharTrait::IndexOf(pos, oldStr)) {
					pos += oldStrLen;
					++count;
				}
				pos = data;

				if (*ref && (**ref).IsShared()) {
					Char* const oldData = data;
					(**ref).DecrementRef();
					Self::Construct(size + (newStrLen - oldStrLen) * count);
					while (pos = CharTrait::IndexOf(pos, oldStr)) {

					}
				}
			}*/
		}

		bool IsStartWith(const Char* prefix)const {
			if (prefix) {
				SizeType prefixLength = CharTrait::GetLength(prefix);
				if (size < prefixLength) {
					return false;
				}
				return CharTrait::CompareN(data, prefix, prefixLength) == 0;
			}
			return false;
		}
		bool IsEndWith(const Char* suffix)const {
			if (suffix) {
				SizeType suffixLength = CharTrait::GetLength(suffix);
				if (size < suffixLength) {
					return false;
				}
				return CharTrait::CompareN(data + size - suffixLength, suffix, suffixLength) == 0;
			}
		}

		Self Clone()const {
			return Self(data, size);
		}
		Self ToUpperCase()const {
			if (!IsEmptyOrNull()) {
				Self result;
				result.Construct(size, nullptr);
				for (SizeType pos = 0; pos < size; ++pos) {
					if (data[pos] >= 'a' && data[pos] <= 'z') {
						result.data[pos] = data[pos] - 32;
					}
					else {
						result.data[pos] = data[pos];
					}
				}
				return result;
			}
			return Self();
		}
		Self ToLowerCase()const {
			if (!IsEmptyOrNull()) {
				Self result;
				result.Construct(size, nullptr);
				for (SizeType pos = 0; pos < size; ++pos) {
					if (data[pos] >= 'A' && data[pos] <= 'Z') {
						result.data[pos] = data[pos] + 32;
					}
					else {
						result.data[pos] = data[pos];
					}
				}
				return result;
			}
			return Self();
		}
		Self GetLeft(const SizeType count)const {
			if (count >= size) {
				return Self(*this);
			}
			else {
				return Self(data, count);
			}
		}
		Self GetLeft(const Char* left) {
			if (left) {
				SizeType index = Self::IndexOf(left);
				return (index != -1) ? Self(data, index) : Self();
			}
			else {
				return Self();
			}
		}
		Self GetRight(const SizeType count)const {
			if (count >= size) {
				return Self(*this);
			}
			else {
				return Self(data + size - count, count);
			}
		}
		Self GetRight(const Char* right) {
			if (right) {
				SizeType index = Self::LastIndexOf(right);
				SizeType length = CharTrait::GetLength(right);
				return (index != -1) ? Self(data + size - index + length, index - length) : Self();
			}
			else {
				return Self();
			}
		}
		Self GetMiddle(const SizeType index, const SizeType count)const {
			if (index >= size) {
				return Self();
			}
			else if (count >= size) {
				return Self(*this);
			}
			else {
				return Self(data + index, count);
			}
		}
		Self Trim()const {
			if (!IsEmptyOrNull()) {
				SizeType index = Self::FirstNotOf(Char(' '));
				SizeType lastIndex = Self::LastNotOf(Char(' '));
				if (index != SizeType(-1) && lastIndex != SizeType(-1)) {
					if (lastIndex != index) {
						return Self(data + index, lastIndex - index);
					}
					else {
						return Self(data + index, size - index);
					}
				}
				else {
					return *this;
				}
			}
		}
	};

	using String = typename Escapist::BasicString<Escapist::Char>;

#ifdef ESCAPIST_HAS_NAMESPACE
}
#endif