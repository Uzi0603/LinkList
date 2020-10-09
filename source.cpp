#include<iostream>
#include<sstream>
#include<string>
using namespace std;

class illegalvalue//自定义的异常类
{
	public:
		illegalvalue(string s = "value is not legal") :message(s) {};
		void output() { cout << message; }
	private:
		string message;
};

template<class T>
struct chainNode
{
	T data;
	chainNode<T>* nextNode;
	
	chainNode(){}
	chainNode(const T& theElement) { this->data = theElement; }
	chainNode(const T& theElement, chainNode<T>* next)
	{
		this->data = theElement;
		this->nextNode = next;
	}
};

template<class T>
class chain
{
	public:
		//构造函数、析构函数
		chain(int initialCapacity = 10);
		chain(const chain<T>&);//复制构造函数
		~chain();
		//方法
		void insert(int theIndex, const T& theElement);//插入
		int erase(const T& theElement);//删除(不存在返回-1)
		//void reverse(chain<T>& list);//倒置
		void reverse();
		int indexOf(const T& theElement) const;//查询索引
		long calculate_xor();//计算异或和
		void combine(chain<T>& a, chain<T>& b);//合并
		//辅助方法
		void creatList(int size = 5);//有序创建一个链表
		void clear();//清空
		void push_back(const T& element);
		void insertFirstNode(const T& theElement);//在原头节点前插入新的头节点
		void output();//打印
		int sizeOf() const {return size; }//查询链表长度
		bool empty() const { return size == 0; }//判断链表是否为空
		//向前迭代器
		class iterator;
		iterator start() { return iterator(firstNode); }
		iterator end() { return iterator(NULL); }

		class iterator
		{
			public:
				//构造函数
				iterator(chainNode<T>* theNode = NULL) { node = theNode; }
				//解引用操作符
				T& operator*() const { return node->data; }
				T* operator->() const { return &node->data; }
				//迭代器加法
				iterator& operator++() //前加
				{ 
					node = node->nextNode; 
					return *this;
				}
				iterator& operator++(int)//后加
				{
					iterator old = *this;
					node = node->nextNode;
					return old;
				}
				//判断是否相等
				bool operator==(const iterator right) const { return node == right.node; }
				bool operator!=(const iterator right) const { return node != right.node; }
			protected:
				chainNode<T>* node;
		};

	protected:
		chainNode<T>* firstNode;
		int size;
};

template<class T>
chain<T>::chain(int initialCapacity)
{
	if (initialCapacity < 1)
	{
		ostringstream s;
		s << "initial capacity = " << initialCapacity << " must be > 0";
		throw illegalvalue(s.str());
	}
	firstNode = NULL;
	size = 0;
}

template<class T>
chain<T>::~chain()
{
	while (firstNode != NULL)
	{
		chainNode<T>* nextnode = firstNode->nextNode;
		delete firstNode;
		firstNode = nextnode;
	}
}

template<class T>
chain<T>::chain(const chain<T>& theList)
{
	size = theList.size;
	if (size == 0)
	{
		firstNode = NULL;
		return;
	}
	chainNode<T>* sourceNode = theList.firstNode;
	firstNode = new chainNode<T>(sourceNode->data);
	sourceNode = sourceNode->nextNode;
	chainNode<T>* targetNode = firstNode;
	while (sourceNode != NULL)
	{
		targetNode->nextNode = new chainNode<T>(sourceNode->data);
		targetNode = targetNode->nextNode;
		sourceNode = sourceNode->nextNode;
	}
	targetNode->nextNode = NULL;
}

template<class T>
void chain<T>::clear()
{
	while (firstNode != NULL)
	{
		chainNode<T>* nextnode = firstNode->nextNode;
		delete firstNode;
		firstNode = nextnode;
	}
	size = 0;
}

template<class T> //冒泡排序
void bubbleSort(T arr[], int len)
{
	T term;
	bool SWAP = true;
	for (int i = 0; i < len - 1 && SWAP; i++)
	{
		SWAP = false;
		for (int j = 0; j < len - 1 - i; j++)
			if (arr[j] > arr[j + 1])
			{
				term = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = term;
				SWAP = true;
			}
	}
}

template<class T>
void chain<T>::creatList(int n)
{
	T* element = new T[n];
	for (int i = 0; i < n; i++)
		cin >> element[i];
	//bubbleSort(element, n);
	for (int i = 0; i < n; i++)
		(*this).insert(i, element[i]);
	delete []element;
}

template<class T>
void chain<T>::insert(int theIndex, const T& theElement)
{
	if (theIndex<0 || theIndex>size)
	{
		ostringstream s;
		s << "the index = " << theIndex << " size = " << size;
		throw illegalvalue(s.str());
	}
	
	if (theIndex == 0)
		firstNode = new chainNode<T>(theElement, firstNode);
	else
	{
		chainNode<T>* lastNode = firstNode;//寻找前驱
		for (int i = 0; i < theIndex - 1; i++)
			lastNode = lastNode->nextNode;

		lastNode->nextNode = new chainNode<T>(theElement, lastNode->nextNode);
		//先执行new，再赋值
	}
	size++;
}

