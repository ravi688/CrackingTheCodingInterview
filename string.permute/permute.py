
def permute(array, offset, length):
	if length == 0:
		print(array)
		return
	for i in range(length):
		array[offset], array[offset + i] = array[offset + i], array[offset]
		permute(array, offset + 1, length - 1)
		array[offset], array[offset + i] = array[offset + i], array[offset]


def main():
	str = 'ABCD'
	array = list(str)
	permute(array, 0, len(array))


main()