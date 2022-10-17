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
	class TypeTrait
	{
		void Copy(T* dest, const T* src, SizeType size) {
			::memcpy((void*)dest, (const void*)src, size * sizeof(T));
		}
		void SafeCopy(T* dest, SizeType capacity, const T* src, SizeType size) {
			::memcpy_s((void* const)dest, capacity * sizeof(T), (const void* const)src, size * sizeof(T));
		}
		void Move(T* dest, const T* src, SizeType size) {
			::memmove((void*)dest, (const void*)src, size * sizeof(T));
		}
		void SafeMove(T* dest, SizeType capacity, const T* src, SizeType size) {
			::memmove_s((void* const)dest, capacity * sizeof(T), (const void* const)src, size * sizeof(T));
		}
		static void Assign(T* dest, const T& val) {
			::memcpy((void*)dest, (const void*)&val, sizeof(T));
		}
		static void Fill(T* dest, const T& val, size_t count) {
			for (; count > 0; --count, ++dest)
				::memcpy((void*)dest, (const void*)&val, sizeof(T));
		}
		static int Equals(const T& left, const T& right) {
			if (left == right)
				return 0;
			else
				if (left < right)
					return -1;
				else
					return 1;
		}
		static int Compare(const T* left, const T* right, size_t size) {
			int rtn;
			for (; !(rtn = TypeTrait<T>::Equals(*left, *right)) && size > 0; ++left, ++right, --size);
			return rtn;
		}

		static size_t GetSize(const T* src) {
			size_t count(0);
			while (!TypeTrait<T>::Equals(*src, T()))
				++count;
			return count;
		}
		static size_t GetCount(const T* src) {
			return GetSize(src);
		}
		static size_t GetLength(const T* src) {
			return GetSize(src);
		}
	};

#ifdef ESCAPIST_HAS_NAMESPACE
}
#endif

namespace EscapistPrivate
{
	enum class TypeTraitPattern :short
	{
		Pod,
		Generic,
		NonDefault
	};

	template<typename T>
	constexpr bool TypeTraitPatternAutoDefiner = !std::_Is_any_of_v<std::remove_cv_t<T>,
		bool, char, signed char, unsigned char, wchar_t, char16_t, char32_t,
		short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long,
		float, double, long double,
		void, std::nullptr_t>;

	template<typename T>
	struct TypeTraitPatternDefiner
	{
		static const TypeTraitPattern Pattern = TypeTraitPatternAutoDefiner<T> ? TypeTraitPattern::Generic : TypeTraitPattern::Pod;
	};

	template<typename T>
	class PodTypeTrait :public
#ifdef ESCAPIST_HAS_NAMESPACE
		Escapist::
#endif
		TypeTrait<T>
	{
		using Base = typename
#ifdef ESCAPIST_HAS_NAMESPACE
			Escapist::
#endif
			TypeTrait<T>;

	public:
		void Copy(T* dest, const T* src, Escapist::SizeType size) {
			::memcpy((void*)dest, (const void*)src, size * sizeof(T));
		}
		void SafeCopy(T* dest, Escapist::SizeType capacity, const T* src, Escapist::SizeType size) {
			::memcpy_s((void* const)dest, capacity * sizeof(T), (const void* const)src, size * sizeof(T));
		}
		void Move(T* dest, const T* src, Escapist::SizeType size) {
			::memmove((void*)dest, (const void*)src, size * sizeof(T));
		}
		void SafeMove(T* dest, Escapist::SizeType capacity, const T* src, Escapist::SizeType size) {
			::memmove_s((void* const)dest, capacity * sizeof(T), (const void* const)src, size * sizeof(T));
		}

		static void Assign(T* dest, const T& val) {
			::memcpy((void*)dest, (const void*)&val, sizeof(T));
		}
		static void Fill(T* dest, const T& val, size_t count) {
			for (; count > 0; --count, ++dest)
				::memcpy((void*)dest, (const void*)&val, sizeof(T));
		}

		static int Equals(const T& left, const T& right) {
			return Base::Equals(left, right);
		}
		static int Compare(const T* left, const T* right, Escapist::SizeType size) {
			return Base::Compare(left, right, size);
		}

		static size_t GetSize(const T* src) {
			return Base::GetSize(src);
		}
		static size_t GetCount(const T* src) {
			return Base::GetCount(src);
		}
		static size_t GetLength(const T* src) {
			return Base::GetLength(src);
		}

		static void Destroy(T* ptr) {}
	};

