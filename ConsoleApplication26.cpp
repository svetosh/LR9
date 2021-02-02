#include <iostream>
#include <ctime>

using namespace std;

template<typename t>
class AVL {
	struct Node {
		t data;
		Node* left;
		Node* right;
		Node(t data) {
			this->data = data;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root; 

public:
	AVL() {
		root = nullptr;
	}

	void Add(t data) {
		Node* k = new Node(data);
		if (!root) {
			root = k;
		}
		else {
			root = Insert(root, k);
		}
	}

	void Delete(t key) {
		root = Delete(root, key);
	}

	void Find(t key) {
		if (Find(key, root)->data == key) {
			cout << "Tree include " << key << "!" << endl;
		}
		else {
			cout << "Not find element!" << endl;
		}
	}

	void PrintTree() {
		if (!root) {
			cout << "Tree is empty!" << endl;
			return;
		}
		PrintTree(root);
	}

	~AVL() {
		clear(root);
	}

private:

	Node* Insert(Node* current, Node* n) 
	{
		if (!current) 
		{
			current = n;
			return current;
		}
		else if (n->data < current->data) 
		{
			current->left = Insert(current->left, n);
		}
		else if (n->data > current->data) 
		{
			current->right = Insert(current->right, n);
		}
		return balance(current);
	}

	Node* balance(Node* current)
	{
		int b_factor = balance_factor(current);
		if (b_factor > 1) 
		{
			if (balance_factor(current->left) > 0) 
			{
				current = RotateLL(current);
			}
			else 
			{
				current = RotateLR(current);
			}
		}
		else if (b_factor < -1) 
		{
			if (balance_factor(current->right) > 0) 
			{
				current = RotateRL(current);
			}
			else 
			{
				current = RotateRR(current);
			}
		}
		return current;
	}

	Node* findmin(Node* p)
	{
		if (p->left)
		{
			return findmin(p->left);
		}
		else
		{
			return p;
		}
	}

	Node* removemin(Node* p) 
	{
		if (!p->left)
			return p->right; 
		p->left = removemin(p->left);
		return balance(p);
	}

	Node* Delete(Node* p, t key)
	
	{
		if (!p)
		{
			return nullptr;
		}
		if (key < p->data)
		{
			p->left = Delete(p->left, key);
		}
		else if (key > p->data)
		{
			p->right = Delete(p->right, key);
		}
		else
		{
			Node* q = p->left;
			Node* r = p->right;
			delete p;
			if (!r)
			{
				return q;
			}
			Node* min = findmin(r);
			min->right = removemin(r);
			min->left = q;
			return balance(min);
		}
		return balance(p);
	}

	void clear(Node*& root)
	{
		if (root) 
		{
			clear(root->left);
			clear(root->right);
			delete root;
			root = nullptr;
		}
	}

	Node* Find(t key, Node* current) 
	{
		if (key < current->data) 
		{
			if (key == current->data) 
			{
				return current;
			}
			else
			{
				return Find(key, current->left);
			}
		}
		else 
		{
			if (key == current->data) 
			{
				return current;
			}
			else
			{
				return Find(key, current->right);
			}
		}
	}

	void PrintTree(Node* current, int lvl = 0) 
	{
		if (current) 
		{
			PrintTree(current->right, lvl + 1);
			for (int i = 0; i < lvl; i++)
			{
				cout << "  ";
			}
			cout << current->data << endl;
			PrintTree(current->left, lvl + 1);
		}
	}

	int max(int l, int r) 
	{
		return l > r ? l : r;
	}

	int getHeight(Node* current) 
	{
		int height = 0;
		if (current)
		{
			int l = getHeight(current->left);
			int r = getHeight(current->right);
			int m = max(l, r);
			height = m + 1;
		}
		return height;
	}

	int balance_factor(Node* current) 
	{
		int l = getHeight(current->left);
		int r = getHeight(current->right);
		int b_factor = l - r;
		return b_factor;
	}

	Node* RotateRR(Node* parent)
	{
		Node* pivot = parent->right;
		parent->right = pivot->left;
		pivot->left = parent;
		return pivot;
	}

	Node* RotateLL(Node* parent)
	{
		Node* pivot = parent->left;
		parent->left = pivot->right;
		pivot->right = parent;
		return pivot;
	}

	Node* RotateLR(Node* parent)
	{
		Node* pivot = parent->left;
		parent->left = RotateRR(pivot);
		return RotateLL(parent);
	}

	Node* RotateRL(Node* parent)
	{
		Node* pivot = parent->right;
		parent->right = RotateLL(pivot);
		return RotateRR(parent);
	}
};

struct pr {
	int f, s;
	pr() :f(0), s(0) {}

	pr(int f, int s) 
	{
		this->f = f; 
		this->s = s;
	}
};

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

ostream& operator<<(ostream& os, pr p)
{
	os << p.f << " " << p.s << endl;
	return os;
}

int main() {
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
		a.Add(m[i]);
	}
	a.PrintTree();

	cout << "Remove from first tree elements: ";
	for (int i = 2; i < 4; i++) 
	{
		cout << m[i] << " ";
		a.Delete(m[i]);
	}
	cout << endl;
	a.PrintTree();

	cout << endl << "-------------------------------------------" << endl;
	cout << "Second tree:" << endl;
	AVL<pr> p;
	for (int i = 0; i < n; i++) 
	{
		p.Add(mm[i]);
	}
	p.PrintTree();

	cout << "Remove from second tree elements: " << endl;
	for (int i = 2; i < 4; i++) 
	{
		cout << mm[i];
		p.Delete(mm[i]);
	}
	cout << endl << endl;
	p.PrintTree();
	return 0;
}
