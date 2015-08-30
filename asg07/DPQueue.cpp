// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_SumII2015_Assign07
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }

   // CONSTRUCTORS AND DESTRUCTOR

   p_queue::p_queue():heap(0), capacity(0), used(0)
   {}

   p_queue::p_queue(const p_queue& src)
   {
      capacity = src.capacity;
      used = src.used;
      heap = new ItemType[capacity];

      size_type i;
      for( i = 0; i < src.used; i++ )
	  {
         heap[i].data = src.heap[i].data;
         heap[i].priority = src.heap[i].priority;
      }
   }

   p_queue::~p_queue()
   {
      delete [] heap;
   }

   // MODIFICATION MEMBER FUNCTIONS
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
      if( capacity > 0 )
         delete [] heap;

      capacity = rhs.capacity;
      heap = new ItemType[capacity];
      used = rhs.used;
      size_type i;

      for( i = 0; i < used; i++ )
	  {
         heap[i].data = rhs.heap[i].data;
	  }
      
      return *this;
   }

   void p_queue::push(const value_type& entry, size_type priority)
   // adds a new item to the queue through the rear
   {		 
      if( (used + 1) > capacity ) 
      {
         size_type nuCapacity = (size_type)(capacity * 1.50);
                                                
         if( nuCapacity == capacity )
            nuCapacity = capacity + 1;

         resize(nuCapacity);
      }

	  // add new entry to the end of the queue where 'used' 
      // is the next empty index in the heap array
	  heap[used].data = entry;
      heap[used].priority = priority;
      used++;

      // if there's only one node, then return (no need to re-heapify)
      if( used <= 1 ) return;      

      // else, there's more than one node, so re-heapify
	  else
	  {
         // re-heapify upward based on priority, starting at 
         // new end node (where index of end node = used - 1)
         size_type childIndex = used-1;
         bool doneFlag = false;
         while( !doneFlag )
         {
            if( childIndex == 0 ||
                heap[childIndex].priority < parent_priority(childIndex) )
               doneFlag = true;
		    else
		    {
               size_type nextIndex = parent_index(childIndex); 
               swap_with_parent(childIndex);
               childIndex = nextIndex;
            }
	     } // end while
	  } // end if
   } // end push

   void p_queue::pop()
   // removes an existing item from the queue through the front
   {
      // assert that pop() is not performed on an empty queue
      assert( size() > 0 );
      
      // if root is already a leaf, decrement 'used' and return
	  if( is_leaf(0) )
      {
         used = used-1;
         return;
	  }

      // replace the root node with the end node
      size_type root = 0,
                endNode = used-1;
      heap[root].data = heap[endNode].data;
      heap[root].priority = heap[endNode].priority;

      used = used - 1;

      // now re-heapify
      size_type parentIndex = root;
	  bool doneFlag = false;
      while( !doneFlag )
	  {
         if ( is_leaf(parentIndex) || 
              heap[parentIndex].priority > big_child_priority(parentIndex) )
            doneFlag = true;
         
		 else
		 {
            size_type nextIndex = big_child_index(parentIndex);
	        swap_with_parent(big_child_index(parentIndex));
            parentIndex = nextIndex;
         }

	  } // end while
   } // end pop()

   // CONSTANT MEMBER FUNCTIONS

   p_queue::size_type p_queue::size() const
   {
      return used; 
   }

   bool p_queue::empty() const
   // return whether used is 0 or not
   {
      return ( used == 0 ); 
   }

   p_queue::value_type p_queue::front() const
   {
      assert( size() > 0 );

      return heap[0].data; 
   }

   // PRIVATE HELPER FUNCTIONS
   void p_queue::resize(size_type new_capacity)
   // Pre:  (none)
   // Post: The size of the dynamic array pointed to by heap (thus
   //       the capacity of the p_queue) has been resized up or down
   //       to new_capacity, but never less than used (to prevent
   //       loss of existing data).
   //       NOTE: All existing items in the p_queue are preserved and
   //             used remains unchanged.
   {
      if( new_capacity < used )
         new_capacity = used;

      if( new_capacity <= 0 )
         new_capacity = 1;

      ItemType *tempHeap;
      tempHeap = new ItemType[new_capacity];
      
      size_type i;
      for( i = 0; i < used; i++ )
	  {
         tempHeap[i] = heap[i];
	  }

      heap = tempHeap;

	  capacity = new_capacity;
   }

   bool p_queue::is_leaf(size_type i) const
   // Pre:  (i < used)
   // Post: If the item at heap[i] has no children, true has been
   //       returned, otherwise false has been returned.
   {
      assert( i < used );   
      // if left child is in array, it will be located at 2i+1;
      // if right child is in array, it will be located at 2i+2;
      // if neither child is in array, then 2i+1 and 2i+2 will each
      // result in an index located outside the bounds of the array
      return( ((2*i)+1 > used-1) && ((2*i)+2 > used-1)  );
   }

   p_queue::size_type
   p_queue::parent_index(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The index of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert( i > 0 && i < used );	  
      
      // the parent is always at [(i-1)/2] 
      return (i-1)/2; 
   }

   p_queue::size_type
   p_queue::parent_priority(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The priority of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert( i > 0 && i < used );

      // the parent is always at [(i-1)/2] 
      return heap[(i-1)/2].priority;
   }

   p_queue::size_type
   p_queue::big_child_index(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The index of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert( !is_leaf(i) );

	  size_type leftChildIndex  = (2*i) + 1,
                rightChildIndex = (2*i) + 2;

      // case: the item at heap[i] has 2 children
      if( leftChildIndex < used && rightChildIndex < used )
	  {
         if( heap[leftChildIndex].priority > heap[rightChildIndex].priority )
            return leftChildIndex;
         else
            return rightChildIndex;
	  }
      // case: the item at heap[i] has only 1 child
	  else
         return leftChildIndex;
   }

   p_queue::size_type 
   p_queue::big_child_priority(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The priority of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert( !is_leaf(i) );  

      return heap[big_child_index(i)].priority;      
   }

   void p_queue::swap_with_parent(size_type i)
   // Pre:  (i > 0) && (i < used)
   // Post: The item at heap[i] has been swapped with its parent.
   {
      assert( i > 0 && i < used );      

      // save child content in temp struct
      ItemType temp;
      temp.data = heap[i].data;
      temp.priority = heap[i].priority;

      // set child content = parent content
      // (parent is always at [(i-1)/2])
	  heap[i].data = heap[(i-1)/2].data;
	  heap[i].priority = heap[(i-1)/2].priority;

      // set parent content = child content 
      // as saved in temp struct
      heap[(i-1)/2].data = temp.data;
	  heap[(i-1)/2].priority = temp.priority;
   }
}// end namespace

