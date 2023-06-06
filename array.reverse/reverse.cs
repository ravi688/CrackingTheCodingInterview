
using System;

class Reverse
{
	private static void PrintArray<T>(T[] array)
	{
		int length = array.Length;
		for(int i = 0; i < length; i++)
			Console.Write($"{array[i]} ");
		Console.WriteLine("");
	}

	private static void Swap<T>(ref T v1, ref T v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;	
	}

	private static void ReverseArray<T>(T[] array)
	{
		int length = array.Length;
		int halfLength = length >> 1;
		for(int i = 0; i < halfLength; i++)
			Swap<T>(ref array[i], ref array[length - i - 1]);
	}

	private static void Main(string[] args)
	{
		int[] array = new int[]{ 10, 203, 40, 50, 60, 50, 10, -30 };
		Console.WriteLine("Original Array: ");
		PrintArray<int>(array);
		ReverseArray<int>(array);
		Console.WriteLine("Reversed Array: ");
		PrintArray<int>(array);
	}
}
