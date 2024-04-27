#include <iostream>
#include <cstring> // strlen
#include <ctype.h> // tolower
#include <stdint.h> // uint32_t
#include <limits> // std::numeric_limits<int64_t>::max()
#include <queue> // std::queue<int>
#include <map> // std::map<int, int>
#include <inttypes.h>


// 16.1 Write a function to swap a number in place (that, without temporary variables)

template<typename T>
void problem1_print_header();

template<>
void problem1_print_header<int>()
{
	std::cout << "Problem 1 <int>: " << std::endl;
}

template<>
void problem1_print_header<float>()
{
	std::cout << "Problem 1 <float>: " << std::endl;
}

template<typename T>
void problem1()
{
	problem1_print_header<T>();
	T a = 100;
	T b = 200;

	std::cout << "\t a = " << a << std::endl;
	std::cout << "\t b = " << b << std::endl;

	b += a;
	a = b - a;
	b = b - a;

	std::cout << "\t after swapping inplace: " << std::endl;
	std::cout << "\t a = " << a << std::endl;
	std::cout << "\t b = " << b << std::endl;
}

// 16.2 Design a method to find the frequency of occurrences of any given word in 
// a book. What if we were running this algorithm multiple times?

std::size_t get_frequency1(const char* book, const char* word)
{
	std::size_t freq = 0;
	std::size_t book_len = strlen(book);
	std::size_t word_len = strlen(word);
	for(std::size_t i = 0; i < book_len; i++)
	{
		std::size_t j = 0;
		while((j < word_len) && (!isspace(book[i])) && (tolower(book[i]) == word[j]))
		{
			++i;
			++j;
		}
		if(j == word_len)
			freq += 1;
	}
	return freq;
}

std::size_t get_frequency2(const char* book, const char* word)
{
	// TODO: implement solution no 2
	return 0;
}

std::size_t get_frequency3(const char* book, const char* word)
{
	// TODO: implement solution no 3
	return 0;
}

void problem2()
{
	// Solution no 1:
	//
	// 	get_frequency(book, word):
	//		freq = 0
	//		book_len = strlen(book)
	//		for i = 0 until book_len:
	//			j = 0
	//			while j < word_len && !isspace(book[i]) && book[i] == word[j]:
	//				++i
	//				++j
	//			if j == word_len:
	//				freq += 1
	//		return freq
	//
	// If we were to run the above proposed solution multiple times then it will be better to cache precaculated
	// results
	//
	// Solution no 2:
	//
	//	Solution no 1:
	//
	//	get_frequency(book, word):
	//		sorted_list = { }
	//		for each word in book:										// O(book_len)
	//			hash = calculate_hash(word)
	//			sorted_list.add_pair(hash, &word)						// AVL Tree or RB Tree
	//		hash = calculate_hash(word)
	//		index = sorted_list.find_first(hash) 						// O(log2(word_cnt)), Binary Search
	//		freq = 0
	//		while sorted_list[index].first == hash:						// O(word_len x (num_collisions + num_frequency))
	//			if strcmp(word, sorted_list[index].second) == 0:
	//				freq += 1
	//		return freq
	//
	//	Solution no 2 (more efficient, as we also precalculate the frequency):
	//	
	//	get_frequency(book, word):
	//		hash_map<string, int> map;
	//		for each word in book:
	//			pair = map.get_or_add(word)
	//			pair->second += 1
	//		pair = hash_map.get(word) 			// O(word_len x num_collisions)
	// 		if pair != null:
	//			return pair->second
	//		else
	//			return 0
	
	std::cout << "Problem 2: " << std::endl;

	const char* book = "This is a book about this universe which contains this earth";
	const char* word = "this";

	std::size_t freq = get_frequency1(book, word);
	std::cout << "\t Solution no 1" << std::endl;
	std::cout << "\t Frequency of \"" << word << "\" in \"" << book << "\" is: " << freq << std::endl;

	freq = get_frequency2(book, word);
	std::cout << "\t Solution no 2" << std::endl;
	std::cout << "\t Frequency of \"" << word << "\" in \"" << book << "\" is: " << freq << std::endl;

	freq = get_frequency3(book, word);
	std::cout << "\t Solution no 3" << std::endl;
	std::cout << "\t Frequency of \"" << word << "\" in \"" << book << "\" is: " << freq << std::endl; 
}


// 16.3 Given two straight line segments (represented as a start point and an end point), compute
// the point of intersection, if any.

