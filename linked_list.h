#ifndef MESH_LINKEDLIST_H_
#define MESH_LINKEDLIST_H_

#include <stdexcept>
#include <iostream>

using namespace std;

template <class T> class DoublyLinkedList;

// Doubly Linked List Elem

template <class T>
class DoublyLinkedListElem {

    T data_;
    DoublyLinkedListElem<T> *next_;
    DoublyLinkedListElem<T> *previous_;

  public:

    // Constructors
    DoublyLinkedListElem(T);
    DoublyLinkedListElem(T, DoublyLinkedListElem<T>*, DoublyLinkedListElem<T>*);

    // Accessors
    DoublyLinkedListElem<T>* next() { return next_; }
    DoublyLinkedListElem<T>* previous() { return previous_; }
    T data() { return data_; }

    // Friend class
    friend class DoublyLinkedList<T>;
};

// Doubly Linked List

template <class T>
class DoublyLinkedList {

    DoublyLinkedListElem<T> *first_;

  public:

    // Constructor / Destructor
    DoublyLinkedList() : first_(0) {}
    ~DoublyLinkedList();
    void DeleteElems(DoublyLinkedListElem<T>*);

    DoublyLinkedListElem<T>* first() { return first_; }
    DoublyLinkedListElem<T>* last();

    // Operations
    void Append(T);
    void Prepend(T);
    void Remove(T);
    void InsertAfter(T, T);
    void InsertBefore(T, T);
    void Print();

    DoublyLinkedListElem<T>* FindElem(T data);

    // Predecessor, Successor
    DoublyLinkedListElem<T>* Predecessor(T);
    DoublyLinkedListElem<T>* Successor(T);

    int Length() const;
};


// Implementation of DoublyLinkedListElem

template <class T>
DoublyLinkedListElem<T>::DoublyLinkedListElem(T data) {
  data_ = data;
  previous_ = this;
  next_ = this;
}

template <class T>
DoublyLinkedListElem<T>::DoublyLinkedListElem(
    T data,
    DoublyLinkedListElem<T>* next,
    DoublyLinkedListElem<T>* previous):
        data_(data),
        next_(next),
        previous_(previous) {}

// Implementation of DoublyLinkedList

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList() {
  if (first_ != nullptr)
    DeleteElems(first_);
}

template <class T>
void DoublyLinkedList<T>::DeleteElems(DoublyLinkedListElem<T>* elem) {
  if (elem->next_ != first_)
    DeleteElems(elem->next_);
  delete elem;
}

template <class T>
DoublyLinkedListElem<T>* DoublyLinkedList<T>::last() {
  if (first_ == nullptr)
    return nullptr;
  return first_->previous_;
}

template <class T>
void DoublyLinkedList<T>::Append(T data) {
  if (first_ == nullptr) {
    first_ = new DoublyLinkedListElem<T>(data);
  } else {
    DoublyLinkedListElem<T> *new_elem = new DoublyLinkedListElem<T>(data, first_, first_->previous_);
    first_->previous_->next_ = new_elem;
    first_->previous_ = new_elem;
  }
}

template <class T>
void DoublyLinkedList<T>::Prepend(T data) {
  Append(data);
  first_ = first_->previous_;
}

template <class T>
DoublyLinkedListElem<T>* DoublyLinkedList<T>::FindElem(T data) {
  if (first_ == nullptr) {
    throw logic_error("Empty List");
  }
  DoublyLinkedListElem<T> *elem;
  elem = first_;
  while (elem->data_ != data) {
    elem = elem->next_;
    if (elem == first_) {
      throw logic_error("Element not Found");
    }
  }
  return elem;
}

template <class T>
void DoublyLinkedList<T>::Remove(T data) {
  // cout << "Removing Element" << endl;
  // cout << "data : " << data << endl;
  DoublyLinkedListElem<T> *elem = FindElem(data);
  // cout << "elem->data : " << elem->data_ << endl;
  // cout << "previous : " << elem->previous_->data_ << endl;
  // cout << "next : " << elem->next_->data_ << endl;
  if (elem->next_ == elem) {
    first_ = nullptr;
  } else {
    elem->previous_->next_ = elem->next_;
    elem->next_->previous_ = elem->previous_;
    if (elem == first_) {
      first_ = elem->next_;
    }
  }
  // this->Print();
  // cout << endl;
  delete elem;
}

template <class T>
void DoublyLinkedList<T>::InsertAfter(T previous, T data) {
  DoublyLinkedListElem<T> *previous_elem, *new_elem;
  previous_elem = FindElem(previous);
  new_elem = new DoublyLinkedListElem<T>(data);
  new_elem->next_ = previous_elem->next_;
  new_elem->previous_ = previous_elem;
  previous_elem->next_->previous_ = new_elem;
  previous_elem->next_ = new_elem;
}

template <class T>
void DoublyLinkedList<T>::InsertBefore(T next, T data) {
  DoublyLinkedListElem<T> *next_elem, *new_elem;
  next_elem = FindElem(next);
  new_elem = new DoublyLinkedListElem<T>(data);
  new_elem->next_ = next_elem;
  new_elem->previous_ = next_elem->previous_;
  next_elem->previous_->next_ = new_elem;
  next_elem->previous_ = new_elem;
}

template <class T>
DoublyLinkedListElem<T>* DoublyLinkedList<T>::Predecessor(T data) {
  return FindElem(data)->previous_;
}

template <class T>
DoublyLinkedListElem<T>* DoublyLinkedList<T>::Successor(T data) {
  return FindElem(data)->next_;
}

template <class T>
int DoublyLinkedList<T>::Length() const {
  if (first_ == nullptr) {
    return 0;
  }
  int length = 0;
  DoublyLinkedListElem<T> *current_elem = first_;
  do {
    current_elem = current_elem->next_;
    length++;
  } while (current_elem != first_);
  return length;
}

template <class T>
void DoublyLinkedList<T>::Print() {
  if (first_ == nullptr) {
    cout << "[]" << endl;
    return;
  }
  cout << "[" << first_->data_;
  DoublyLinkedListElem<T> *current_elem = first_->next_;
  while (current_elem != first_) {
    cout << ", " << current_elem->data_;
    current_elem = current_elem->next_;
  }
  cout << "]" << endl;
}

#endif // MESH_LINKEDLIST_H_
