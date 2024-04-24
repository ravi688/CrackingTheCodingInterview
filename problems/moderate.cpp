#include <iostream>
#include <cstring> // strlen
#include <ctype.h> // tolower
#include <stdint.h> // uint32_t


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
	sprintf(str, "%llu", n);
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
	return 0;
}
