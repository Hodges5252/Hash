/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *
 *    __    ___     ___    _   __
 *   /_ |  / _ \   / _ \  (_) / /
 *    | | | | | | | | | |    / /
 *    | | | | | | | | | |   / /
 *    | | | |_| | | |_| |  / / _
 *    |_|  \___/   \___/  /_/ (_)
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
public:
   //
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list()
   {
      clear();
   }

   //
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);

   //
   // Iterator
   //

   class  iterator;
   iterator begin()  { return iterator(pHead);   }
   iterator rbegin() { return iterator(pTail);   }
   iterator end()   { return iterator(nullptr); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   //
   // Status
   //

   bool empty()  const { return size() == 0; }
   size_t size() const { return numElements; }


#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // number of elements
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node(               ) : pNext(nullptr), pPrev(nullptr), data(               ) {}
   Node(const T &  data) : pNext(nullptr), pPrev(nullptr), data(data           ) {}
   Node(      T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) {}

   //
   // Data
   //

   T data;       // user data
   Node * pNext; // pointer to next node
   Node * pPrev; // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
public:
   // constructors, destructors, and assignment operator
   iterator()
      : p(nullptr)
   {
   }
   iterator(Node * p)
      : p(p)
   {
   }
   iterator(const iterator  & rhs)
      : p(nullptr)
   {
      *this = rhs;
   }
   iterator & operator = (const iterator & rhs)
   {
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return rhs.p == this->p; }
   bool operator != (const iterator & rhs) const { return rhs.p != this->p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      if (p)
         return p->data;
      else
         throw "ERROR: Trying to dereference a nullptr pointer";
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator old(*this);
      if (p)
         p = p->pNext;
      return old;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      if (p)
         p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator old(*this);
      if (p)
         p = p->pPrev;
      return old;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      if (p)
         p = p->pPrev;
      return *this;
   }

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) :
   numElements(0), pHead(nullptr), pTail(nullptr)
{
   if (num)
   {
      list <T> ::Node* pNew;
      list <T> ::Node* pPrevious;

      // first create the head element
      pHead = pPrevious = pNew = new list <T> ::Node(t);
      pHead->pPrev = nullptr;

      // next, create the middle elements
      for (size_t i = 1; i < num; i++)
      {
         assert(pPrevious != nullptr);
         pNew = new list <T> ::Node(t);
         pNew->pPrev = pPrevious;
         pNew->pPrev->pNext = pNew;
         pPrevious = pNew;
      }

      // finally, hook up the tail to the last element
      pNew->pNext = nullptr;
      pTail = pNew;
      numElements = num;
   }
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last)
   : numElements(0), pHead(nullptr), pTail(nullptr)
{
   for (auto it = first; it != last; ++it)
      push_back(*it);
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
   : numElements(0), pHead(nullptr), pTail(nullptr)
{
   *this = il;
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
   : numElements(0), pHead(nullptr), pTail(nullptr)
{
   if (num)
   {
      list <T> ::Node* pNew;
      list <T> ::Node* pPrevious;

      // first create the head element
      pHead = pPrevious = pNew = new list <T> ::Node();
      pHead->pPrev = nullptr;

      // next, create the middle elements
      for (size_t i = 1; i < num; i++)
      {
         assert(pPrevious != nullptr);
         pNew = new list <T> ::Node();
         pNew->pPrev = pPrevious;
         pNew->pPrev->pNext = pNew;
         pPrevious = pNew;
      }

      // finally, hook up the tail to the last element
      pNew->pNext = nullptr;
      pTail = pNew;
      numElements = num;
   }
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list()
   : numElements(0), pHead(nullptr), pTail(nullptr)
{
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs)
   : numElements(0), pHead(nullptr), pTail(nullptr)
{
   *this = rhs;
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
   : numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   // clear the old list
   clear();
   
   // swap old and new
   swap(*this, rhs);

   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   auto itRHS = rhs.begin();
   auto itLHS = begin();

   // replace the slots the are already filled using the assignment operator
   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }

   // if there are still items the RHS, then push them back
   if (itRHS != rhs.end())
   {
      for (; itRHS != rhs.end(); ++itRHS)
         push_back(*itRHS);
   }
   // if the RHS is empty, then just clear the list
   else if (rhs.empty())
   {
      clear();
   }
   // if there are excess items on the list, remove them
   else if (itLHS != end())
   {
      list <T> ::Node* p = itLHS.p;
      assert(p);
      pTail = p->pPrev;
      list <T> ::Node* pNext = p->pNext;
      for (p = itLHS.p; p; p = pNext)
      {
         pNext = p->pNext;
         delete p;
         numElements--;
      }
      pTail->pNext = nullptr;
   }

   // return the new buffer
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
   auto itRHS = rhs.begin();
   auto itLHS = begin();

   // replace the slots the are already filled using the assignment operator
   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      ++itLHS;
      ++itRHS;
   }

   // if there are still items the RHS, then push them back
   if (itRHS != rhs.end())
   {
      for (; itRHS != rhs.end(); ++itRHS)
         push_back(*itRHS);
   }
   // if the RHS is empty, then just clear the list
   else if (rhs.size() == 0)
   {
      clear();
   }
   // if there are excess items on the list, remove them
   else if (itLHS != end())
   {
      list <T> ::Node* p = itLHS.p;
      assert(p);
      pTail = p->pPrev;
      list <T> ::Node* pNext = p->pNext;
      for (p = itLHS.p; p; p = pNext)
      {
         pNext = p->pNext;
         delete p;
         numElements--;
      }
      pTail->pNext = nullptr;
   }

   // return the new buffer
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
   // loop through the entire list, removing everything
   list <T> :: Node * pNext;
   for (list <T> :: Node * p = pHead; p; p = pNext)
   {
      pNext = p->pNext;
      delete p;
   }

   // set the member variables to the cleared state
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   assert(numElements >= 0);
   try
   {
      // create the node
      list <T> :: Node * pNew = new list <T> :: Node (data);

      // point it to the old tail
      pNew->pPrev = pTail;

      // now point tail to the new guy
      if (pTail != nullptr)
         pTail->pNext = pNew;
      else
         pHead = pNew;   // there is no tail so there is no head!
      
      // finally, this is the new tail
      pTail = pNew;
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }
}

template <typename T>
void list <T> ::push_back(T && data)
{
   assert(numElements >= 0);
   try
   {
      // create the node
      list <T> ::Node* pNew = new list <T> ::Node(std::move(data));

      // point it to the old tail
      pNew->pPrev = pTail;

      // now point tail to the new guy
      if (pTail != nullptr)
         pTail->pNext = pNew;
      else
         pHead = pNew;   // there is no tail so there is no head!

      // finally, this is the new tail
      pTail = pNew;
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   assert(numElements >= 0);
   try
   {
      // create the node
      list <T> :: Node * pNew = new list <T> :: Node (data);

      // point it to the old head
      pNew->pNext = pHead;

      // now point head to the new guy
      if (pHead != nullptr)
         pHead->pPrev = pNew;
      else
         pTail = pNew;       // there is no head so there is no tail!

      // finally, this is the new head
      pHead = pNew;
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }
}

template <typename T>
void list <T> ::push_front(T && data)
{
   assert(numElements >= 0);
   try
   {
      // create the node
      list <T> ::Node* pNew = new list <T> ::Node(std::move(data));

      // point it to the old head
      pNew->pNext = pHead;

      // now point head to the new guy
      if (pHead != nullptr)
         pHead->pPrev = pNew;
      else
         pTail = pNew;       // there is no head so there is no tail!

      // finally, this is the new head
      pHead = pNew;
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   assert(numElements >= 0);

   // when there are many elements, only change the tail
   if (numElements > 1)
   {
      assert(pTail != nullptr);
      pTail = pTail->pPrev;
      delete pTail->pNext;
      pTail->pNext = nullptr;
      numElements--;
   }
   // when there is just one element, set the list to empty;
   else if (numElements == 1)
   {
      assert(pTail != nullptr);
      assert(pHead != nullptr);
      assert(pHead == pTail);
      delete pTail;
      pHead = pTail = nullptr;
      numElements = 0;
   }
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   assert(numElements >= 0);

   // when there are many elements, only change the tail
   if (numElements > 1)
   {
      assert(pHead != nullptr);
      pHead = pHead->pNext;
      delete pHead->pPrev;
      pHead->pPrev = nullptr;
      numElements--;
   }
   // when there is just one element, set the list to empty;
   else if (numElements == 1)
   {
      assert(pTail != nullptr);
      assert(pHead != nullptr);
      assert(pHead == pTail);
      delete pTail;
      pHead = pTail = nullptr;
      numElements = 0;
   }
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
   assert(numElements >= 0);
   if (!empty())
      return pHead->data;
   else
      throw "ERROR: unable to access data from an empty list";
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   assert(numElements >= 0);
   if (!empty())
      return pTail->data;
   else
      throw "ERROR: unable to access data from an empty list";
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
   assert(numElements >= 0);
   list <T> :: iterator itNext = end();
   
   // invalid iterator case
   if (it == end())
      return it;
   assert(pHead && pTail);
   
   // fixup everything after the current location
   if (it.p->pNext)   // not the last element
   {
      it.p->pNext->pPrev = it.p->pPrev;
      itNext = it.p->pNext;
   }
   else  // are the last element
      pTail = pTail->pPrev;

   // fixup everything before the current location
   if (it.p->pPrev) // not the first element
      it.p->pPrev->pNext = it.p->pNext;
   else
      pHead = pHead->pNext;

   // delete self and return
   delete it.p;
   numElements--;
   return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data)
{
   // empty list case
   if (empty())
   {
      assert(pTail == nullptr && pHead == nullptr);
      pHead = pTail = new list <T> :: Node (data);
      numElements = 1;
      return begin();
   }

   // make sure pHead and pTail are correctly set
   assert(pTail && pHead);
   assert(pHead->pPrev == nullptr);
   assert(pTail->pNext == nullptr);
   assert(numElements >= 0);

   try
   {
      list <T> :: Node * pNew = new list <T> :: Node (data);

      // end of list case
      if (it == end())
      {
         // update pTail as appropriate
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;

         // update the iterator
         it = pNew;
      }
         // otherwise we have a valid iterator
      else
      {
         // set pNext and pPrev
         pNew->pPrev = it.p->pPrev;
         pNew->pNext = it.p;

         // update who pNext and pPrev point to
         if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
         else
            pHead = pNew;
         if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
         else
            pTail = pNew;

         it = pNew;
      }
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }

   return it;
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
   // empty list case
   if (empty())
   {
      assert(pTail == nullptr && pHead == nullptr);
      pHead = pTail = new list <T> ::Node(std::move(data));
      numElements = 1;
      return begin();
   }

   // make sure pHead and pTail are correctly set
   assert(pTail && pHead);
   assert(pHead->pPrev == nullptr);
   assert(pTail->pNext == nullptr);
   assert(numElements >= 0);

   try
   {
      list <T> ::Node* pNew = new list <T> ::Node(std::move(data));

      // end of list case
      if (it == end())
      {
         // update pTail as appropriate
         pTail->pNext = pNew;
         pNew->pPrev = pTail;
         pTail = pNew;

         // update the iterator
         it = pNew;
      }
      // otherwise we have a valid iterator
      else
      {
         // set pNext and pPrev
         pNew->pPrev = it.p->pPrev;
         pNew->pNext = it.p;

         // update who pNext and pPrev point to
         if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
         else
            pHead = pNew;
         if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
         else
            pTail = pNew;

         it = pNew;
      }
      numElements++;
   }
   catch (...)
   {
      throw "ERROR: unable to allocate a new node for a list";
   }

   return it;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   // swap head
   auto pTemp = lhs.pHead;
   lhs.pHead = rhs.pHead;
   rhs.pHead = pTemp;

   // swap tail
   pTemp = lhs.pTail;
   lhs.pTail = rhs.pTail;
   rhs.pTail = pTemp;

   // swap num elements
   auto numTemp = lhs.numElements;
   lhs.numElements = rhs.numElements;
   rhs.numElements = numTemp;
}


//#endif
}; // namespace custom