void problem3()
{
	// Solution no 1 (using line equation and internal division check):
	//	
	//	Math:
	//		(y - b1) / (x - a1) = (d1 - b1) / (c1 - a1) = m1
	//	 	(y - b2) / (x - a2) = (d2 - b2) / (c2 - a2) = m2
	//		y = m1 * (x - a1) + b1    - (1)
	//		y = m2 * (x - a2) + b2    - (2)
	//		Solving eqn (1) and (2):
	//			m1 * (x - a1) + b1 = m2 * (x - a2) + b2
	//			x *(m1 - m2) - m1 * a1 + b1 = -m2 * a2 + b2
	//			x = (m1 * a1 - b1 + b2 - m2 * a2) / (m1 - m2)
	//		Putting the value of x into (1), we can get the value of y corresponding to the x
	//	
	//		To check if the point of intersection is also the point of intersection of the two segments,
	//		We can check if the point internally divides either of segments.
	//			Formula for internal division is:
	//					x = (m * x1 + n * x2) / (m + n)
	//					y = (m * y1 + n * y2) / (m + n)
	//			So,
	//					m * x + n * x = m * x1 + n * x2
	//					m * (x - x1) + n * (x - x2) = 0    - (1)
	//			Similarly, m * (y - y1) + n * (y - y2) = 0    - (2)
	//			Solving (1) and (2), we can get the ratios m and n.
	//		If either of m or n is negative, that means the point of intersection lies off the segments.
	//
	// Soution no 2 (using line equation and opposite side check):
	//		use line segments as L { (x1, y1), (x2, y2) }
	//		L(x, y) = m * (x - x1) + y1 - y, where m = (y2 - y1) / (x2 - x1)
	//		is_L2end = L1(L2.end.x, L2.end.y) < 0
	//		is_L2begin = L1(L2.start.x, L2.start.y) < 0
	//		if (is_L2end XOR is_L2start) && (is_L1end XOR is_L1start):
	//			point = calculate_point_of_intersection(seg1, seg2)
	//			return point 
	//		else
	//			return null
}

// 16.4 Design an algorithm to figure out if someone has won a game of tic-tac-toe

void problem4()
{
	//	Solution no 1
	//
	//	get_int(x):
	//		switch x:
	//			case 'O' : return 1
	//			case 'X' : return -1
	//			default: return 0
	//
	//	has_won(board):
	//		for i = 0 until board.size:
	//			sum = 0
	//			for j = 0 until board.size:
	//				sum += get_int(board[i][j])
	//			if sum == board.size:
	//				return 'O' won
	//			else if sum == -board.size:
	//				return 'X' won
	//		for i = 0 until board.size:
	//			sum = 0
	//				for j = 0 until board.size:
	//					sum += get_int(board[j][i])
	//			if sum == board.size:
	//				return 'O' won
	//			else if sum == -board.size:
	//				return 'X' won
	//		sum1 = 0
	//		sum2 = 0
	//		for i = 0 until board.size:
	//			sum1 += board[i][i]
	//			sum2 += board[i][board.size - i - 1]
	//		if sum1 == board.size || sum2 == board.size:
	//			return 'O' won
	//		else if sum1 == -board.size || sum2 == -board.size:
	//			reutrn 'X' won
	//
	//	Solution no 2 (merge all three loops into one):
	//
	//	has_won(board):
	//		sum3 = 0
	//		sum4 = 0
	//		for i = 0 until board.size:
	//			sum3 += get_int(board[i][i])
	//			sum4 += get_int(board[i][board.size - i - 1])
	//			sum1 = 0
	//			sum2 = 0
	//			for j = 0 until board.size:
	//				sum1 += get_int(board[i][j])
	//				sum1 += get_int(board[j][i])
	//			if sum1 == board.size || sum2 == board.size:
	//				return 'O' won
	//			else if sum1 == -board.size || sum2 == -board.size:
	//				return 'X' won
	//		if sum3 == board.size || sum4 == board.size:
	//			return 'O' won
	//		else if sum3 == -board.size || sum4 == -board.size:
	//			return 'X' won
	//
	// 	Solution no 3 (incremental):
	//		
	//	has_won(board, x, y):
	//		if x == y:
	//			sum1 = 0
	//			sum2 = 0
	//			for i = 0 until board.size:
	//				sum1 += get_int(board[i][i])
	//				sum2 += get_int(board[i][board.size - i - 1])
	//			if sum1 == board.size || sum2 == board.size:
	//				return 'O' won
	//			else if sum1 == -board.size || sum2 == board.size:
	//				return 'X' won
	//		sum1 = 0
	//		sum2 = 0
	//		for i = until board.size
	//			sum1 += get_int(board[y][i])
	//			sum2 += get_int(board[i][x])
	//		if sum1 == board.size || sum2 == board.size:
	//			return 'O' won
	//		else if sum1 == -board.size || sum2 == -board.size:
	//			return 'X' won

}


