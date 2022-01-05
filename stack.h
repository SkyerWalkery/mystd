#pragma once

namespace mystd {
	
	template<typename T>
	class stack {
	private:
		class Node {
			friend class stack;
		private:
			T data;
			Node* next;
			Node* prev;
		public:
			Node() :next(nullptr), prev(nullptr) {}
			Node(T _data) :Node(_data, nullptr, nullptr) {}
			Node(T _data, Node* _next, Node* _prev) :data(_data), next(next), prev(_prev) {}
		};


	private:
		//封装链表，头节点做栈底，尾节点为栈顶
		Node* head;
		Node* tail;
		size_t stackSize;

	public:
		stack() :head(nullptr), tail(nullptr), stackSize(0) {}
		~stack() {
			while (head) {
				Node* p = head;
				head = head->next;
				delete p;
			}
			head = tail = nullptr;
			stackSize = 0;
		}

		size_t size()const noexcept { return stackSize; }
		bool empty()const noexcept { return stackSize == 0; }

		void push(const T& value) {
			if (empty()) {
				head = tail = new Node(value);
				stackSize++;
				return;
			}
			tail->next = new Node(value, nullptr, tail);
			tail = tail->next;
			stackSize++;
		}

		void push(T&& value) {
			if (empty()) {
				head = tail = new Node(value);
				stackSize++;
				return;
			}
			tail->next = new Node(value, nullptr, tail);
			tail = tail->next;
			stackSize++;
		}

		void pop() {
			stackSize--;
			if (empty()) {
				delete tail;
				tail = head = nullptr;
				return;
			}
			Node* p = tail;
			tail = tail->prev;
			tail->next = nullptr;
			delete p;
		}

		T& top() {
			if (!empty())
				return tail->data;
			throw std::out_of_range("at top()");
		}

	};
}

