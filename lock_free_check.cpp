#include <iostream>

#include <atomic>


int main()
{
	#ifdef ATOMIC_BOOL_LOCK_FREE
	std::cout << "ATOMIC_BOOL_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_CHAR_LOCK_FREE
	std::cout << "ATOMIC_CHAR_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_CHAR16_T_LOCK_FREE
	std::cout << "ATOMIC_CHAR16_T_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_CHAR32_T_LOCK_FREE
	std::cout << "ATOMIC_CHAR32_T_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_WCHAR_T_LOCK_FREE
	std::cout << "ATOMIC_WCHAR_T_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_SHORT_LOCK_FREE
	std::cout << "ATOMIC_SHORT_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_INT_LOCK_FREE
	std::cout << "ATOMIC_INT_LOCk_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_LONG_LOCK_FREE
	std::cout << "ATOMIC_LONG_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_LLONG_LOCK_FREE
	std::cout << "ATOMIC_LLONG_LOCK_FREE" << std::endl;
	#endif
	#ifdef ATOMIC_POINTER_LOCK_FREE
	std::cout << "ATOMIC_POINTER_LOCK_FREEE" << std::endl;
	#endif
	return 0;
}
