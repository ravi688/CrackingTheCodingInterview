#include <iostream> // for std::cout
#include <string_view> // for std::string_view
#include <cstddef> // for std::byte
#include <vector> // for std::vector
#include <cstdint> // for std::uint32_t
#include <queue> // for std::priority_queue<>
#include <unordered_map> // for std::unordered_map<>
#include <exception> // for std::exception

struct HuffmanCodeNode
{
	uint32_t frequency { };
	HuffmanCodeNode* left { };
	HuffmanCodeNode* right { };
	bool isLeaf() const noexcept { return !left && !right; }
};

// Inheriting from a struct is always public by default
struct HuffmanCodeLeafNode : HuffmanCodeNode
{
	char ch;
};

struct HuffmanCodeNodeCompare
{
	bool operator()(HuffmanCodeNode* const &lhs, HuffmanCodeNode* const &rhs) noexcept
	{
		return lhs->frequency > rhs->frequency;
	}
};

class BitList;

class BitReference
{
	friend class BitIterator;
	friend class BitList;
	private:
		BitList& m_bitList;
		std::size_t m_bitIndex;
		BitReference(BitList& bitList, std::size_t bitIndex) noexcept : m_bitList(bitList), m_bitIndex(bitIndex) { }

	public:
		BitReference& operator=(bool value);
		operator bool() const;
};


class ConstBitReference
{
	friend class ConstBitIterator;
	friend class BitList;
	private:
		const BitList& m_bitList;
		std::size_t m_bitIndex;
		ConstBitReference(const BitList& bitList, std::size_t bitIndex) noexcept : m_bitList(bitList), m_bitIndex(bitIndex) { }

	public:
		operator bool() const;
};

class BitIterator
{
	friend class BitList;
	private:
		BitList& m_bitList;
		std::size_t m_bitIndex { };
		BitIterator(BitList& bitList, std::size_t index) noexcept : m_bitList(bitList), m_bitIndex(index) { }
	public:
		BitIterator(BitList& bitList) noexcept : m_bitList(bitList) { }

		// Equality operator
		bool operator==(const BitIterator& iterator) const;

		// Post increment
		BitIterator operator++(int);
		// Pre increment
		BitIterator operator++();
		// Post decrement
		BitIterator operator--(int);
		// Pre decrement
		BitIterator operator--();

		BitReference operator*();
};


class ConstBitIterator
{
	friend class BitList;
	private:
		const BitList& m_bitList;
		std::size_t m_bitIndex { };
		ConstBitIterator(const BitList& bitList, std::size_t index) noexcept : m_bitList(bitList), m_bitIndex(index) { }
	public:
		ConstBitIterator(const BitList& bitList) noexcept : m_bitList(bitList) { }

		// Equality operator
		bool operator==(const ConstBitIterator& iterator) const;

		// Post increment
		ConstBitIterator operator++(int);
		// Pre increment
		ConstBitIterator operator++();
		// Post decrement
		ConstBitIterator operator--(int);
		// Pre decrement
		ConstBitIterator operator--();

		ConstBitReference operator*();
};

class BitList
{
	private:
		std::vector<std::byte> m_bytes;
		// Number of bits
		std::size_t m_size { };
	public:
		void pushBack(bool bit);
		void pushBack(const BitList& list);
		void popBack();
		void setAt(std::size_t index, bool value);
		bool getAt(std::size_t index) const;
		BitReference operator[](std::size_t bitIndex);
		BitIterator begin() { return { *this, 0 }; }
		BitIterator end() { return { *this, m_size }; }
		ConstBitIterator cbegin() const { return { *this, 0 }; }
		ConstBitIterator cend() const { return { *this, m_size }; }
		auto begin() const { return cbegin(); }
		auto end() const { return cend(); }
		std::size_t size() const noexcept { return m_size; }
		std::size_t numBytes() const noexcept { return m_bytes.size(); }
};

// ------------- IMPLEMENTATION of BitReference ------------
BitReference& BitReference::operator=(bool value)
{
	m_bitList.setAt(m_bitIndex, value);
	return *this;
}
BitReference::operator bool() const
{
	return m_bitList.getAt(m_bitIndex);
}
// --------------------------------------------------------


// ------------- IMPLEMENTATION of ConstBitReference ------------
ConstBitReference::operator bool() const
{
	return m_bitList.getAt(m_bitIndex);
}
// --------------------------------------------------------

// ------------- IMPLEMENTATION of BitIterator -------------
// Equality operator
bool BitIterator::operator==(const BitIterator& iterator) const
{
	return m_bitIndex == iterator.m_bitIndex;
}

