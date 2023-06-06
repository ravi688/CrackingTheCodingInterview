
def printArray(array):
	for val in array:
		print('{0} '.format(val), end ="")
	print('')
	return

def swap(v1, v2):
	return v2, v1

def reverse(array):
	length = len(array)
	halfLength = length >> 1
	for i in range(halfLength):
		array[i], array[length - i - 1] = swap(array[i], array[length - i - 1])
	return

def main():
	array = [ 5, 6, 7, 100, -10, 223, -34 ]
	print('Original Array')
	printArray(array)
	reverse(array)
	print('Reversed Array')
	printArray(array)

main()
