#pragma once

#include<cassert>
#include<memory>
#include"Internal/ReferenceCount.h"
#include"Allocator.h"
#include"TypeTrait.h"

template<typename Type>
class List {
	using ReferenceCount = typename EscapistPrivate::ReferenceCount;
	using TypeTrait = typename EscapistPrivate::TypeTraitPatternSelector<Type>::TypeTrait;
	using Self = typename List<Type>;

	static constexpr SizeType MinimumCapacity = (4 * sizeof(Type*)) / sizeof(Type);
	static constexpr bool EnableMinimumCapacity = MinimumCapacity;
	static inline constexpr SizeType Capacity(SizeType initialSize) {
		if (initialSize) {
			if (EnableMinimumCapacity && initialSize <= MinimumCapacity) {
				return MinimumCapacity;
			}
			else {
				return initialSize * 1.5;
			}
		}
		else {
			return 0;
		}
	}
	static inline constexpr SizeType MemorySize(const SizeType initialCapacity) {
		return sizeof(ReferenceCount*) + initialCapacity * sizeof(Char);
	}

	ReferenceCount** ref;
	Type* data;
	SizeType size;
	SizeType capacity;

	void Allocate(ReferenceCount* const& initialRef = nullptr) {
		Allocator<ReferenceCount*>::Allocate(ref, MemorySize(capacity));
		(*ref) = initialRef;
		data = (Type*)(ref + 1);
	}
	void Construct(const SizeType initialSize, ReferenceCount* const& initialRef = nullptr) {
		Construct(initialSize, Capacity(initialSize), initialRef);
	}
	void Construct(const SizeType initialSize, const SizeType initialCapacity, ReferenceCount* const& initialRef = nullptr) {
		size = initialSize;
		capacity = initialCapacity;
		Allocate(initialRef);
	}
	void InitializeRef(const int&& initialValue) {
		Allocator<ReferenceCount>::TypedAllocate(*ref, 1);
		Allocator<ReferenceCount>::ParameterConstruct<const int&>(*ref, initialValue);
	}

