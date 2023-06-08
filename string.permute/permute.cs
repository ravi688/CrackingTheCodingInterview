
using System;

struct ForwardStringSlice
{
	/* private fields */
	private char[] m_str;
	private int m_begin;

	/* public properties  */
	public char[] original { get { return m_str; } }
	public int length { get { return m_str.Length - m_begin; }}

	/* constructors */
	public ForwardStringSlice(string str, int begin = 0)
	{
		// we can't modify string's as they are immutable (readonly) so we would need to create character array
		m_str = str.ToCharArray();
		m_begin = begin;
	}

	public ForwardStringSlice(char[] str, int begin = 0)
	{
		m_str = str;
		m_begin = begin;
	}

	/* operator overloads */
	public ref char this[int index] 
	{ 
		get { return ref m_str[m_begin + index]; }
	}

	public ForwardStringSlice getSlice(int begin)
	{
		return new ForwardStringSlice(m_str, m_begin + begin);
	}
}

class StringPermutation 
{
	private static void Swap<T>(ref T v1, ref T v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;
	}

	private static void Permute(ForwardStringSlice slice)
	{
		int length = slice.length;
		if(length == 0)
		{
			Console.WriteLine(slice.original);
			return;
		}

		for(int i = 0; i < length; i++)
		{
			Swap<char>(ref slice[0], ref slice[i]);
			Permute(slice.getSlice(1));
			Swap<char>(ref slice[0], ref slice[i]);
		}
	}

	private static void Main(string[] args)
	{
		string str = "ABCD";
		Permute(new ForwardStringSlice(str));
	}
}