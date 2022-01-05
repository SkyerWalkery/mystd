#pragma once

namespace mystd {

template<typename T>
class deque {
private:
	class Node {
		friend class deque;
	private:
		T data;
		Node* next;
		Node* prev;
	public:
		Node() :next(nullptr), prev(nullptr) {}
		Node(T _data) :Node(_data, nullptr, nullptr) {}
		Node(T _data, Node* _next, Node* _prev) :data(_data), next(_next), prev(_prev) {}
	};


private:
	Node* head;
	Node* tail;
	size_t dequeSize;

public:
	deque() :head(nullptr), tail(nullptr), dequeSize(0) {}
	~deque() {
		if (!empty()) {
			while (head) {
				Node* p = head;
				head = head->next;
				delete p;
			}
		}
		head = tail = nullptr;
		dequeSize = 0;
	}
	size_t size()const noexcept { return dequeSize; }
	bool empty()const noexcept { return dequeSize == 0; }

	void push_back(const T& value) {
		if (empty()) {
			head = tail = new Node(value);
			dequeSize++;
			return;
		}
		tail->next = new Node(value, nullptr, tail);
		tail = tail->next;
		dequeSize++;
	}

	void push_front(const T& value) {
		if (empty()) {
			head = tail = new Node(value);
			dequeSize++;
			return;
		}
		head->prev = new Node(value, head, nullptr);
		head = head->next;
		dequeSize++;
	}

	void pop_back() {
		dequeSize--;
		Node* p = tail;
		tail = tail->prev;
		if(tail)
			tail->next = nullptr;
		delete p;
	}

	void pop_front() {
		dequeSize--;
		Node* p = head;
		head = head->next;
		if(head)
			head->prev = nullptr;
		delete p;
	}

	T& front() {
		if (!empty())
			return head->data;
		throw std::out_of_range("at mystd::deque::front()");
	}

	T& back() {
		if (!empty())
			return tail->data;
		throw std::out_of_range("at mystd::deque::front()");
	}
};
}
