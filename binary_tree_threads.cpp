#include <iostream>
#include <thread>
#include <functional>
#include <exception>
#include <string>
#include <mutex>
using namespace std;
class TreeException : public std::exception
{
	std::string _error;
public:
	TreeException(std::string error);
	char const* what();
};

template <class T, typename U = std::less<>>
class Tree
{
public:
	Tree() noexcept;
	Tree(const Tree<T, U>& cop);
	Tree(Tree<T, U>&& per) noexcept;

	void add(const T& val);
	void add(const Tree<T, U>& cop);
	bool find(const T& val) noexcept;
	Tree<T, U> subTree(const T& val);
	void deleteSubTree(const T& val);
	void deleteElem(const T& val);
	void clear() noexcept;

	template <class T1, typename U1>
	friend std::ostream& operator<< (std::ostream& os, const Tree<T1, U1>& tree);
	void lkp(std::ostream& os = std::cout, const char& sep = ' ') noexcept;
	void klp(std::ostream& os = std::cout, const char& sep = ' ') noexcept;
	void lpk(std::ostream& os = std::cout, const char& sep = ' ') noexcept;

	~Tree();

protected:
	struct Root
	{
		T _value;
		Root* _left;
		Root* _right;
		Root(const T& val);
	};

	typedef Tree<T, U>::Root* pRoot;

	U u;
	std::mutex mtx;
	pRoot _root;

	static pRoot recursive_copy(const pRoot& cop);
	void recursive_add(pRoot& root, const T& val);
	void recursive_tree_add(pRoot& root, const pRoot& adding);
	static void recursive_del(pRoot& root) noexcept;
	static void recursive_lkp(const pRoot& root, std::ostream& os, const char& sep) noexcept;
	static void recursive_lpk(const pRoot& root, std::ostream& os, const char& sep) noexcept;
	static void recursive_klp(const pRoot& root, std::ostream& os, const char& sep) noexcept;
	pRoot& recursive_search(pRoot& root, const T& val) noexcept;
};

template<class T, typename U>
inline Tree<T, U>::Tree() noexcept : _root(nullptr)
{
	u = U();
}

template<class T, typename U>
inline Tree<T, U>::Tree(const Tree<T, U>& cop)
{
	u = U();
	_root = recursive_copy(cop._root);
}

template<class T, typename U>
inline Tree<T, U>::Tree(Tree<T, U>&& per) noexcept
{
	u = U();
	_root = per->_root;
	per->_root = nullptr;
}

template<class T, typename U>
inline void Tree<T, U>::add(const T& val)
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_add(_root, val);
}

template<class T, typename U>
inline void Tree<T, U>::add(const Tree<T, U>& cop)
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_tree_add(_root, cop._root);
}

template<class T, typename U>
inline bool Tree<T, U>::find(const T& val) noexcept
{
	std::lock_guard<std::mutex> im_locked(mtx);
	return recursive_search(this->_root, val) != nullptr;
}

template<class T, typename U>
inline Tree<T, U> Tree<T, U>::subTree(const T& val)
{
	Tree<T, U> ret_tree;
	std::lock_guard<std::mutex> im_locked(mtx);
	ret_tree._root = recursive_copy(recursive_search(_root, val));
	return ret_tree;
}

template<class T, typename U>
inline void Tree<T, U>::deleteSubTree(const T& val)
{
	std::lock_guard<std::mutex> im_locked(mtx);
	pRoot& found = recursive_search(_root, val);
	if (found == nullptr)
		throw TreeException("Element was not found");
	recursive_del(found);
}

template<class T, typename U>
inline void Tree<T, U>::deleteElem(const T& val)
{
	std::lock_guard<std::mutex> im_locked(mtx);
	pRoot& deleting_elem = recursive_search(_root, val);
	if (deleting_elem == nullptr)
		throw TreeException("Elem was not found");
	pRoot deleting_memory = deleting_elem;
	if (deleting_elem->_right == nullptr)
	{
		deleting_elem = deleting_elem->_left;
	}
	else
	{
		if (deleting_elem->_right->_left == nullptr)
		{
			deleting_elem->_right->_left = deleting_elem->_left;
			deleting_elem = deleting_elem->_right;
		}
		else
		{
			pRoot& very_left_elem = deleting_elem->_right->_left;
			while (very_left_elem->_left != nullptr)
				very_left_elem = very_left_elem->_left;
			very_left_elem->_left = deleting_elem->_left;
			very_left_elem->_right = deleting_elem->_right;
			deleting_elem = very_left_elem;
			very_left_elem = nullptr;
		}
	}
	delete deleting_memory;
}

template<class T, typename U>
inline void Tree<T, U>::clear() noexcept
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_del(_root);
}

template<class T, typename U>
inline void Tree<T, U>::lkp(std::ostream& os, const char& sep) noexcept
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_lkp(_root, os, sep);
}

template<class T, typename U>
inline void Tree<T, U>::lpk(std::ostream& os, const char& sep) noexcept
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_lpk(_root, os, sep);
}

template<class T, typename U>
inline void Tree<T, U>::klp(std::ostream& os, const char& sep) noexcept
{
	std::lock_guard<std::mutex> im_locked(mtx);
	recursive_klp(_root, os, sep);
}

