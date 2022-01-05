#pragma once
#include "vector.h"
#include "algorithm.h"

namespace mystd {

template<typename T>
class Queue {
private:
	class Node {
		friend class Queue;
	private:
		T data;
		Node* next;
	public:
		Node() :next(nullptr) {}
		Node(T _data) :Node(_data, nullptr) {}
		Node(T _data, Node* _next) :data(_data), next(_next) {}
	};


private:
	Node* head;
	Node* tail;
	size_t queueSize;

public:
	Queue() :head(nullptr), tail(nullptr), queueSize(0) {}
	~Queue() {
		while (head) {
			Node* p = head;
			head = head->next;
			delete p;
		}
		head = tail = nullptr;
		queueSize = 0;
	}
	size_t size()const noexcept { return queueSize; }
	bool empty()const noexcept { return queueSize == 0; }

	void push(const T& value) {
		if (empty()) {
			head = tail = new Node(value);
			queueSize++;
			return;
		}
		tail->next = new Node(value, nullptr);
		tail = tail->next;
		queueSize++;
	}

	void pop() {
		queueSize--;
		Node* p = head;
		head = head->next;
		delete p;
	}

	T& front() {
		if (!empty())
			return head->data;
		throw std::out_of_range("at mystd::queue::front()");
	}

	T& back() {
		if (!empty())
			return tail->data;
		throw std::out_of_range("at mystd::queue::back()");
	}
};


template <typename T, typename Container = mystd::vector<T>, typename Compare = std::less<typename Container::value_type>> 
class priority_queue {
public:
	using container_type = Container;
	using value_type = typename Container::value_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using size_type = typename Container::size_type;

private:
	Compare comp_;
	Container container_;

public:
	/* constructor */
//TO DO: range and move-range
	explicit priority_queue(const Compare& comp, const Container& ctnr)
		: comp_(comp), container_(ctnr){
		mystd::make_heap(container_.begin(), container_.end(), comp_);
	}

	explicit priority_queue(const Compare& comp = Compare(), Container&& ctnr = Container())
		: comp_(comp), container_(std::move(ctnr)) {
		mystd::make_heap(container_.begin(), container_.end(), comp_);
	}

	~priority_queue() = default;

	bool empty() const{
		return container_.empty();
	}

	size_type size() const{
		return container_.size();
	}

	const_reference top() const{
		if (empty()){
			throw std::out_of_range("at priority_queue::top()");
		}
		return container_.front();
	}

	void pop(){
		if (empty()) {
			throw std::out_of_range("at priority_queue::top()");
		}
		mystd::pop_heap(container_.begin(), container_.end(), comp_);
		container_.pop_back();
	}

	void push(const value_type& elem){
		value_type copy = elem;
		push(std::move(copy));
	}

	void push(value_type&& elem){
		container_.push_back(elem);
		mystd::push_heap(container_.begin(), container_.end(), comp_);
	}
};


}
