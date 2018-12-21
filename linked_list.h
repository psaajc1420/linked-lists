/*
    Jacob Cadena
    11/22/18
    linked_list.h
    
    This is an implementation of a list data structure. 
    
*/

#ifndef DATA_STRUCTURES_LINKED_LISTS_LINKED_LIST_H_
#define DATA_STRUCTURES_LINKED_LISTS_LINKED_LIST_H_

#include <iostream>

template <typename T>
struct Node {
  T data;
  Node* next;
};

template <typename T>
class LinkedListIterator {

 public:
  LinkedListIterator();
  LinkedListIterator(Node<T>*);
  T& operator*() const;
  LinkedListIterator<T> operator++ ();
  bool operator== (const LinkedListIterator<T>&) const;
  bool operator!= (const LinkedListIterator<T>&) const;

 private:
  Node<T>* current;
};


template <typename T>
class LinkedList {

 public:
  LinkedList();
  LinkedList(const LinkedList<T> &);
  const LinkedList<T>& operator= (const LinkedList<T>&);
  ~LinkedList();

  typedef LinkedListIterator<T> iterator;

  void PushBack(T);
  void PushFront(T);
  void Insert(T);
  // void popFront();
  // void popBack();
  void Remove(T);

  void Sort();

  template <typename Compare>
  void Sort(Compare);

  T Back() const;
  T Front() const;
  LinkedListIterator<T> Begin();
  LinkedListIterator<T> End();
  bool IsEmpty() const;
  unsigned int Size() const;
  void Print(bool newLine=true) const;
  void Clear();

  Node<T>* GetHead() const;
  Node<T>* GetTail() const;

 private:
  Node<T>* head_;
  Node<T>* tail_;
  unsigned int length_;

  void Copy(const LinkedList<T>&);
  void Initialize();
  void MergeSort(Node<T>**);
  template <typename Compare>
  void MergeSort(Node<T>**, Compare);
  void FrontBackSplit(Node<T>* source, Node<T>** front,
                      Node<T>** back);
  Node<T>* Merge(Node<T>*, Node<T>*);
  template <typename Compare>
  Node<T>* Merge(Node<T>*, Node<T>*, Compare);
};

template <typename T>
LinkedListIterator<T>::LinkedListIterator() {
  current = nullptr;
}

template <typename T>
LinkedListIterator<T>::LinkedListIterator(Node<T>* node) {
  current = node;
}

template <typename T>
T& LinkedListIterator<T>::operator*() const {
  assert(current != nullptr);
  return current->data;
}

template <typename T>
LinkedListIterator<T> LinkedListIterator<T>::operator++() {
  assert(current != nullptr);
  current = current->next;
  return *this;
}

template <typename T>
bool LinkedListIterator<T>::operator== 
    (const LinkedListIterator<T>& right) const {
  return (current == right.current);
}

template <typename T>
bool LinkedListIterator<T>::operator!=
    (const LinkedListIterator<T>& right) const {
  return (current != right.current);
}

template <typename T>
LinkedList<T>::LinkedList() {
  Initialize(); 
}

/*
    Implementation of the copying the list 
*/
template <typename T>
void LinkedList<T>::Copy(const LinkedList<T> &list) {

  // If the list is not empty, make it empty
  if (head_ != nullptr)
    Clear();

  if (list.head_ != nullptr)  {
    length_ = list.length_;

    // Traverse through the linked list passed in
    Node<T> *current = list.head_;
    Node<T> *newNode;

    // Copy the first node
    head_ = new Node<T>();
    head_->data = current->data;
    head_->next = nullptr;
    tail_ = head_;
    current = current->next;

    // Copy the remaining nodes if applicable
    while (current != nullptr) {
      newNode = new Node<T>();
      newNode->data = current->data;
      newNode->next = nullptr;
      tail_->next = newNode;
      tail_ = newNode;
      current = current->next;
    }

  } else {
    Initialize();
  }
}

