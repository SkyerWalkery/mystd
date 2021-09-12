#pragma once
#include <iostream>
/*
* Project url: https://github.com/SkyerWalkery/mystd
* 
* To distinguish it from std::list, I name it with mystd::List
* Lastest update: 2021/9/13
* Iterator support: Iterator and const_Iterator
* Function support: 
*	Initialize without parameter
*	Copy constructor and operator=
*	begin, end, cbegin, cend
*	empty, size
*	front, back
*	push_front, push_back, insert, erase, clear
* For more information, please refer to class declaration
*/
namespace mystd {

	using std::endl; 
	using std::cerr;

	template <typename T> class List {
	public:
		using size_type = unsigned int;//别名
		using reference = T&;
		using const_reference = const T&;

	private:
		//节点定义
		class Node {
		public:
			Node() = default;
			Node(T _data, Node* _next, Node* _prev) :data(_data), next(_next), prev(_prev) {}
			T data;
			Node* next = nullptr;
			Node* prev = nullptr;
		};

	public:
		//迭代器
		class Iterator {
			friend class List<T>;
		public:
			Iterator();
			~Iterator() = default;
			explicit Iterator(Node* p);
			Iterator& operator++();
			Iterator& operator--();
			Iterator operator++(int);
			Iterator operator--(int);
			bool operator==(const Iterator& other) const;
			bool operator!=(const Iterator& other) const;
			bool operator==(Iterator&& other) const;
			bool operator!=(Iterator&& other) const;//左值、右值版本
			reference operator*() const;//解引用

		protected:
			Node* pointer;//封装的指针，指向节点
		};

		class const_Iterator :public Iterator {
		public:
			const_Iterator() :Iterator() {}
			const_Iterator(Node* p) :Iterator(p) {}//继承所有构造函数
			const_Iterator(const Iterator& rhs) :Iterator(rhs.pointer) {};
			const_Iterator(Iterator&& rhs) :Iterator(rhs.pointer) {};//允许Iterator到const_Iterator的转换
			~const_Iterator() = default;
			const_Iterator& operator++();
			const_Iterator& operator--();
			const_Iterator operator++(int);
			const_Iterator operator--(int);
			const_reference operator*() const;//解引用
			//==, !=继承基类
		
		};

	public:
		List();
		List(const List& other);
		List(List&& other);
		~List();
		List& operator=(const List& other);
		List& operator=(List&& other);
		bool operator==(const List& other)const;
		bool operator==(List&& other)const;

		Iterator begin() noexcept;
		Iterator end() noexcept;
		const_Iterator cbegin()const noexcept;
		const_Iterator cend()const noexcept;//返回尾后迭代器
		reference front();
		reference back();
		const_reference front()const;
		const_reference back()const;
		size_type size() const noexcept;
		bool empty() const noexcept;

		Iterator insert(const_Iterator position, const_reference object);
		Iterator insert(const_Iterator position, T&& object);
		void push_back(const_reference object);
		void push_front(const_reference object);
		void push_back(T&& object);
		void push_front(T&& object);

		void clear()noexcept;

		Iterator erase(const_Iterator position);
		Iterator erase(const_Iterator first, const_Iterator last);//[beg, end)


	private:
		void __init__();
		Node* head = nullptr;
		Node* tail = nullptr;//实际上是尾后指针，不存储值
		size_type listSize = 0;

	};



	template <typename T>
	List<T>::Iterator::Iterator() :pointer(nullptr) {}


