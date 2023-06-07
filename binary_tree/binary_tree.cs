
namespace DataStructures
{
	abstract class NodeValue
	{
		public abstract NodeValue Add(NodeValue rhs);
		public abstract NodeValue Accumulate(NodeValue rhs);
	}
	
	class NodeValueInt : NodeValue
	{
		public int value { get; set; }
	
		public static NodeValueInt zero { get { return new NodeValueInt(0); } }
	
		public NodeValueInt() { value = 0; }
		public NodeValueInt(int value) { this.value = value; }
	
		public override NodeValue Add(NodeValue rhs)
		{
			return new NodeValueInt((rhs as NodeValueInt).value + this.value);
		}
	
		public override NodeValue Accumulate(NodeValue rhs)
		{
			this.value += (rhs as NodeValueInt).value;
			return this;
		}
	}
	
	
	class BinaryNode<T> where T : NodeValue, new()
	{
		private T nodeValue;
		private static T zeroValue;
		private BinaryNode<T> left;
		private BinaryNode<T> right;
	
		public BinaryNode(T data = null, T zeroValue = null)
		{
			nodeValue = data;
			left = null;
			right = null;
			BinaryNode<T>.zeroValue = zeroValue;
		}
	
		public void SetSatelliteData(T data)
		{
			nodeValue = data;
		}
	
		public BinaryNode<T> CreateLeft(T data = null)
		{
			left = new BinaryNode<T>(data);
			return left;
		}
	
		public BinaryNode<T> CreateRight(T data = null)
		{
			right = new BinaryNode<T>(data);
			return right;
		}
	
		public T GetSum()
		{
			T sum = new T();
			if(nodeValue != null)
				sum.Accumulate(nodeValue);
			if(left != null)
				sum.Accumulate(left.GetSum());
			if(right != null)
				sum.Accumulate(right.GetSum());
			return sum;
		}
	}
	
	class BinaryTree<T> where T : NodeValue, new()
	{
		public BinaryNode<T> root { get; private set; }
	
		public BinaryTree(T zeroValue)
		{
			root = new BinaryNode<T>(null, zeroValue);
		}
	
		public T GetSum()
		{
			return root.GetSum();
		}
	}

}