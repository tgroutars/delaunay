#ifndef MESH_LINKEDLIST_H_
#define MESH_LINKEDLIST_H_

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

// Implementation

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

    // Operations
    void Append(T);
    void Prepend(T);

    int Length() const;
};

// Implementation

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
void DoublyLinkedList<T>::Append(T data) {
  if (first_ == nullptr) {
    first_ = new DoublyLinkedListElem<T>(data);
    return;
  }
  DoublyLinkedListElem<T> *new_elem = new DoublyLinkedListElem<T>(data, first_->previous_, first_);
  first_->previous_->next_ = new_elem;
  first_->previous_ = new_elem;
}

template <class T>
void DoublyLinkedList<T>::Prepend(T data) {
  Append(data);
  first_ = first_->previous_;
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


#endif // MESH_LINKEDLIST_H_
