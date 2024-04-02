#include <iostream>
#include <cstring> // strlen
#include <ctype.h> // tolower


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

int main()
{
	problem1<int>();
	problem1<float>();
	problem2();
	return 0;
}