template<class T, typename U>
inline Tree<T, U>::~Tree()
{
	recursive_del(_root);
}

template<class T, typename U>
typename Tree<T, U>::pRoot Tree<T, U>::recursive_copy(const Tree<T, U>::pRoot& cop)
{
	Tree<T, U>::pRoot ret;
	if (cop != nullptr)
	{
		ret = new Root(cop->_value);
		if (ret == nullptr)
			throw TreeException("Memory error");
		ret->_left = recursive_copy(cop->_left);
		ret->_right = recursive_copy(cop->_right);
	}
	else
		ret = nullptr;
	return ret;
}

template<class T, typename U>
void Tree<T, U>::recursive_add(pRoot& root, const T& val)
{
	if (root == nullptr)
	{
		root = new Root(val);
		if (root == nullptr)
			throw TreeException("Memory error");
	}
	else
		recursive_add((u(val, root->_value)) ? root->_left : root->_right, val);
}

template<class T, typename U>
inline void Tree<T, U>::recursive_tree_add(pRoot& root, const pRoot& adding)
{
	if (adding != nullptr)
	{
		recursive_tree_add(root, adding->_left);
		recursive_tree_add(root, adding->_right);
		recursive_add(root, adding->_value);
	}
}

template<class T, typename U>
void Tree<T, U>::recursive_del(pRoot& root) noexcept
{
	if (root != nullptr)
	{
		recursive_del(root->_left);
		recursive_del(root->_right);
		delete root;
		root = nullptr;
	}
}

template<class T, typename U>
void Tree<T, U>::recursive_lkp(const pRoot& root, std::ostream& os, const char& sep) noexcept
{
	if (root != nullptr)
	{
		recursive_lkp(root->_left, os, sep);
		os << root->_value << sep;
		recursive_lkp(root->_right, os, sep);
	}
}

template<class T, typename U>
inline void Tree<T, U>::recursive_lpk(const pRoot& root, std::ostream& os, const char& sep) noexcept
{
	if (root != nullptr)
	{
		recursive_lpk(root->_left, os, sep);
		recursive_lpk(root->_right, os, sep);
		os << root->_value << sep;
	}
}

template<class T, typename U>
inline void Tree<T, U>::recursive_klp(const pRoot& root, std::ostream& os, const char& sep) noexcept
{
	if (root != nullptr)
	{
		os << root->_value << sep;
		recursive_klp(root->_left, os, sep);
		recursive_klp(root->_right, os, sep);
	}
}

template<class T, typename U>
typename Tree<T, U>::pRoot& Tree<T, U>::recursive_search(typename Tree<T, U>::pRoot& root, const T& val) noexcept
{
	if ((root == nullptr) || (root->_value == val))
			return root;
	else
		return recursive_search((u(val, root->_value)) ? root->_left : root->_right, val);
}

template<class T, typename U>
inline Tree<T, U>::Root::Root(const T& val) : _left(nullptr), _right(nullptr), _value(val) {}

template<class T1, typename U1>
inline std::ostream& operator<<(std::ostream& os, const Tree<T1, U1>& tree)
{
	Tree<T1, U1>::recursive_lkp(tree._root, os, ' ');
	return os;
}

char const* TreeException::what()
{
	return _error.c_str();
}

inline TreeException::TreeException(std::string error) : _error(error) {}
struct AlphabeticOrder
{
	bool operator()(const string& s1, const string& s2, const int& i = 0)
	{
		if ((s1[i] == '\0') || (s1[i] < s2[i]))
			return true;
		else
		{
			if (s1[i] > s2[i])
				return false;
			else
				return operator()(s1, s2, i + 1);
		}
	}
};


int main()
{
    Tree<int> t;
    std::thread th1([&]() {
        t.add(8);
        t.add(10);
        t.add(2);
        t.add(6);
    });
    std::thread th2([&]() {
        t.add(7);
        t.add(12);
        t.add(5);
        t.add(9);
    });

    th1.join();
    th2.join();
    cout << t << endl;

    Tree<string, AlphabeticOrder> abcTree;
    abcTree.add("carlos");
    abcTree.add("gabriel");
    abcTree.add("morales");
    abcTree.add("umasi");
    abcTree.add("erick");
    abcTree.add("oooscar");
    cout << abcTree << endl;

    // Prueba de find
    int val1 = 7;
    cout << "Find " << val1 << ": " << (t.find(val1) ? "Found" : "Not Found") << endl;

    string val2 = "gabriel";
    cout << "Find " << val2 << ": " << (abcTree.find(val2) ? "Found" : "Not Found") << endl;

    // Prueba de deleteSubTree
    int val3 = 2;
    cout << "Deleting subtree with root " << val3 << endl;
    t.deleteSubTree(val3);
    cout << t << endl;

    string val4 = "morales";
    cout << "Deleting subtree with root " << val4 << endl;
    abcTree.deleteSubTree(val4);
    cout << abcTree << endl;

    // Prueba de deleteElem
    int val5 = 10;
    cout << "Deleting element " << val5 << endl;
    t.deleteElem(val5);
    cout << t << endl;

    string val6 = "carlos";
    cout << "Deleting element " << val6 << endl;
    abcTree.deleteElem(val6);
    cout << abcTree << endl;

    return 0;
}
