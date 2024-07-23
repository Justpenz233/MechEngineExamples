///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef EASTL_FUNCTIONAL_H
#define EASTL_FUNCTIONAL_H


#include <EABase/eabase.h>
#include <EASTL/internal/config.h>
#include <EASTL/internal/move_help.h>
#include <EASTL/type_traits.h>
#include <EASTL/internal/functional_base.h>
#include <EASTL/internal/mem_fn.h>


#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{
	///////////////////////////////////////////////////////////////////////
	// Primary C++ functions
	///////////////////////////////////////////////////////////////////////

	template <typename T = void>
	struct plus
	{
		EA_CPP14_CONSTEXPR T operator()(const T& a, const T& b) const
			{ return a + b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/plus_void
	template <>
	struct plus<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) + eastl::forward<B>(b))
			{ return eastl::forward<A>(a) + eastl::forward<B>(b); }
	};

	template <typename T = void>
	struct minus
	{
		EA_CPP14_CONSTEXPR T operator()(const T& a, const T& b) const
			{ return a - b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/minus_void
	template <>
	struct minus<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) - eastl::forward<B>(b))
			{ return eastl::forward<A>(a) - eastl::forward<B>(b); }
	};

	template <typename T = void>
	struct multiplies
	{
		EA_CPP14_CONSTEXPR T operator()(const T& a, const T& b) const
			{ return a * b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/multiplies_void
	template <>
	struct multiplies<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) * eastl::forward<B>(b))
			{ return eastl::forward<A>(a) * eastl::forward<B>(b); }
	};

    template <typename T = void>
    struct divides
    {
		EA_CPP14_CONSTEXPR T operator()(const T& a, const T& b) const
			{ return a / b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/divides_void
	template <>
	struct divides<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) / eastl::forward<B>(b))
			{ return eastl::forward<A>(a) / eastl::forward<B>(b); }
	};

    template <typename T = void>
    struct modulus
    {
		EA_CPP14_CONSTEXPR T operator()(const T& a, const T& b) const
			{ return a % b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/modulus_void
	template <>
	struct modulus<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) % eastl::forward<B>(b))
			{ return eastl::forward<A>(a) % eastl::forward<B>(b); }
	};

    template <typename T = void>
    struct negate
    {
		EA_CPP14_CONSTEXPR T operator()(const T& a) const
			{ return -a; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/negate_void
	template <>
	struct negate<void> 
	{
		typedef int is_transparent;
		template<typename T>
		EA_CPP14_CONSTEXPR auto operator()(T&& t) const
			-> decltype(-eastl::forward<T>(t))
			{ return -eastl::forward<T>(t); }
	};

	template <typename T = void>
	struct equal_to
	{
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a == b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/equal_to_void
	template <>
	struct equal_to<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) == eastl::forward<B>(b))
			{ return eastl::forward<A>(a) == eastl::forward<B>(b); }
	};

	template <typename T, typename Compare>
	bool validate_equal_to(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) == compare(b, a);
	}

    template <typename T = void>
    struct not_equal_to
    {
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a != b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/not_equal_to_void
	template <>
	struct not_equal_to<void> 
	{
		typedef int is_transparent;
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) != eastl::forward<B>(b))
			{ return eastl::forward<A>(a) != eastl::forward<B>(b); }
	};

	template <typename T, typename Compare>
	bool validate_not_equal_to(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) == compare(b, a); // We want the not equal comparison results to be equal.
	}

	/// str_equal_to
	///
	/// Compares two 0-terminated string types.
	/// The T types are expected to be iterators or act like iterators.
	/// The expected behavior of str_less is the same as (strcmp(p1, p2) == 0).
	///
	/// Example usage:
	///     hash_set<const char*, hash<const char*>, str_equal_to<const char*> > stringHashSet;
	///
	/// Note:
	/// You couldn't use str_equal_to like this:
	///     bool result = equal("hi", "hi" + 2, "ho", str_equal_to<const char*>());
	/// This is because equal tests an array of something, with each element by
	/// the comparison function. But str_equal_to tests an array of something itself.
	///
	/// To consider: Update this code to use existing word-based comparison optimizations, 
	/// such as that used in the EAStdC Strcmp function.
	///
	template <typename T>
	struct str_equal_to
	{
		EA_CPP14_CONSTEXPR bool operator()(T a, T b) const
		{
			while(*a && (*a == *b))
			{
				++a;
				++b;
			}
			return (*a == *b);
		}
	};

	template <typename T = void>
	struct greater
	{
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a > b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/greater_void
	template <>
	struct greater<void>
	{
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) > eastl::forward<B>(b))
			{ return eastl::forward<A>(a) > eastl::forward<B>(b); }
	};

	template <typename T, typename Compare>
	bool validate_greater(const T& a, const T& b, Compare compare)
	{
		return !compare(a, b) || !compare(b, a); // If (a > b), then !(b > a)
	}


	template <typename T, typename Compare>
	bool validate_less(const T& a, const T& b, Compare compare)
	{
		return !compare(a, b) || !compare(b, a); // If (a < b), then !(b < a)
	}

	/// str_less
	///
	/// Compares two 0-terminated string types. 
	/// The T types are expected to be iterators or act like iterators, 
	/// and that includes being a pointer to a C character array.
	/// The expected behavior of str_less is the same as (strcmp(p1, p2) < 0).
	/// This function is not Unicode-correct and it's not guaranteed to work
	/// with all Unicode strings.
	///
	/// Example usage:
	///     set<const char*, str_less<const char*> > stringSet;
	///
	/// To consider: Update this code to use existing word-based comparison optimizations, 
	/// such as that used in the EAStdC Strcmp function.
	///
	template <typename T>
	struct str_less
	{
		bool operator()(T a, T b) const
		{
			while(static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*a) == 
				  static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*b))
			{
				if(*a == 0)
					return (*b != 0);
				++a;
				++b;
			}

			char aValue = static_cast<typename remove_pointer<T>::type>(*a);
			char bValue = static_cast<typename remove_pointer<T>::type>(*b);

			typename make_unsigned<char>::type aValueU = static_cast<typename make_unsigned<char>::type>(aValue);
			typename make_unsigned<char>::type bValueU = static_cast<typename make_unsigned<char>::type>(bValue);

			return aValueU < bValueU;

			//return (static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*a) < 
			//        static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*b));
		}
	};

    template <typename T = void>
    struct greater_equal
    {
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a >= b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/greater_equal_void
	template <>
	struct greater_equal<void>
	{
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) >= eastl::forward<B>(b))
			{ return eastl::forward<A>(a) >= eastl::forward<B>(b); }
	};

	template <typename T, typename Compare>
	bool validate_greater_equal(const T& a, const T& b, Compare compare)
	{
		return !compare(a, b) || !compare(b, a); // If (a >= b), then !(b >= a)
	}

	template <typename T = void>
	struct less_equal
	{
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a <= b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/less_equal_void
	template <>
	struct less_equal<void>
	{
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) <= eastl::forward<B>(b))
			{ return eastl::forward<A>(a) <= eastl::forward<B>(b); }
	};

	template <typename T, typename Compare>
	bool validate_less_equal(const T& a, const T& b, Compare compare)
	{
		return !compare(a, b) || !compare(b, a); // If (a <= b), then !(b <= a)
	}

	template <typename T = void>
	struct logical_and
	{
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a && b; }
	};
	
	// http://en.cppreference.com/w/cpp/utility/functional/logical_and_void
	template <>
	struct logical_and<void>
	{
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) && eastl::forward<B>(b))
			{ return eastl::forward<A>(a) && eastl::forward<B>(b); }
	};

    template <typename T = void>
    struct logical_or
    {
		EA_CPP14_CONSTEXPR bool operator()(const T& a, const T& b) const
			{ return a || b; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/logical_or_void
	template <>
	struct logical_or<void>
	{
		template<typename A, typename B>
		EA_CPP14_CONSTEXPR auto operator()(A&& a, B&& b) const
			-> decltype(eastl::forward<A>(a) || eastl::forward<B>(b))
			{ return eastl::forward<A>(a) || eastl::forward<B>(b); }
	};

    template <typename T = void>
    struct logical_not
    {
		EA_CPP14_CONSTEXPR bool operator()(const T& a) const
			{ return !a; }
	};

	// http://en.cppreference.com/w/cpp/utility/functional/logical_not_void
	template <>
	struct logical_not<void>
	{
		template<typename T>
		EA_CPP14_CONSTEXPR auto operator()(T&& t) const
			-> decltype(!eastl::forward<T>(t))
			{ return !eastl::forward<T>(t); }
	};

	// not_fn_ret
	// not_fn_ret is a implementation specified return type of eastl::not_fn.
	// The type name is not specified but it does have mandated functions that conforming implementations must support.
	//
	// http://en.cppreference.com/w/cpp/utility/functional/not_fn
	//
	template <typename F>
	struct not_fn_ret
	{
		explicit not_fn_ret(F&& f) : mDecayF(eastl::forward<F>(f)) {}
		not_fn_ret(not_fn_ret&& f) = default;
		not_fn_ret(const not_fn_ret& f) = default;

		// overloads for lvalues
		template <class... Args>
		auto operator()(Args&&... args) &
		    -> decltype(!eastl::declval<eastl::invoke_result_t<eastl::decay_t<F>&, Args...>>())
		{ return !eastl::invoke(mDecayF, eastl::forward<Args>(args)...); }

		template <class... Args>
		auto operator()(Args&&... args) const &
		    -> decltype(!eastl::declval<eastl::invoke_result_t<eastl::decay_t<F> const&, Args...>>())
		{ return !eastl::invoke(mDecayF, eastl::forward<Args>(args)...); }

		// overloads for rvalues
		template <class... Args>
		auto operator()(Args&&... args) &&
		    -> decltype(!eastl::declval<eastl::invoke_result_t<eastl::decay_t<F>, Args...>>())
		{ return !eastl::invoke(eastl::move(mDecayF), eastl::forward<Args>(args)...); }

		template <class... Args>
		auto operator()(Args&&... args) const &&
		    -> decltype(!eastl::declval<eastl::invoke_result_t<eastl::decay_t<F> const, Args...>>())
		{ return !eastl::invoke(eastl::move(mDecayF), eastl::forward<Args>(args)...); }

		eastl::decay_t<F> mDecayF;
	};

	/// not_fn
	///
	/// Creates an implementation specified functor that returns the complement of the callable object it was passed.
	/// not_fn is intended to replace the C++03-era negators eastl::not1 and eastl::not2.
	///
	/// http://en.cppreference.com/w/cpp/utility/functional/not_fn
	///
	/// Example usage:
	///
	///		auto nf = eastl::not_fn([]{ return false; });
	///     assert(nf());  // return true
	///
	template <class F>
	inline not_fn_ret<F> not_fn(F&& f)
	{
		return not_fn_ret<F>(eastl::forward<F>(f));
	}


	///////////////////////////////////////////////////////////////////////
	// hash
	///////////////////////////////////////////////////////////////////////
	namespace Internal
	{
		// utility to disable the generic template specialization that is
		// used for enum types only.
		template <typename T, bool Enabled>
		struct EnableHashIf {};

		template <typename T>
		struct EnableHashIf<T, true>
		{
			size_t operator()(T p) const { return size_t(p); }
		};
	} // namespace Internal


	template <typename T> struct hash;

	template <typename T>
	struct hash : Internal::EnableHashIf<T, is_enum_v<T>> {};

	template <typename T> struct hash<T*> // Note that we use the pointer as-is and don't divide by sizeof(T*). This is because the table is of a prime size and this division doesn't benefit distribution.
		{ size_t operator()(T* p) const { return size_t(uintptr_t(p)); } };

	template <> struct hash<bool>
		{ size_t operator()(bool val) const { return static_cast<size_t>(val); } };

	template <> struct hash<char>
		{ size_t operator()(char val) const { return static_cast<size_t>(val); } };

	template <> struct hash<signed char>
		{ size_t operator()(signed char val) const { return static_cast<size_t>(val); } };

	template <> struct hash<unsigned char>
		{ size_t operator()(unsigned char val) const { return static_cast<size_t>(val); } };

	#if defined(EA_CHAR8_UNIQUE) && EA_CHAR8_UNIQUE
		template <> struct hash<char8_t>
			{ size_t operator()(char8_t val) const { return static_cast<size_t>(val); } };
	#endif

	#if defined(EA_CHAR16_NATIVE) && EA_CHAR16_NATIVE
		template <> struct hash<char16_t>
			{ size_t operator()(char16_t val) const { return static_cast<size_t>(val); } };
	#endif

	#if defined(EA_CHAR32_NATIVE) && EA_CHAR32_NATIVE
		template <> struct hash<char32_t>
			{ size_t operator()(char32_t val) const { return static_cast<size_t>(val); } };
	#endif

	// If wchar_t is a native type instead of simply a define to an existing type...
	#if !defined(EA_WCHAR_T_NON_NATIVE)
		template <> struct hash<wchar_t>
			{ size_t operator()(wchar_t val) const { return static_cast<size_t>(val); } };
	#endif

	template <> struct hash<signed short>
		{ size_t operator()(signed short val) const { return static_cast<size_t>(val); } };

	template <> struct hash<unsigned short>
		{ size_t operator()(unsigned short val) const { return static_cast<size_t>(val); } };

	template <> struct hash<signed int>
		{ size_t operator()(signed int val) const { return static_cast<size_t>(val); } };

	template <> struct hash<unsigned int>
		{ size_t operator()(unsigned int val) const { return static_cast<size_t>(val); } };

	template <> struct hash<signed long>
		{ size_t operator()(signed long val) const { return static_cast<size_t>(val); } };

	template <> struct hash<unsigned long>
		{ size_t operator()(unsigned long val) const { return static_cast<size_t>(val); } };

	template <> struct hash<signed long long>
		{ size_t operator()(signed long long val) const { return static_cast<size_t>(val); } };

	template <> struct hash<unsigned long long>
		{ size_t operator()(unsigned long long val) const { return static_cast<size_t>(val); } };

	template <> struct hash<float>
		{ size_t operator()(float val) const { return static_cast<size_t>(val); } };

	template <> struct hash<double>
		{ size_t operator()(double val) const { return static_cast<size_t>(val); } };

	template <> struct hash<long double>
		{ size_t operator()(long double val) const { return static_cast<size_t>(val); } };

	#if defined(EA_HAVE_INT128) && EA_HAVE_INT128
	template <> struct hash<uint128_t>
		{ size_t operator()(uint128_t val) const { return static_cast<size_t>(val); } };
	#endif


	///////////////////////////////////////////////////////////////////////////
	// string hashes
	//
	// Note that our string hashes here intentionally are slow for long strings.
	// The reasoning for this is so:
	//    - The large majority of hashed strings are only a few bytes long.
	//    - The hash function is significantly more efficient if it can make this assumption.
	//    - The user is welcome to make a custom hash for those uncommon cases where
	//      long strings need to be hashed. Indeed, the user can probably make a 
	//      special hash customized for such strings that's better than what we provide.
	///////////////////////////////////////////////////////////////////////////

	template <> struct hash<char*>
	{
		size_t operator()(const char* p) const
		{
			uint32_t c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint8_t)*p++) != 0)     // Using '!=' disables compiler warnings.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<const char*>
	{
		size_t operator()(const char* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint8_t)*p++) != 0)     // cast to unsigned 8 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

