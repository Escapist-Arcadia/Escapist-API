#pragma once

#include<cassert>
#include<atomic>

namespace EscapistPrivate
{
	class ReferenceCount
	{
	private:
		std::atomic<int> ref;

	public:
		ReferenceCount() :ref() {}
		ReferenceCount(const int& value) :ref(value) {}
		ReferenceCount(const ReferenceCount& value) = delete;

		int GetValue()const {
			return ref.load(std::memory_order::memory_order_acquire);
		}
		ReferenceCount& SetValue(const int& value) {
			ref.store(value, std::memory_order::memory_order_release);
			return *this;
		}

		ReferenceCount& IncrementRef() {
			ref.fetch_add(1, std::memory_order::memory_order_acq_rel);
			return *this;
		}
		ReferenceCount& DecrementRef() {
			ref.fetch_sub(1, std::memory_order::memory_order_acq_rel);
			return *this;
		}

		bool IsShared()const {
			return GetValue() > 1;
		}
	};
}