// 16.5 Write an algorithm which computes the number of trailing zeros in n factorial

void problem5()
{
	// 0! = 1
	// 1! = 1
	// 2! = 2
	// 3! = 6
	// 4! = 4 x 3 x 2 x 1 = 24
	// 5! = 5 x 4 x 3 x 2 x 1 = 120
	// 6! = 6 x 5 x 4 x 3 x 2 x 1 = 720
	// 7! = 7 x 6 x 5 x 4 x 3 x 2 x 1 = 5040

	// Solution no 1 (using legendre's formula for computing prime factorization):
	//
	// 	get_num_powers(n, t)
	//		powers = 0
	//		while true:
	//			c = floor(n / t)
	//			if c == 0:
	//				break
	//			else:
	//				t *= t
	//				powers += c
	//		return powers
	//
	//	get_num_trailing_zeros(n):
	//		powers_2 = get_num_powers(n, 2)
	//		powers_5 = get_num_powers(n, 5)
	//		return min(powers_2, powers_5)
	//
	// Solution no 2 (calculate factorial first, and check how many times it is divisible by 10)
	//	
	//	get_factorial(n):
	//		t = 1
	//		for i = 2; i <=n; i++:
	//			t *= i
	//		return t
	//	
	//	get_num_trailing_zeros(n):
	//		f = get_factorial(n)
	//		z = -1
	//		while(f != 0)
	//			z++
	//			f /= 10
	//		return max(z, 0)
}

void problem6()
{
	// Solution no 1 (brute force):
	//
	//  Time complexity: O(arr1.size() * arr2.size())
	//	smallest_diff(arr1, arr2):
	//		min_diff = 0
	//		for i = 0; i < arr1.size(); i++:
	//			for j = 0; j < arr2.size(); j++:
	//				diff = abs(arr1[i] - arr2[j])
	//				if min_diff > diff:
	//					min_diff = diff
	//		return min_diff
	//
	// Solution no 2:
	//
	//	smallest_diff(arr1, arr2):
	//		sort_ascending(arr1)
	//		sort_ascending(arr2) 
	//		min_diff = 0
	//		while a < arr1.size() && b < arr2.size():
	//			diff = abs(arr1[a] - arr2[b])
	//				if min_diff > diff:
	//					min_diff = diff
	//			if arr1[a] > arr2[b]:
	//				b++
	//			else:
	//				a++
	//		return min_diff
}

// 16.7 Write a method that finds the maximum of two numbers. You should not use if-else or any other
// comparison operator.

template<typename T>
void problem7();

int flip_bit(int bit)
{
	return 1 ^ bit;
}

void problem7_sol2(int a, int b)
{
	std::cout<< "\t Solution no 2: " << std::endl;
	int diff = a - b;
	int index = ((unsigned int)(diff >> 31)) & 0x1;
	int findex = flip_bit(index);
	int max = a * findex + b * index;
	std::cout << "\t Max of " << a << " and " << b << " is: " << max << std::endl;
}

template<>
void problem7<int>()
{
	// Solution no 1:
	//		get_max(a, b):
	//				diff = a - b
	//				index = (unsigned)(diff & (1 << 31)) >> 31
	//				arr[2] = { a, b }
	//				return arr[index]
	//
	// Solution no 2:
	//		flip(x):
	//			return 1 ^ x
	//		get_max(a, b):
	//			diff = a - b
	//			index = ((unsigned int)(diff >> 31)) & 0x1
	//			findex = flip(index)
	//			return findex * a + index * b

	std::cout << "Problem 7 <int>: " << std::endl;

	int a = 200;
	int b = -345;
	int diff = a - b;
	int index = ((unsigned int)(diff & (1 << 31))) >> 31;
	int arr[2] = { a, b };
	int max = arr[index];

	std::cout << "\t Max of " << a << " and " << b << " is: " << max << std::endl;

	problem7_sol2(a, b);
}

template<>
void problem7<float>()
{
	std::cout << "Problem 7 <float>: " << std::endl;
	float a = 200;
	float b = 345;
	int diff = a - b;
	int index = ((unsigned int)(diff & (1 << 31))) >> 31;
	float arr[2] = { a, b };
	float max = arr[index];

	std::cout << "\t Max of " << a << " and " << b << " is: " << max << std::endl;
}

