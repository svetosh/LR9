#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

template<typename t>
class AVL {

	//узел дерева
	struct Node
	{
		t key;
		unsigned char height;
		Node* left;
		Node* right;
		Node(t k)
		{
			key = k;
			left = right = nullptr;
			height = 1;
		}
	};

	Node* root; //корень дерева

	//получаем высоту дерева
	unsigned char height(Node* p)
	{
		return p ? p->height : 0;
	}

	//коэфф балансировки
	int bfactor(Node* p)
	{
		return height(p->right) - height(p->left);
	}

	//вычисляем высоту дерева
	void fixheight(Node* p)
	{
		unsigned char hl = height(p->left);
		unsigned char hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	//правый поворот вокруг p
	Node* rotateright(Node* p)
	{
		Node* q = p->left;
		p->left = q->right;
		q->right = p;
		fixheight(p);
		fixheight(q);
		return q;
	}

	//левый поворот вокруг q
	Node* rotateleft(Node* q)
	{
		Node* p = q->right;
		q->right = p->left;
		p->left = q;
		fixheight(q);
		fixheight(p);
		return p;
	}

	//балансировка узла p
	Node* balance(Node* p)
	{
		fixheight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->right) < 0)
				p->right = rotateright(p->right);
			return rotateleft(p);
		}
		if (bfactor(p) == -2) {
			if (bfactor(p->left) > 0)
				p->left = rotateleft(p->left);
			return rotateright(p);
		}
		return p; // балансировка не нужна
	}

	// вставка элемента в дерево
	Node* add(Node* p, t k)
	{
		if (!p)
			return new Node(k);
		if (k < p->key)
			p->left = add(p->left, k);
		else if (k > p->key)
			p->right = add(p->right, k);
		return balance(p);
	}

	//поиск узла с минимальным ключом в дереве p 
	Node* findmin(Node* p)
	{
		return p->left ? findmin(p->left) : p;
	}

	//удаление узла с минимальным ключом из дерева p
	Node* removemin(Node* p)
	{
		if (p->left == 0)
			return p->right;
		p->left = removemin(p->left);
		return balance(p);
	}

	// удаление элемента из дерева
	Node* remove(Node* p, t k)
	{
		if (!p) return 0;
		if (k < p->key)
			p->left = remove(p->left, k);
		else if (k > p->key)
			p->right = remove(p->right, k);
		else //  k == p->key 
		{
			Node* q = p->left;
			Node* r = p->right;
			delete p;
			if (!r) return q;
			Node* min = findmin(r);
			min->right = removemin(r);
			min->left = q;
			return balance(min);
		}
		return balance(p); //балансируем при выходе из рекурсии
	}

	//асимметричный вывод дерева
	void print(Node* root, int lvl = 0)
	{
		if (root)
		{
			print(root->right, lvl + 1);
			for (int i = 0; i < lvl; i++)
			{
				cout << "   ";
			}
			cout << root->key << endl;
			print(root->left, lvl + 1);
		}
	}

	//очистка дерева
	void clear(Node*& root)
	{
		if (root)
		{
			clear(root->left);
			clear(root->right);
			delete[]root;
			root = nullptr;
		}
	}

	//открытые члены
public:

	//конструктор
	AVL()
	{
		root = nullptr;
	}

	//деструктор
	~AVL()
	{
		clear(root);
	}

	//добавление элемента
	void add(t elem) 
	{
		root = add(root, elem);
	}

	//удаление элемента
	void remove(t elem) 
	{
		root = remove(root, elem);
	}

	//печать дерева
	void print()
	{
		print(root);
	}
};

//элементы пар
struct pr
{
	int f, s;

	pr() :f(0), s(0) {}

	pr(int f, int s)
	{
		this->f = f;
		this->s = s;
	}
};

//для сравнения
bool operator>(const pr& a, const pr& b)
{
	if (a.f == b.f)
	{
		return a.s > b.s;
	}
	return a.f > b.f;
}

bool operator<(const pr& a, const pr& b)
{
	if (a.f == b.f)
	{
		return a.s < b.s;
	}
	return a.f < b.f;
}

bool operator==(const pr& a, const pr& b)
{
	return a.f == b.f && a.s == b.s;
}

//для вывода в поток
ostream& operator<<(ostream& os, pr p)
{
	os << p.f << "; " << p.s << endl;
	return os;
}

//тест авл дерева
int main()
{
	srand(time(NULL));
	int n = 7;
	int* m = new int[n];
	pr* mm = new pr[n];
	for (int i = 0; i < n; i++)
	{
		m[i] = rand() % 100 + 1;
		mm[i] = pr(rand() % 100 + 1, rand() % 100);
	}

	cout << "First tree:" << endl;
	AVL<int> a;
	for (int i = 0; i < n; i++)
	{
		a.add(m[i]);
	}
	a.print();

	cout << "Remove from first tree elements: ";
	for (int i = 2; i < 4; i++)
	{
		cout << m[i] << " ";
		a.remove(m[i]);
	}
	cout << endl;
	a.print();

	cout << "\n-------------------------------------------" << endl;
	cout << "Second tree:" << endl;
	AVL<pr> p;
	for (int i = 0; i < n; i++)
	{
		p.add(mm[i]);
	}
	p.print();

	cout << "Remove from second tree elements: " << endl;
	for (int i = 2; i < 4; i++)
	{
		cout << mm[i];
		p.remove(mm[i]);
	}
	cout << endl << endl;
	p.print();

	
	return 0;
}