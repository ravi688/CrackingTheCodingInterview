
class binary_node:

	def __init__(self, data = None):
		self.left = None
		self.right = None
		self.data = data

	def set_data(self, data):
		self.data = data

	def create_left(self, data = None):
		self.left = binary_node(data)
		return self.left

	def create_right(self, data = None):
		self.right = binary_node(data)
		return self.right

	def get_sum(self):
		sum = 0
		if not self.data is None:
			sum += self.data
		if not self.left is None:
			sum += self.left.get_sum()
		if not self.right is None:
			sum += self.right.get_sum()
		return sum


class binary_tree:

	def __init__(self):
		self.root = binary_node(None)

	def get_root(self):
		return self.root

	def get_sum(self):
		return self.root.get_sum()

def build_binary_tree(tree):
	n0 = tree.get_root()
	n0.set_data(-1)

	n1 = n0.create_left(2)
	n2 = n0.create_right(4)

	n3 = n1.create_left(5)
	n4 = n1.create_right(6)
	n5 = n2.create_left(67)
	n6 = n2.create_right(100)

def main():
	tree = binary_tree()
	build_binary_tree(tree)
	print('Sum: {0}'.format(tree.get_sum()))

main()