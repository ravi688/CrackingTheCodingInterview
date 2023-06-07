

#include <optional> // std::optional

#include <stdint.h>
typedef uint64_t size_t;

template<typename SatelliteDataType>
class BinaryNode;

template<typename SatelliteDataType>
class BinaryNode
{
private:
	std::optional<SatelliteDataType> satellite_data;
	BinaryNode<SatelliteDataType>* left;
	BinaryNode<SatelliteDataType>* right;

public:

	BinaryNode<SatelliteDataType>* createLeft(const SatelliteDataType& data)
	{
		left = new BinaryNode<SatelliteDataType>();
		left->setSatelliteData(data);
		return left;
	}
	BinaryNode<SatelliteDataType>* createLeft()
	{
		left = new BinaryNode<SatelliteDataType>();
		return left;
	}
	BinaryNode<SatelliteDataType>* createRight(const SatelliteDataType& data)
	{
		right = new BinaryNode<SatelliteDataType>();
		right->setSatelliteData(data);
		return right;
	}
	BinaryNode<SatelliteDataType>* createRight()
	{
		right = new BinaryNode<SatelliteDataType>();
		return right;
	}

	void setSatelliteData(const SatelliteDataType& data) { satellite_data = data; }
	std::optional<SatelliteDataType>& getSatelliteData() const { return satellite_data; }

	void* operator new(size_t size)
	{
		auto ptr = reinterpret_cast<BinaryNode<SatelliteDataType>*>(::operator new(size));
		ptr->left = nullptr;
		ptr->right = nullptr;
		ptr->satellite_data = { };
		return ptr;
	}

	void operator delete(void* ptr)
	{
		BinaryNode<SatelliteDataType>* node = reinterpret_cast<BinaryNode<SatelliteDataType>*>(ptr);
		if(node->left != nullptr)
			BinaryNode<SatelliteDataType>::operator delete(node->left);
		if(node->right != nullptr)
			BinaryNode<SatelliteDataType>::operator delete(node->right);
		::operator delete(ptr);
	}

	SatelliteDataType getSum() const
	{
		SatelliteDataType sum = 0;
		if(satellite_data)
			sum += satellite_data.value();
		return sum + ((left != nullptr) ? left->getSum() : 0)  + ((right != nullptr) ? right->getSum() : 0);
	}
};

template<typename SatelliteDataType>
class BinaryTree
{
private:
	BinaryNode<SatelliteDataType>* root;

public:
	BinaryTree() : root(new BinaryNode<SatelliteDataType>()) { }
	~BinaryTree()
	{
		delete root;
	}

	BinaryNode<SatelliteDataType>* getRoot() { return root; }
	SatelliteDataType getSum()
	{
		return getRoot()->getSum();
	}
};