	template<typename T>
	List<T>::Iterator::Iterator(List<T>::Node* p) : pointer(p) {}


	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator++() {
		if (pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
        pointer = pointer->next;
        if(pointer == nullptr){
            cerr << "Error: Out of memory!\n";
            exit(-1);
        }
		return *this;
	}


	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator--() {
		if (pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
		pointer = pointer->prev;
		if(pointer == nullptr){
            cerr << "Error: Out of memory!\n";
            exit(-1);
		}
		return *this;
	}


	template<typename T>
	typename List<T>:: Iterator List<T>::Iterator::operator++(int) {
		Iterator ret = *this;
		++(*this);
		return ret;
	}


	template<typename T>
	typename List<T>:: Iterator List<T>::Iterator::operator--(int) {
		Iterator ret = *this;
		--(*this);
		return ret;
	}


	template<typename T>
	bool List<T>::Iterator::operator==(const Iterator& other) const {
		return this->pointer == other.pointer;
	}


	template<typename T>
	bool List<T>::Iterator::operator!=(const Iterator& other) const {
		return this->pointer != other.pointer;
	}


	template<typename T>
	bool List<T>::Iterator::operator==(Iterator&& other) const {
		return this->pointer == other.pointer;
	}


	template<typename T>
	bool List<T>::Iterator::operator!=(Iterator&& other) const {
		return this->pointer != other.pointer;
	}


	template<typename T>
	typename List<T>::reference List<T>::Iterator::operator*() const
	{
		return pointer->data;
	}


	template<typename T>
	typename List<T>::const_Iterator& List<T>::const_Iterator::operator++() {
		if (this->pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
		this->pointer = this->pointer->next;
		if (this->pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
		return *this;
	}


	template<typename T>
	typename List<T>::const_Iterator& List<T>::const_Iterator::operator--() {
		if (this->pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
		this->pointer = this->pointer->prev;
		if (this->pointer == nullptr) {
			cerr << "Error: Out of memory!\n";
			exit(-1);
		}
		return *this;
	}


	template<typename T>
	typename List<T>::const_Iterator List<T>::const_Iterator::operator++(int) {
		const_Iterator ret = *this;
		++(*this);
		return ret;
	}


	template<typename T>
	typename List<T>::const_Iterator List<T>::const_Iterator::operator--(int) {
		const_Iterator ret = *this;
		--(*this);
		return ret;
	}


	template<typename T>
	typename List<T>::const_reference List<T>::const_Iterator::operator*() const
	{
		return this->pointer->data;
	}


	template<typename T>
	List<T>::List() {
		__init__();
	}

	template<typename T>
	List<T>::List(const List& other){
		__init__();
		*this = other;
	}

	template<typename T>
	List<T>::List(List&& other){
		__init__();
		*this = other;
	}


	template<typename T>
	List<T>::~List() {
		Node* p;
		while (head) {
			p = head;
			head = head->next;
			delete p;
		}
	}

	template<typename T>
	typename List<T>::List& List<T>::operator=(const List& other){
		if (this == &other)
			return *this;

		this->clear();
		for (List<T>::const_Iterator it = other.cbegin(); it != other.cend(); ++it) {
			this->push_back(*it);
		}
		return *this;
	}

	template<typename T>
	typename List<T>::List& List<T>::operator=(List&& other){
		if (this == &other)
			return *this;

		this->clear();
		for (List<T>::const_Iterator it = other.cbegin(); it != other.cend(); ++it) {
			this->push_back(*it);
		}
		return *this;
	}

	template<typename T>
	bool List<T>::operator==(const List& other) const{
		return head == other.head && tail == other.tail && listSize == other.listSize;
	}

	template<typename T>
	bool List<T>::operator==(List&& other) const{
		return head == other.head && tail == other.tail && listSize == other.listSize;
	}


	template <typename T>
	typename List<T>::Iterator List<T>::begin() noexcept{
		return Iterator(this->head);
	}


	template <typename T>
	typename List<T>::Iterator List<T>::end() noexcept{
		return Iterator(this->tail);
	}
	

	template <typename T>
	typename List<T>::const_Iterator List<T>::cbegin()const noexcept {
		return const_Iterator(this->head);
	}


	template <typename T>
	typename List<T>::const_Iterator List<T>::cend()const noexcept{
		return const_Iterator(this->tail);
	}


	template <typename T>
	typename List<T>::reference List<T>::front() {
		if (empty()) {
			cerr << "Error: Empty List!\n";
			exit(-1);
		}
		return begin()->data;
	}


	template <typename T>
	typename List<T>::reference List<T>::back() {
		Iterator it = end();
		--it;//错误处理交给operator--
		return it->data;
	}


	template <typename T>
	typename List<T>::const_reference List<T>::front()const {
		if (empty()) {
			cerr << "Error: Empty List!\n";
			exit(-1);
		}
		return begin()->data;
	}


	template <typename T>
	typename List<T>::const_reference List<T>::back()const {
		const_Iterator it = cend();
		--it;//错误处理交给operator--
		return it->data;
	}


	template <typename T>
	typename List<T>::size_type List<T>::size()const noexcept {
		return listSize;
	}


	template<typename T>
	bool List<T>::empty() const noexcept {
		return listSize == 0;
	}


	template<typename T>
	typename List<T>::Iterator List<T>::insert(const_Iterator position, const_reference object) {
		Node* p = new Node(object, nullptr, nullptr);
		if (p == nullptr) {
			cerr << "New Failed" << endl;
			exit(-1);
		}

		Node* it_p = position.pointer;
		if (empty()) {
			head = p;
			p->next = tail;
			tail->prev = p;
		}
		else if (position == cbegin()) {
			p->next = head;
			head->prev = p;
			head = p;
		}
		else {
			it_p->prev->next = p;
			p->prev = it_p->prev;
			p->next = it_p;
			it_p->prev = p;
		}

		++listSize;

		return Iterator(p);
	}


	template<typename T>
	typename List<T>::Iterator List<T>::insert(const_Iterator position, T&& object) {
		Node* p = new Node(object, nullptr, nullptr);
		if (p == nullptr) {
			cerr << "New Failed" << endl;
			exit(-1);
		}

		Node* it_p = position.pointer;
		if (empty()) {
			head = p;
			p->next = tail;
			tail->prev = p;
		}
		else if (position == cbegin()) {
			p->next = head;
			head->prev = p;
			head = p;
		}
		else {
			it_p->prev->next = p;
			p->prev = it_p->prev;
			p->next = it_p;
			it_p->prev = p;
		}

		++listSize;

		return Iterator(p);
	}


	template<typename T>
	void List<T>::push_back(const_reference object) {
		insert(cend(), object);
	}


	template<typename T>
	void List<T>::push_front(const_reference object) {
		insert(cbegin(), object);
	}


	template<typename T>
	void List<T>::push_back(T&& object) {
		insert(cend(), object);
	}


	template<typename T>
	void List<T>::push_front(T&& object) {
		insert(cbegin(), object);
	}


	template<typename T>
	void List<T>::clear() noexcept{
		while (head != tail) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		listSize = 0;
	}


	template<typename T>
	typename List<T>::Iterator List<T>::erase(const_Iterator position) {
		if (position == cend()) {
			cerr << "Invalid Iterator\n";
			exit(-1);
		}
		if (position == cbegin()) {
			Node* temp = head;
			Iterator ret((++position).pointer);
			head = head->next;
			head->prev = nullptr;
			delete temp;
			--listSize;
			return ret;
		}
		else {
			Node* temp = position.pointer;
			Iterator ret((++position).pointer);
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
			--listSize;
			return ret;
		}
	}


	template<typename T>
	typename List<T>::Iterator List<T>::erase(const_Iterator first, const_Iterator last) {
		for (const_Iterator it = first; it != last;) {
			it = erase(it);//错误处理交给erase(it)
		}
		return last;
	}


	template<typename T>
	inline void List<T>::__init__(){
		Node* pNode = new Node;
		head = pNode;
		tail = pNode;
	}



}

