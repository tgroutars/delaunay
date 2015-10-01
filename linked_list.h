#ifndef MESH_LINKEDLIST_H_
#define MESH_LINKEDLIST_H_

template <class T> class DoublyLinkedList;

template <class T>
class DoublyLinkedListElem {

    T *data_;
    DoublyLinkedListElem<T> *next_;
    DoublyLinkedListElem<T> *previous_;

  public:

    // Constructors
    DoublyLinkedListElem(T);
    DoublyLinkedListElem(T, DoublyLinkedListElem<T>*, DoublyLinkedListElem<T>*);

    // Accessors
    DoublyLinkedListElem<T>* next() { return next_; }
    DoublyLinkedListElem<T>* previous() { return previous_; }

    // Friend class
    friend class DoublyLinkedList<T>;
};

template <class T>
class DoublyLinkedList {

    DoublyLinkedListElem<T> *first_;

  public:

    // Constructor / Destructor
    DoublyLinkedList() : first_(0) { }
    ~DoublyLinkedList();
    void DeleteElems(DoublyLinkedListElem<T>*);

    // Operations
    void Append(T);
    void Prepend(T);
};

#endif // MESH_LINKEDLIST_H_