template<class T>
int chain<T>::indexOf(const T& theElement) const
{
	chainNode<T>* currentNode = firstNode;
	int index = 0;
	while (currentNode != NULL && currentNode->data != theElement)
	{
		currentNode = currentNode->nextNode;
		index++;
	}
	if (currentNode == NULL)
		return -1;
	else
		return index;
}

template<class T>
int chain<T>::erase(const T& theElement)
{
	int delete_index = indexOf(theElement);

	if (delete_index == -1)
		return -1;

	chainNode<T>* deleteNode;
	if (delete_index == 0)
	{
		deleteNode = firstNode;
		firstNode = firstNode->nextNode;
	}
	else
	{
		chainNode<T>* lastNode = firstNode;//寻找前驱
		for (int i = 0; i < delete_index - 1; i++)
			lastNode = lastNode->nextNode;
		deleteNode = lastNode->nextNode;
		lastNode->nextNode = lastNode->nextNode->nextNode;
	}
	delete deleteNode;//删除指针并释放空间
	size--;
	return 0;
}

template<class T>
void chain<T>::insertFirstNode(const T& theElement)
{
	firstNode = new chainNode<T>(theElement, firstNode);
	size++;
}

template<class T>
void chain<T>::reverse()
{
	chainNode<T>* currentNode, * lastNode, * nextNode;
	lastNode = firstNode;//前驱
	currentNode = firstNode->nextNode;//当前
	firstNode->nextNode = NULL;//首指针变末指针
	while (currentNode != NULL)
	{
		nextNode = currentNode->nextNode;//保存后继指针防止丢失
		currentNode->nextNode = lastNode;//指针反转
		//后移
		lastNode = currentNode;
		currentNode = nextNode;
	}
	//此时currenNode=NULL
	firstNode = lastNode;
}

template<class T>
void chain<T>::output()
{
	for (chainNode<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->nextNode)
		cout << currentNode->data << " ";
}

template<class T>
void chain<T>::push_back(const T& element)
{
	chainNode<T>* newNode = new chainNode<T>(element, NULL);
	if (firstNode == NULL)
		firstNode = newNode;
	else
	{
		chainNode<T>* lastnode = firstNode;
		for (int i = 0; i < size-1; i++)
			lastnode = lastnode->nextNode;
		lastnode->nextNode = newNode;
	}
	size++;
}

template<class T>
void chain<T>::combine(chain<T>& a, chain<T>& b)
{
	chain<T>::iterator astart = a.start();
	chain<T>::iterator aend = a.end();
	chain<T>::iterator bstart = b.start();
	chain<T>::iterator bend = b.end();

	while (astart != aend && bstart != bend)
	{
		if (*astart < *bstart)
		{
			(*this).push_back(*astart);
			astart++;
		}
		else if (*astart > * bstart)
		{
			(*this).push_back(*bstart);
			bstart++;
		}
		else
		{
			(*this).push_back(*astart);
			(*this).push_back(*bstart);
			astart++;
			bstart++;
		}
	}
	
	while (astart != aend)
	{
		(*this).push_back(*astart);
		astart++;
	}
	while (bstart != bend)
	{
		(*this).push_back(*bstart);
		bstart++;
	}

}

template<class T>
long chain<T>::calculate_xor()
{
	long sum_xor = 0;
	long temp;//中间变量
	chain<T>::iterator istart = (*this).start();
	chain<T>::iterator iend = (*this).end();
	for (int i = 0; istart != iend; i++, istart++)
	{
		temp = i ^ (*istart);
		sum_xor += temp;
	}
	return sum_xor;
}

int main()
{
	//加入异常处理
	try
	{
		int n, q;
		cin >> n >> q;

		chain<int> element;
		element.creatList(n);

		int result_index = 0;
		long* result = new long[q];
		int decision, idx, val, temp;
		//temp定义在switch里会被case跳过
		for (int i = 0; i < q; i++)
		{
			cin >> decision;
			switch (decision)
			{
			case(1):
				cin >> idx >> val;
				element.insert(idx, val);
				break;
			case(2):
				cin >> val;
				temp = element.erase(val);
				if (temp == -1)
				{
					result[result_index] = -1;
					result_index++;
				}
				break;
			case(3):
				element.reverse();
				break;
			case(4):
				cin >> val;
				result[result_index] = element.indexOf(val);
				result_index++;
				break;
			case(5):
				result[result_index] = element.calculate_xor();
				result_index++;
			default:
				break;
			}
		}
		for (int j = 0; j < result_index; j++)
			cout << result[j] << endl;
	}
	//处理异常
	catch (illegalvalue message)
	{
		message.output();
		exit(1);
	}
	return 0;
}
/*
int main()
{
	int n,m;
	cin >> n>>m;
	
	chain<int> a,b,list;
	a.creatList(n);
	b.creatList(m);

	
	a.reverse();
	a.output();
	//list.combine(a, b);

	//cout << a.calculate_xor() << endl << b.calculate_xor() << endl << list.calculate_xor();
}
*/