	Type* GrowthAppend(const SizeType growthSize) {
		if (growthSize) {
			if (ref && data && size) {
				SizeType oldSize = size;
				if (*ref && (**ref).IsShared()) {
					Type* const oldData = data;
					(**ref).DecrementRef();
					Construct(size + growthSize, nullptr);
					TypeTrait::Copy(data, oldData, oldSize);
				}
				else {
					size += growthSize;
					if (size > capacity) {
						capacity = Capacity(size);
						ReferenceCount** oldRef = ref;
						Allocator<ReferenceCount*>::Reallocate(ref, MemorySize(capacity));
					}
				}
				return data + oldSize;
			}
			else {
				Construct(growthSize, nullptr);
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
					Type* const oldData = data;
					(**ref).DecrementRef();
					Construct(size + growthSize, nullptr);
					TypeTrait::Copy(data + growthSize, oldData, oldSize);
				}
				else {
					size += growthSize;
					if (size >= capacity) {
						capacity = Capacity(size);
						ReferenceCount* const oldRef = *ref;
						Type* const oldData = data;
						Construct(size, oldRef);
						EscapistPrivate::PodTypeTrait<Type>::Copy(data + oldSize, oldData, oldSize);
					}
					else {
						TypeTrait::Move(data + growthSize, data, oldSize);
					}
				}
			}
			else {
				Construct(growthSize, nullptr);
			}
		}
	}
	bool GrowthInsert(const SizeType growthIndex, const SizeType growthSize) {
		if (growthSize) {
			if (growthIndex >= size) {
				GrowthAppend(growthSize);
			}
			else if (growthIndex == 0) {
				GrowthPrepend(growthSize);
			}
			else {
				if (ref && data && size) {
					SizeType oldSize = size;
					if ((*ref) && (**ref).IsShared()) {
						Type* const oldData = data;
						(**ref).DecrementRef();
						Construct(size + growthSize, nullptr);
						TypeTrait::Copy(data, oldData, growthIndex);
						TypeTrait::Copy(data + growthIndex + growthSize,
							oldData + growthIndex,
							oldSize - growthIndex);
					}
					else {
						size += growthSize;
						if (size >= capacity) {
							ReferenceCount* const oldRef = *ref;
							Type* const oldData = data;
							Construct(size + growthSize, oldRef);
							EscapistPrivate::PodTypeTrait<Type>::Copy(data, oldData, growthIndex);
							EscapistPrivate::PodTypeTrait<Type>::Copy(data + growthIndex + growthSize,
								oldData + growthIndex,
								oldSize - growthIndex);
						}
						else {
							TypeTrait::Move(data + growthIndex + growthSize,
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
	List() :ref(nullptr), data(nullptr), size(0), capacity(0) {

	}
	List(SizeType initialCapacity) {
		if (initialCapacity) {
			Construct(0, initialCapacity, nullptr);
		}
		else {
			new(this)Self();
		}
	}
	List(const Type& val, SizeType count) {
		if (count) {
			Construct(count, nullptr);
			TypeTrait::Fill(data, val, count);
		}
		else {
			new(this)Self();
		}
	}
	List(const Type* initialData, SizeType dataSize) {
		if (initialData && dataSize) {
			Construct(dataSize, nullptr);
			TypeTrait::Copy(data, initialData, dataSize);
		}
		else {
			new(this)Self();
		}
	}
	List(const Self& other) {
		if (other.ref && other.data && other.size) {
			::memcpy((void*)this, (const void*)&other, sizeof(Self));
			if (*ref) {
				(**ref).IncrementRef();
			}
			else {
				InitializeRef(2);
			}
		}
		else {
			new(this)Self();
		}
	}
	List(const Self& other, SizeType dataSize) {
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
	List(const Self& other, SizeType dataSize, SizeType dataOffset) {
		// ???
	}
	~List() {
		if (ref && data) {
			if (*ref) {
				if ((**ref).IsShared()) {
					(**ref).DecrementRef();
					return;
				}
				else {
					Allocator<ReferenceCount>::Free(*ref);
				}
			}
			TypeTrait::Destroy(data, size);
			Allocator<ReferenceCount*>::Free(ref);
		}
	}

	Type* GetData() {
		if (!IsEmptyOrNull()) {
			if ((*ref) && (**ref).IsShared()) {
				Type* const oldData = data;
				(**ref).DecrementRef();
				Construct(size);
				TypeTrait::Copy(data, oldData, size);
			}
			return data;
		}
		else {
			return nullptr;
		}
	}
	const Type* GetConstData()const {
		return ref && data ? data : nullptr;
	}
	Type& GetAt(SizeType index) {
		if (!IsEmptyOrNull()) {
			if ((*ref) && (**ref).IsShared()) {
				Type* const oldData = data;
				(**ref).DecrementRef();
				Construct(size);
				TypeTrait::Copy(data, oldData, size);
			}
			return data[size];
		}
	}
	const Type& GetConstAt(SizeType index)const {
		return data[size];
	}
	Self& SetAt(const SizeType index, const Type& val) {
		if (!IsEmptyOrNull()) {
			if ((*ref) && (**ref).IsShared()) {
				Type* const oldData = data;
				(**ref).DecrementRef();
				Construct(size);
				TypeTrait::Copy(data, oldData, index);
				TypeTrait::Copy(data + index + 1, oldData + index + 1, size - index - 1);
			}
			TypeTrait::Assign(data + index, val);
		}
		return *this;
	}
	SizeType GetSize()const {
		return ref && data ? size : 0;
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
		return IsShared() && ref == other.ref;
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

	Self& Append(const Type& val) {
		TypeTrait::Assign(GrowthAppend(1), val);
		return *this;
	}
	Self& Append(const Type& val, const SizeType count) {
		if (count) {
			TypeTrait::Fill(GrowthAppend(count), val, count);
		}
		return *this;
	}
	Self& Append(const Type* initialData, const SizeType dataSize) {
		if (initialData && dataSize) {
			TypeTrait::Copy(GrowthAppend(dataSize), initialData, dataSize);
		}
		return *this;
	}
	Self& Append(const Self& other) {
		if (other.ref && other.data && other.size) {
			TypeTrait::Copy(GrowthAppend(other.size), other.data, other.size);
		}
		return *this;
	}
	Self& Append(const Self& other, SizeType dataSize) {
		if (dataSize && other.ref && other.data && other.size) {
			if (dataSize >= other.size) {
				return Append(other);
			}
			else {
				return Append(other.data, dataSize);
			}
		}
		return *this;
	}
	Self& Append(const Self& other, SizeType dataSize, SizeType dataOffset) {
		if (dataOffset) {
			if (dataSize && other.ref && other.data && other.size) {
				if (dataSize > other.size - dataOffset) {
					dataSize = other.size - dataOffset;
				}
				return Append(other.data + dataOffset, dataSize);
			}
		}
		else {
			return Append(other.data, dataSize);
		}
		return *this;
	}

	Self& Prepend(const Type& val) {
		GrowthPrepend(1);
		TypeTrait::Assign(data, val);
		return *this;
	}
	Self& Prepend(const Type& val, SizeType count) {
		if (count) {
			GrowthPrepend(count);
			TypeTrait::Fill(data, val, count);
		}
		return *this;
	}
	Self& Prepend(const Type* prependData, SizeType dataSize) {
		if (prependData && dataSize) {
			GrowthPrepend(dataSize);
			TypeTrait::Copy(data, prependData, dataSize);
		}
		return *this;
	}
	Self& Prepend(const Self& other) {
		if (other.ref && other.data && other.size) {
			GrowthPrepend(other.size);
			TypeTrait::Copy(data, other.data, other.size);
		}
		return *this;
	}
	Self& Prepend(const Self& other, SizeType dataSize) {
		if (dataSize && other.ref && other.data && other.size) {
			if (dataSize >= other.size) {
				return Prepend(other);
			}
			else {
				return Prepend(other.data, other.size);
			}
		}
		return *this;
	}
	Self& Prepend(const Self& other, SizeType dataSize, SizeType dataOffset) {
		if (dataOffset) {
			if (dataSize && other.ref && other.data && other.size) {
				if (dataSize > other.size - dataOffset) {
					dataSize = other.size - dataOffset;
				}
				return Prepend(other.data + dataOffset, dataSize);
			}
		}
		else {
			return Prepend(other.data, dataSize);
		}
		return *this;
	}

	Self& Insert(const SizeType index, const Type& ch) {
		if (GrowthInsert(index, 1)) {
			TypeTrait::Assign(data + index, ch);
		}
		return *this;
	}
	Self& Insert(const SizeType index, const Type& ch, const SizeType count) {
		if (count && GrowthInsert(index, count)) {
			TypeTrait::Fill(data + index, ch, count);
		}
		return *this;
	}
	Self& Insert(const SizeType index, const Type* initialData, const SizeType dataSize) {
		if (initialData && dataSize && GrowthInsert(index, dataSize)) {
			TypeTrait::Copy(data + index, initialData, dataSize);
		}
		return *this;
	}
	Self& Insert(const SizeType index, const Self& other) {
		if (!other.IsEmpty() && GrowthInsert(index, other.size)) {
			TypeTrait::Copy(data + index, other.data, other.size);
		}
		return *this;
	}
	Self& Insert(const SizeType index, const Self& other, const SizeType dataSize) {
		if (dataSize >= other.size) {
			return Insert(index, other);
		}
		if (!other.IsEmpty() && GrowthInsert(index, dataSize)) {
			TypeTrait::Copy(data + index, other.data, dataSize);
		}
		return *this;
	}


};