
#include <iostream>
#include <string>

class forward_string_slice_t
{
private:
	std::string& m_str;
	size_t m_begin;
public:
	forward_string_slice_t(std::string& str, size_t begin = 0) : m_str(str), m_begin(begin) { }
	~forward_string_slice_t() = default;

	const char* c_str() const { return m_str.c_str() + m_begin; }
	operator const char*() const { return c_str(); }
	size_t size() const { return m_str.size() - m_begin; }

	const std::string& get_original() const { return m_str; }
	std::string& get_original() { return m_str; }

	operator std::string&() { return m_str; }
	operator const std::string&() const { return m_str; }

	char& operator [](size_t index) { return m_str[m_begin + index]; }
	const char& operator [](size_t index) const { return m_str[m_begin + index]; }

	forward_string_slice_t get_slice(size_t begin)
	{
		return forward_string_slice_t(m_str, m_begin + begin);
	}

	const forward_string_slice_t get_slice(size_t begin) const
	{
		return forward_string_slice_t(m_str, m_begin + begin);
	}
};

template<typename T>
static void swap(T& v1, T& v2)
{
	T temp = v1;
	v1 = v2;
	v2 = temp;
}

static void permute(forward_string_slice_t str)
{
	size_t length = str.size();
	if(length == 0)
	{
		std::cout << str.get_original() << std::endl;
		return;
	}

	for(size_t i = 0; i < length; i++)
	{
		swap(str[0], str[i]);
		permute(str.get_slice(1));
		swap(str[0], str[i]);
	}
}

int main(int argc, const char* argv[])
{
	std::string str { "ABCD" };
	permute(str);
	return 0;
}