// Post increment
BitIterator BitIterator::operator++(int)
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("BitIterator: Increment past the end() is not possible");
	BitIterator it { m_bitList, m_bitIndex };
	m_bitIndex +=1;
	return it;
}
// Pre increment
BitIterator BitIterator::operator++()
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("BitIterator: Increment past the end() is not possible");
	m_bitIndex +=1;
	return { m_bitList, m_bitIndex };
}
// Post decrement
BitIterator BitIterator::operator--(int)
{
	if(m_bitIndex == 0)
		throw std::out_of_range("BitIterator: Decrement past 0 index is not possible");
	BitIterator it { m_bitList, m_bitIndex };
	m_bitIndex -= 1;
	return it;
}
// Pre decrement
BitIterator BitIterator::operator--()
{
	if(m_bitIndex == 0)
		throw std::out_of_range("BitIterator: Decrement past 0 index is not possible");
	m_bitIndex -= 1;
	return { m_bitList, m_bitIndex };
}

BitReference BitIterator::operator*()
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("BitIterator: Deferencing end() is not possible");
	return { m_bitList, m_bitIndex };
}
// ----------------------------------------------------------

// ------------- IMPLEMENTATION of ConstBitIterator -------------
// Equality operator
bool ConstBitIterator::operator==(const ConstBitIterator& iterator) const
{
	return m_bitIndex == iterator.m_bitIndex;
}


// Post increment
ConstBitIterator ConstBitIterator::operator++(int)
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("BitIterator: Increment past the end() is not possible");
	ConstBitIterator it { m_bitList, m_bitIndex };
	m_bitIndex +=1;
	return it;
}
// Pre increment
ConstBitIterator ConstBitIterator::operator++()
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("BitIterator: Increment past the end() is not possible");
	m_bitIndex +=1;
	return { m_bitList, m_bitIndex };
}
// Post decrement
ConstBitIterator ConstBitIterator::operator--(int)
{
	if(m_bitIndex == 0)
		throw std::out_of_range("ConstBitIterator: Decrement past 0 index is not possible");
	ConstBitIterator it { m_bitList, m_bitIndex };
	m_bitIndex -= 1;
	return it;
}
// Pre decrement
ConstBitIterator ConstBitIterator::operator--()
{
	if(m_bitIndex == 0)
		throw std::out_of_range("ConstBitIterator: Decrement past 0 index is not possible");
	m_bitIndex -= 1;
	return { m_bitList, m_bitIndex };
}

ConstBitReference ConstBitIterator::operator*()
{
	if(m_bitIndex >= m_bitList.size())
		throw std::out_of_range("ConstBitIterator: Deferencing end() is not possible");
	return { m_bitList, m_bitIndex };
}
// ----------------------------------------------------------


// ------------- IMPLEMENTATION of BitList ------------------
void BitList::pushBack(bool bit)
{
	auto maxBits = m_bytes.size() * 8;
	// Check if more bits can be accomodated in the so-far created bytes
	// If not then create a new byte
	if(m_size == maxBits)
		m_bytes.push_back(std::byte { });
	std::byte& byteRef = m_bytes.back();
	auto localBitIndex = m_size % 8;
	byteRef |= (std::byte{bit} << localBitIndex);
	++m_size;
}
void BitList::pushBack(const BitList& list)
{
	// Optimize this
	for(auto bit : list)
		pushBack(static_cast<bool>(bit));
}
void BitList::popBack()
{
	if(m_size <= 0)
		throw std::out_of_range("BitList: Index is out of range, BitList is empty, can't pop back");
	setAt(m_size - 1, false);
	--m_size;
	// Check if number of bytes rquired is decreased, if yes, then remove the last byte from m_bytes
	auto reqBytes = ((m_size + 7) >> 3);
	if(reqBytes < m_bytes.size())
		m_bytes.pop_back();
}
void BitList::setAt(std::size_t index, bool value)
{
	if(index >= m_size)
		throw std::out_of_range("BitList: Index is out of range, can't set bit value");
	auto localBitIndex = index % 8;
	auto byteIndex = index >> 3;
	std::byte& byteRef = m_bytes[byteIndex];
	// Unset the old bit value
	byteRef &= ~(std::byte{1} << localBitIndex);
	// Set the new bit value
	byteRef |= (std::byte{value} << localBitIndex);
}
bool BitList::getAt(std::size_t index) const
{
	if(index >= m_size)
		throw std::out_of_range("BitList: Index is out of range, can't get bit value");
	auto localBitIndex = index % 8;
	auto byteIndex = index >> 3;
	std::byte byte = m_bytes[byteIndex];
	return std::to_integer<uint8_t>(byte & (std::byte{1} << localBitIndex));
}
// ------------------------------------------------------

