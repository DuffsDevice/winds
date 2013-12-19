#ifndef _WIN_T_TUPLEFUN_
#define _WIN_T_TUPLEFUN_

#include <cstddef>     // for std::size_t
#include "_type/type.h"

namespace _tupleFun
{
	template<typename T, T... I>
	struct integer_sequence {
	  using value_type = T;

	  static constexpr std::size_t size() noexcept {
		return sizeof...(I);
	  }
	};

	namespace integer_sequence_detail {
	template <typename, typename> struct concat;

	template <typename T, T... A, T... B>
	struct concat<integer_sequence<T, A...>, integer_sequence<T, B...>> {
	  typedef integer_sequence<T, A..., B...> type;
	};

	template <typename T, int First, int Count>
	struct build_helper {
	  using type = typename concat<
		typename build_helper<T, First,           Count/2>::type,
		typename build_helper<T, First + Count/2, Count - Count/2>::type
	  >::type;
	};

	template <typename T, int First>
	struct build_helper<T, First, 1> {
	  using type = integer_sequence<T, T(First)>;
	};

	template <typename T, int First>
	struct build_helper<T, First, 0> {
	  using type = integer_sequence<T>;
	};

	template <typename T, T start , T end>
	using builder = typename build_helper<T, start, end-start>::type;
	} // namespace integer_sequence_detail

	template <typename T, T start , T end>
	using make_integer_sequence = integer_sequence_detail::builder<T, start, end>;

	template <size_t... I>
	using index_sequence = integer_sequence<std::size_t, I...>;

	template<size_t start , size_t end>
	using make_index_sequence = make_integer_sequence<size_t, start, end>;

	#include <tuple>

	template<typename... TN, size_t... IN>
	auto select( const _tuple<T...>& arg , index_sequence<IN...>) 
	  -> decltype(make_tuple(get<IN>(arg)...))
	{
	  return make_tuple(get<IN>(arg)...);
	}

	template <size_t start, size_t end , typename... T>
	auto select_sequence(const _tuple<T...>& t)
	  -> decltype(select(t, make_index_sequence<start, end>()))
	{
	  return select(t, make_index_sequence<start, end>());
	}
	
} // Namespace

#endif