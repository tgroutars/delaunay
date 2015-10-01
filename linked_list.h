#ifndef MESH_LINKEDLIST_H_
#define MESH_LINKEDLIST_H_

template <class T>
class DoublyLinkedList {
    T *data;
    DoublyLinkedList *next_;
    DoublyLinkedList *previous_;
  public:
    DoublyLinkedList* next() { return next_; }
    DoublyLinkedList* previous() { return previous_; }
};

#endif // MESH_LINKEDLIST_H_