static std::ostream& operator<<(std::ostream& stream, const BitList& bitList)
{
	for(auto bit : bitList)
	{
		stream << static_cast<bool>(bit);
	}
	return stream;
}

static void _generateCodeWords(const HuffmanCodeNode* node, std::unordered_map<char, BitList>& codewordMap, BitList& bufferBits)
{
	if(node->isLeaf())
	{
		const HuffmanCodeLeafNode* leafNode = static_cast<const HuffmanCodeLeafNode*>(node);
		codewordMap.insert({ leafNode->ch, bufferBits });
		return;
	}

	// Traverse the right sub-tree
	bufferBits.pushBack(true);
	_generateCodeWords(node->right, codewordMap, bufferBits);
	bufferBits.popBack();

	// Traverse the left sub-tree
	bufferBits.pushBack(false);
	_generateCodeWords(node->left, codewordMap, bufferBits);
	bufferBits.popBack();
}

using CodeWordTable = std::unordered_map<char, BitList>;

static void generateCodeWords(const HuffmanCodeNode* node, CodeWordTable& codewordMap)
{
	BitList bufferBits;
	bufferBits.pushBack(false);
	_generateCodeWords(node, codewordMap, bufferBits);
}

static void deleteHuffmanCodeTree(HuffmanCodeNode* node)
{
	if(node->isLeaf())
	{
		// WARN: If we don't cast the pointer to its original type (the type for which the memory block was allocated)
		// Then we will get valgrind memcheck error telling about mismatch sizes when calling new and delete.
		auto* leafNode = static_cast<HuffmanCodeLeafNode*>(node);
		delete leafNode;
		return;
	}
	deleteHuffmanCodeTree(node->right);
	deleteHuffmanCodeTree(node->left);
	delete node;
}

static std::pair<BitList, CodeWordTable>  compress(const std::string_view str) noexcept
{
	// Calculate frequency of each character in the string
	std::unordered_map<char, std::uint32_t> frequencyTable;
	for(const auto& ch : str)
		frequencyTable[ch] += 1;

	// Sort the (frequency, char) pair in increasing frequency order
	// i.e. The pair with lowest frequency is returned first when queried inside the priority queue
	std::priority_queue<HuffmanCodeNode*, std::vector<HuffmanCodeNode*>, HuffmanCodeNodeCompare> pqueue;
	for(const auto& pair : frequencyTable)
	{
		auto* node = new HuffmanCodeLeafNode();
		node->frequency = pair.second;
		node->left = nullptr;
		node->right = nullptr;
		node->ch = pair.first;
		pqueue.push(node);
	}

	// Build Frequency Sorted Binary Tree
	while(pqueue.size() > 1)
	{
		auto* node1 = pqueue.top(); pqueue.pop();
		auto* node2 = pqueue.top(); pqueue.pop();
		auto* node = new HuffmanCodeNode();
		node->frequency = node1->frequency + node2->frequency;
		node->left = node1;
		node->right = node2;
		pqueue.push(node);
	}

	auto* tree = pqueue.top();

	// Generate Code Words for each character
	CodeWordTable codewords;
	generateCodeWords(tree, codewords);

	// Free-up allocated memory
	deleteHuffmanCodeTree(tree);

	// Build compressed data
	BitList compressedData;
	for(auto& ch : str)
		compressedData.pushBack(codewords[ch]);

	// PERF: Here RVO won't kick in!
	return { std::move(compressedData), std::move(codewords) };
}

static std::string decompress(const BitList& bits, const std::unordered_map<char, BitList>& codewordTable) noexcept
{
	return { };
}


int main()
{
	std::string_view strData = "fsdaj;flksdjflaksdjf;lsdajkf;lsdjfks;dlfjsd;lfjasl;djf;lasdjf";
	std::cout << "String to compress: " << strData << "\n";
	auto [bits, codewordTable] = compress(strData);
	std::cout << "Original Size: " << strData.size() * 8 << " bits\n";
	std::cout << "Compressed Size (in memory): " << bits.numBytes() * 8 << " bits\n";
	std::cout << "Compressed Size (in theory): " << bits.size() << " bits\n";
	std::cout << "Compressed Bits: " << bits << std::endl;
	std::string strData2 = decompress(bits, codewordTable);
	std::cout << "Decompressed String: " << strData2 << std::endl;
	return 0;
}
