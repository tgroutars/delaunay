#include "linked_list.h"


// DoublyLinkedListElem

template <class T>
DoublyLinkedListElem<T>::DoublyLinkedListElem(T data) {
  data_ = data;
  previous_ = this;
  next_ = this;
}

template <class T>
DoublyLinkedListElem<T>::DoublyLinkedListElem(T data,
                                              DoublyLinkedListElem<T>* next,
                                              DoublyLinkedListElem<T>* previous) {
  data_ = data;
  previous_ = previous;
  next_ = next;
}


// DoublyLinkedList

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
