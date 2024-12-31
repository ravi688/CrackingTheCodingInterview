#include <type_traits>

struct Data
{
	int value;
};

template<typename ValueType>
static void function(ValueType& value, auto visitor)
{
	visitor(value);
}

template<typename T>
struct remove_pointer
{
	typedef T type;
};

template<typename T> struct remove_pointer<T*>
{
	typedef T type;
};

template<typename T>
struct is_refers_to_const_value
{
	static constexpr bool value = std::is_const<typename remove_pointer<typename std::remove_reference<T>::type>::type>::value;
};

template<typename T> requires(std::is_pointer<T>::value)
static void functionWrapper(T& t)
{
	// NOTE: if T is const Data*
	// then decltype(t->value) would return 'int'
	// That means decltype() doesn't care about the cv-qualification of t when t->value is passed into it!
	// Therefore we need to add const qualification based on if t refers to a constant value
	using ValueType = typename std::conditional<is_refers_to_const_value<decltype(t)>::value,
				typename std::add_const<decltype(t->value)>::type,
				decltype(t->value)>::type;
	function(t->value, [](ValueType& value) { });
}

static void myFunction()
{
	Data* data = new Data { };
	functionWrapper(data);
	const Data* const_data = data;
	functionWrapper(const_data);
}