template <typename T>
void LinkedList<T>::Initialize() {
  head_ = nullptr;
  tail_ = nullptr;
  length_ = 0;
}

/*
    Implementation of the Copy Constructor
*/
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
  head_ = nullptr;
  Copy(list);
}

/*
    Implementation of the assignment operator
*/
template <typename T>
const LinkedList<T>& LinkedList<T>::operator= (const LinkedList<T>& list) {
    
  if(this != &list)
    Copy(list);

  return *this;
}

/*
    Implementation of the Destructor
*/
template <typename T>
LinkedList<T>::~LinkedList() {
  Clear();
}

/*
    Implementation of the clear 
*/
template <typename T>
void LinkedList<T>::Clear() {

  Node<T>* current = head_;
  Node<T>* temp = nullptr;
  while(current != nullptr) {
    temp = current;
    current = current->next;
    delete temp;
  }
  Initialize();
}

template <typename T>
void LinkedList<T>::Sort() {
  MergeSort(&head_);
}

template <typename T>
template <typename Compare>
void LinkedList<T>::Sort(Compare compare) {
  MergeSort(&head_, compare);
}

template <typename T> 
void LinkedList<T>::MergeSort(Node<T>** head_ref) {
  Node<T>* head = *head_ref;
  Node<T>* left;
  Node<T>* right;

  // Base case for recursion (length is 0 or 1)
  if((head == nullptr) || (head->next == nullptr)) { return; }
  
  // Split the head into sublists (left and right)
  FrontBackSplit(head, &left, &right);

  // Recursively sort the sublists
  MergeSort(&left);
  MergeSort(&right);

  *head_ref = Merge(left, right);
}

template <typename T> 
template <typename Compare>
void LinkedList<T>::MergeSort(Node<T>** head_ref,
                              Compare compare) {
  Node<T>* head = *head_ref;
  Node<T>* left;
  Node<T>* right;

  // Base case for recursion (length is 0 or 1)
  if((head == nullptr) || (head->next == nullptr)) { return; }
  
  // Split the head into sublists (left and right)
  FrontBackSplit(head, &left, &right);

  // std::cout << "Before MergeSort Left: " << std::endl;

  // Recursively sort the sublists
  MergeSort(&left, compare);
  // std::cout << "Before MergeSort Right: " << std::endl;
  MergeSort(&right, compare);

  // std::cout << "Before Merge: " << std::endl;
  *head_ref = Merge(left, right, compare);
}