#if EA_CHAR8_UNIQUE
	template <> struct hash<char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint8_t)*p++) != 0)     // Using '!=' disables compiler warnings.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<const char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint8_t)*p++) != 0)     // cast to unsigned 8 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};
#endif


	template <> struct hash<char16_t*>
	{
		size_t operator()(const char16_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<const char16_t*>
	{
		size_t operator()(const char16_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<char32_t*>
	{
		size_t operator()(const char32_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<const char32_t*>
	{
		size_t operator()(const char32_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

#if defined(EA_WCHAR_UNIQUE) && EA_WCHAR_UNIQUE
	template<> struct hash<wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;    // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while ((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template<> struct hash<const wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;    // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while ((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};
#endif

	/// string_hash
	///
	/// Defines a generic string hash for an arbitrary EASTL basic_string container.
	///
	/// Example usage:
	///    eastl::hash_set<MyString, eastl::string_hash<MyString> > hashSet;
	///
	template <typename String>
	struct string_hash
	{
		typedef String                                         string_type;
		typedef typename String::value_type                    value_type;
		typedef typename eastl::make_unsigned<value_type>::type unsigned_value_type;

		size_t operator()(const string_type& s) const
		{
			const unsigned_value_type* p = (const unsigned_value_type*)s.c_str();
			uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while((c = *p++) != 0)
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};


} // namespace eastl

#include <EASTL/internal/function.h>

#endif // Header include guard