	template<typename T>
	class GenericTypeTrait :public Escapist::TypeTrait<T>
	{
		using Base = typename Escapist::TypeTrait<T>;

	public:
		static void Copy(T* dest, const T* src, Escapist::SizeType size)
		{
			for (; size > 0; ++dest, ++src)
				new(dest)T(*src);
		}
		static void Move(T* dest, const T* src, Escapist::SizeType size)
		{
			if (dest <= src || dest >= (src + size))
			{
				for (; size > 0; ++dest, ++src)
					new(dest)T(*src);
			}
			else
			{
				dest = dest + size - 1;
				src = src + size - 1;

				for (; size > 0; --dest, --src)
					new(dest)T(*src);
			}
		}

		static void Assign(T* dest, const T& val) { new(dest)T(val); }
		static void Fill(T* dest, const T& val, Escapist::SizeType count)
		{
			for (; count > 0; --count, ++dest)
				new(dest)T(val);
		}

		static int Equals(const T& left, const T& right) { return Base::Equals(left, right); }
		static int Compare(const T* left, const T* right, Escapist::SizeType size) { return Base::Compare(left, right, size); }

		static size_t GetSize(const T* src) { return Base::GetSize(src); }
		static size_t GetCount(const T* src) { return Base::GetCount(src); }
		static size_t GetLength(const T* src) { return Base::GetLength(src); }

		static void Destroy(T* ptr) { ptr->~T(); }
		static void Destroy(T* ptr, Escapist::SizeType count) {
			for (; count > 0; ++ptr, --count)
				ptr->~T();
		}
	};

	template<typename T, typename = void>
	struct TypeTraitPatternSelector
	{
		using TypeTrait = EscapistPrivate::PodTypeTrait<T>;
	};

	template<typename T>
	struct TypeTraitPatternSelector<T,
		typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Pod)>::type>
	{
		using TypeTrait = EscapistPrivate::PodTypeTrait<T>;
	};

	template<typename T>
	struct TypeTraitPatternSelector<T,
		typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::Generic)>::type>
	{
		using TypeTrait = EscapistPrivate::GenericTypeTrait<T>;
	};

	template<typename T>
	struct TypeTraitPatternSelector<T,
		typename std::enable_if<(TypeTraitPatternDefiner<T>::Pattern == TypeTraitPattern::NonDefault)>::type>
	{
		using TypeTrait = 
#ifdef ESCAPIST_HAS_NAMESPACE
			Escapist::
#endif // ESCAPIST_HAS_NAMESPACE
			TypeTrait<T>;
	};
}

#define DefineTypeTrait(T,P) \
template<>\
struct EscapistPrivate::TypeTraitPatternDefiner<T>\
{\
	static const EscapistPrivate::TypeTraitPattern Pattern = P;\
}
#define DefinePodTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::Pod)
#define DefineGenericTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::Generic)
#define DefineNonDefaultTypeTrait(T) DefineTypeTrait(T,EscapistPrivate::TypeTraitPattern::NonDefault)

DefinePodTypeTrait(bool);
DefinePodTypeTrait(char);
DefinePodTypeTrait(signed char);
DefinePodTypeTrait(unsigned char);
DefinePodTypeTrait(wchar_t);
DefinePodTypeTrait(char16_t);
DefinePodTypeTrait(char32_t);
DefinePodTypeTrait(short);
DefinePodTypeTrait(unsigned short);
DefinePodTypeTrait(int);
DefinePodTypeTrait(unsigned int);
DefinePodTypeTrait(long);
DefinePodTypeTrait(unsigned long);
DefinePodTypeTrait(long long);
DefinePodTypeTrait(unsigned long long);
DefinePodTypeTrait(float);
DefinePodTypeTrait(double);
DefinePodTypeTrait(long double);