// English Int: Given any integer, print an English phrase that describes the integer (e.g., "OneThousand, Two Hundred Thirty Four").
void problem8()
{
	// 3453434
	// Three million
	// Four hundred fifty three thousands
	// Four hundred thirty four

	//
	//  units = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" }
	//	tens1 = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen",
	//	  "Eighteen", "Nineteen" }
	//	tens2 = { "Twenty", "Thirty", "Fourty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" }
	//	arr1 = { "", "Thousands", "Millions", "Billions", "Trillions" } 	
	//	str = to_string(n)
	// 	len = strlen(str)
	// 	for i = len - 1; i >= 0; --i
	//		ch = str[len - i - 1]
	//		k = i % 3
	//		if k != 1:
	//			print(units[ch - '0'])
	//			putchar(' ')
	//			if k == 2:
	//				print('Hundred ')
	//		else:
	//			if ch == '1':
	//				--i
	//				ch = str[len - i - 1]
	//				print(tens1[ch - '0'])
	//				putchar(' ')
	//			else:
	//				print(tens2[ch - '2'])
	//				putchar(' ')
	//		if k == 0:
	//			k = i / 3
	//			print(arr1[k])
	//			putchar(' ')
	
	uint64_t n = 3453434;

	const char* units[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" };
	const char* tens1[] = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };
	const char* tens2[] = { "Twenty", "Thirty", "Fourty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
	const char* arr1[]  = { "", "Thousands", "Millions", "Billions", "Trillions" };

	char str[16];
	sprintf(str, PRIu64, n);
	int len = strlen(str);
	for(int i = len - 1; i >= 0; --i)
	{
		char ch = str[len - i - 1];
		if(ch == '0')
			continue;
		int k = i % 3;
		if(k != 1)
		{
			std::cout << units[ch - '0'] << " ";
			if(k == 2)
			{
				std::cout << "Hundred ";
			}
		}
		else
		{
			if(ch == '1')
			{
				--i;
				ch = str[len - i - 1];
				std::cout << tens1[ch - '0'] << " ";
			}
			else
			{
				std::cout << tens2[ch - '2'] << " ";
			}
		}
		if (k == 0)
		{
			k = i / 3;
			std::cout << arr1[k] << " ";
		}
	}

	std::cout << std::endl;
}

int64_t flip_sign(int64_t a)
{
	// we also might calculate two's complement by ~a +1
	int t = (a < 0) ? 1 : -1;
	int64_t n = 0;
	while(a != 0)
	{
		n += t;
		a += t;
	}
	return n;
}

int64_t multiply(int64_t a, int64_t b)
{
	int64_t s = (a ^ b) >> (63);
	a = std::abs(a);
	b = std::abs(b);
	int64_t m = std::max(a, b);
	a = (m == a) ? b : a;
	int64_t res = 0;
	for(int64_t i = 0; i < a; i++)
		res += m;
	return (s < 0) ? flip_sign(res) : res;
}

int64_t subtract(int64_t a, int64_t b)
{
	return a + flip_sign(b);
}

int64_t divide(int64_t a, int64_t b)
{
	int64_t s = (a ^ b) >> 63;
	a = std::abs(a);
	b = std::abs(b);
	int64_t r = 0;
	while(a > b)
	{
		a -= b;
		r += 1;
	}
	return (s < 0) ? flip_sign(r) : r;
}


// Operations: Write methods to implement the multiply, subtract, and divide operations for integers. 
// The results of all of these are integers. Use only the add operator
void problem9()
{
	// Operations
	// 1. Multiply
	// 2. Subtract
	// 3. Divide

	// Multiply:
	//	Solution no 1: (Doesn't work!)
	//		a = 2324
	//		b = 8
	//		s = (a ^ b) >> (INT_BITS - 1) // arithemtic right shift by INT_BITS - 1 bits
	//		a = abs(a)
	//		b = abs(b)
	//		m = max(a, b)
	//		a = (m == a) ? b : a
	//		r = 0
	//		for i = 0 until a:
	//			r += m
	//		r += INT_MAX & s
	//		ret r
	//
	//	Solution no 2:
	//		flip_sign(a):
	//			t = (a > 0) ? -1 : 1
	//			neg = 0
	//			while a != 0:
	//				neg += t
	//				a += t
	//		ret neg
	//		mul(a, b):
	//			a = 2324
	//			b = 8
	//			s = (a ^ b) >> (INT_BITS - 1)
	//			a = abs(a)
	//			b = abs(b)
	//			m = max(a, b)
	//			a = (m == a) ? b : a
	//			r = 0
	//			for i = 0 until a:
	//				r += m
	//			ret (s < 0) ? flip_sign(r) : r
	//
	// Subtract:
	//	Solution no 1:
	//		flip_sign(a):
	//			ret a + INT_MAX	
	//		sub(a, b):
	//			ret a + flip_sign(b)
	// 	Solution no 2:
	//		flip_sign(a):
	//			t = (a > 0) ? -1 : 1
	//			n = 0
	//			while a != 0:
	//				n += t
	//				a += t
	//			ret n
	//		sub(a, b):
	//			ret a + flip_sign(b)
	//
	// Divide: Division is just the reverse of multiplication
	//	Solution no 1:
	//		a = 2324 // dividend
	//		b = 8 // divisor
	//		s = (a ^ b) >> (INT_BITS - 1) // arithemtic right shift by INT_BITS - 1 bits
	//		a = abs(a)
	//		b = abs(b)
	//		r = 0
	//		k = 0
	//		while k != a:
	//			r += 1
	//			k += b
	//		r += INT_MAX & s // doesn't work
	//		ret r
	//
	int64_t a = 9;
	int64_t b = -343;

	int64_t c = multiply(a, b);
	std::cout << "Mul(" << a <<"," << b <<") = " << c << std::endl;

	c = subtract(a, b);
	std::cout << "Sub(" << a <<"," << b <<") = " << c << std::endl;
	
	c = divide(a, b);
	std::cout << "Div(" << a <<"," << b <<") = " << c << std::endl;
}

// Living People: Given a list of people with their birth and death years, implement a method to
// compute the year with the most number of people alive. You may assume that all people were born
// between 1900 and 2000 (inclusive). If a person was alive during any portion of that year, they should
// be included in that year's count. For example, Person (birth= 1908, death= 1909) is included in the
// counts for both 1908 and 1909
void problem10()
{
	// Solution no 1 (Brute Force):
	//
	// 	Time Complexity: O(num_people)
	//	get_num_people(year, list)
	//		num_people = 0:
	//		for i = 0 until list.count():
	//			pair = list.get(i)
	//			if year >= pair.l && year <= pair.h:
	//				num_people++
	//		ret num_people
	//
	// 	Time Complexity: O(num_people * (max_h - max_l) + num_people)
	//					 O(num_people * (max_h - max_l))
	//	most_living(list):
	//		max_l = 0
	//		max_h = 0
	//		for i = 0 until list.count():
	//			if max_h < list.get(i).h:
	//				max_h = list.get(i).h
	//			if min_l < list.get(i).l:
	//				min_l = list.get(i).l
	//		max_num_people = 0
	//		max_live_year = max_l
	//		for i = max_l until equals max_h:
	//			num_people = get_num_people(i, list)
	//			if max_num_people < num_people:
	//				max_num_people = num_people
	//				max_live_year = i 
	//		ret max_live_year
	//
	// Solution no 2:
	//
	// 	Time Complexity: O(num_people + num_people * log(num_people) + 2 * num_people)
	//					 O(3 * num_people + num_people * log(num_people))
	//					 O(num_people * log(num_people))
	//	most_living(list):
	//		arr[list.count * 2]
	//		for i = 0 until list.count():
	//			arr[2 * i + 0] = list.get(i).l
	//			arr[2 * i + 1] = -list.get(i).h
	//		sort(arr, (v1, v2) { ret abs(v1) > abs(v2) }) // sort in ascending order
	//		max_alive = 0
	//		for i = 0 until arr.size():
	//			year = abs(arr[i])
	//			while year == abs(arr[i]):
	//				if arr[i] > 0:
	//					alive += 1
	//				else if arr[i] < 0:
	//					alive -= 1
	//				i++
	//			if max_alive < alive:
	//				max_alive = alive
	//		ret max_alive
	//
	// Solution no 3 (Can we optimize further solve it in linear time?):
	//
	//	Time Complexity: O(num_people + num_people + (max_h - max_l))
	//					 O(2 * num_people + (max_h - max_l))
	//					 O(num_people + (max_h - max_l))
	//	most_living(list):
	//		max_l = 0
	//		max_h = 0
	//		for i = 0 until list.count():
	//			if max_h < list.get(i).h:
	//				max_h = list.get(i).h
	//			if min_l < list.get(i).l:
	//				min_l = list.get(i).l
	//		map[max_h - max_l]
	//		for i = 0 until list.count():
	//			pair = list.get(i)
	//			map[pair.l - min_l] += 1
	//			map[pair.h - min_l] -= 1
	//		max_alive = 0
	//		for i = until map.size():
	//			if max_alive < map[i]:
	//				max_alive = map[i]
	//		ret max_alive
}

// Diving Board: You are building a diving board by placing a bunch of planks of wood end-to-end.
// There are two types of planks, one of length shorter and one of length longer. You must use
// exactly K planks of wood. Write a method to generate all possible lengths for the diving board
namespace Problem11
{
	std::queue<int>& get_max(int short_len, int large_len, std::size_t k)
	{
		if (k == 0)
		{
			static std::queue<int> queue;
			return queue;
		}
		else if(k == 1)
		{
			static std::queue<int> queue;
			queue.push(short_len);
			queue.push(large_len);
			return queue;
		}
		std::queue<int>& queue = get_max(short_len, large_len, k - 1);
		std::size_t count = queue.size();
		while (count > 0)
		{
			int len = queue.front();
			queue.pop();
			--count;
			queue.push(len + short_len);
			queue.push(len + large_len);
		}
		return queue;
	}

	std::map<int, int> get_union(std::queue<int>& queue)
	{
		std::map<int, int> map;
		while(!queue.empty())
		{
			int len = queue.front();
			queue.pop();
			map[len] = len;
		}
		return map;
	}

	void solution1()
	{
		int short_len = 3;
		int large_len = 4;
		std::size_t k = 5;
		std::cout << "Problem 11:\n\tSolution no 1: " << std::endl;
		std::cout << "\t\tInput: short_len = " << short_len << ", large_len = " << large_len << ", k = " << k << std::endl;
		std::queue<int>& queue = get_max(short_len, large_len, k);
		std::map<int, int> map = get_union(queue);
		std::cout << "\t\tTotal: " << map.size() << " Possible lengths" << std::endl;
		for(std::pair<int, int> pair : map)
		{
			std::cout << pair.first << ", ";
		}
		std::cout << std::endl;
	}

	void solution2()
	{
		int short_len = 3;
		int large_len = 4;
		std::size_t k = 5;
		std::cout << "Problem 11:\n\tSolution no 2: " << std::endl;
		std::cout << "\t\tInput: short_len = " << short_len << ", large_len = " << large_len << ", k = " << k << std::endl;
		for(std::size_t i = 0; i <= k; i++)
		{
			long int len = i * (short_len - large_len) + k * large_len;
			std::cout << len << ", ";
		}
		std::cout << std::endl;
	}

	void problem11()
	{
		// Solution no 1:
		//	Time Complexity: O(1 + 2 + 2^2 + ... + 2^(k - 1))
		//					 O((2^(k) - 1) / (2 - 1))
		//					 O(2^k - 1)
		//					 O(2^k)
		//	_get_max(short, large, k):
		//		if k == 0:
		//			queue
		//			ret queue
		//		elif k == 1:
		//			queue
		//			queue.enque(short)
		//			queue.enque(large)
		//			ret queue
		//		queue = _get_max(k - 1)
		//		count = queue.count()
		//		while count > 0:
		//			len = queue.deque()
		//			--count
		//			queue.enque(len + short)
		//			queue.enque(len + large)
		//		ret queue
		// 	get_max(short, large, k):
		//		queue = _get_max(short, large, k)
		//		map = { }
		//		while !queue.empty():
		//			len = queue.deque()
		//			map[len] = len
		//		for pair in map:
		//			print(pair.first)
		//
		// Solution no 2:
		// Obervation:
		//	n = shorter planks
		//	m = larger planks
		//	n + m = k number of any planks
		//	n * short_len + m * large_len = len
		//	n * short_len + (k - n) * large_len = len
		//	n * (short_len - large_len) + k * large_len = len
		//
		//	Time Complexity: O(k)
		// 	get_max(short, large, k):
		//		for i = 0 until equals k:
		//			print(i * (short_len - large_len) + k * large_len)

		solution1();
		solution2();
	}
}

int main()
{
	problem1<int>();
	problem1<float>();
	problem2();
	problem3();
	problem4();
	problem5();
	problem6();
	problem7<int>();
	problem7<float>();
	problem8();
	problem9();
	problem10();
	Problem11::problem11();
	return 0;
}