template <typename T>
void LinkedList<T>::FrontBackSplit(Node<T>* source, Node<T>** front,
                                   Node<T>** back) {
  Node<T>* fast;
  Node<T>* slow;
  slow = source;
  fast = source->next;

  // Fast node traverses two times faster than the
  // the slow node
  while(fast != nullptr) {
    fast = fast->next;
    while(fast != nullptr) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *front = source;
  *back = slow->next;    
  slow->next = nullptr;                                
}

template <typename T> 
Node<T>* LinkedList<T>::Merge(Node<T>* left, Node<T>* right) {
  
  Node<T>* result = nullptr; 

  // Base cases
  if (left == nullptr)  return right; 
  else if (right == nullptr) return left; 

  // Pick either left or right, and recur
  if (left->data <= right->data) { 
    result = left; 
    result->next = Merge(left->next, right); 
  } else { 
    result = right; 
    result->next = Merge(left, right->next); 
  } 

  return result;
}

template <typename T> 
template <typename Compare>
Node<T>* LinkedList<T>::Merge(Node<T>* left, Node<T>* right,
                              Compare compare) {
  
  Node<T>* result = nullptr; 

  if(left != nullptr && right != nullptr) {
    // std::cout << "Left Data: " << left->data << std::endl;
    // std::cout << "Right Data: " << right->data << std::endl;
    // std::cout << std::endl;
  }

  // Base cases
  if (left == nullptr)  return right; 
  else if (right == nullptr) return left; 

  // Pick either left or right, and recur
  if (compare(left->data, right->data)) { 
    result = left; 
    result->next = Merge(left->next, right, compare); 
  } else { 
    result = right; 
    result->next = Merge(left, right->next, compare); 
  } 

  return result;
}

template <typename T>
void LinkedList<T>::Insert(T data) {

  Node<T>* newNode = new Node<T>();
  newNode->data = data;
  newNode->next = nullptr;

  if(head_ != nullptr) {
    Node<T> current = head_;
    while(current != nullptr) {
      if(data < current->data) {

      }
      current = current->next;
    }
  } else {
    head_ = newNode;
    tail_ = head_;
  }

  length_++;
}

template <typename T>
void LinkedList<T>::Remove(T data) {
  assert(head_ != nullptr);
  // Check if list only contains one element.
  if(head_->next == nullptr) {
    delete head_;
    Initialize();
  } else {

    Node<T>* current = head_;
    Node<T>* previous = nullptr;
    Node<T>* temp = nullptr;

    // iterate over the list until the last element
    // is reached then break if the data was not found
    while(current->next != nullptr) {
      if(current->data == data) break;
      previous = current;
      current = current->next;
    }

    // check whether or not current is the first node
    // or last node as they are special cases
    if(current == head_) {
      temp = current;
      head_ = current->next;
      delete temp;
      length_--;
    } else if(current->next == nullptr) {
      if(current->data == data) {
        temp = current;
        previous->next = nullptr;
        current = previous;
        tail_ = current;
        delete temp;
        length_--;
      }
      current = current->next;
    } else if(current != nullptr) {
      temp = current;
      previous->next = current->next;
      delete temp;
      length_--;
    } else {
      std::cerr << "Something is very wrong" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

template <typename T>
void LinkedList<T>::PushBack(T data) {

  Node<T>* newNode = new Node<T>();
  newNode->data = data;
  newNode->next = nullptr;

  if(head_ != nullptr) {
    tail_->next = newNode;
    tail_ = newNode;
  } else {
    head_ = newNode;
    tail_ = head_;
  }

  length_++;
}

template <typename T>
void LinkedList<T>::PushFront(T data) {

  Node<T>* newNode = new Node<T>();
  newNode->data = data;

  if(head_ != nullptr) {
    newNode->next = head_;
    head_ = newNode;
  } else {
    newNode->next = nullptr;
    head_ = newNode;
    tail_ = head_;
  }

  length_++;
}



template <typename T>
LinkedListIterator<T> LinkedList<T>::Begin() {
  LinkedListIterator<T> temp(head_);
  return temp;
}

template <typename T>
LinkedListIterator<T> LinkedList<T>::End() {
  LinkedListIterator<T> temp(nullptr);
  return temp;
}

template <typename T>
void LinkedList<T>::Print(bool newLine) const {
  Node<T>* current = head_;
  while(current != nullptr) {
    if(newLine) {
      std::cout << current->data << std::endl;
    } else {
      std::cout << current->data << " ";
    }
    current = current->next;
  }
  if(newLine){
    std::cout << std::endl;
  }
  else {
    std::cout << std::endl << std::endl;
  }
}

template <typename T>
unsigned int LinkedList<T>::Size() const { return length_; }

template <typename T>
T LinkedList<T>::Front() const { return head_->data; }

template <typename T>
T LinkedList<T>::Back() const { return tail_->data; }

template <typename T>
bool LinkedList<T>::IsEmpty() const { 
  return (head_ == nullptr); 
}

template <typename T>
Node<T>* LinkedList<T>::GetHead() const { return head_; };

template <typename T>
Node<T>* LinkedList<T>::GetTail() const { return tail_; };

#endif // DATA_STRUCTURES_LINKED_LISTS_LINKED_LIST_H_