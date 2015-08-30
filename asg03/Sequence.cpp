// FILE: Sequence.cpp
// CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
// INVARIANT for the sequence ADT:
//   1. The number of items in the sequence is in the member variable
//      used;
//   2. The actual items of the sequence are stored in a partially
//      filled array. The array is a dynamic array, pointed to by
//      the member variable data. For an empty sequence, we do not
//      care what is stored in any of data; for a non-empty sequence
//      the items in the sequence are stored in data[0] through
//      data[used-1], and we don't care what's in the rest of data.
//   3. The size of the dynamic array is in the member variable
//      capacity.
//   4. The index of the current item is in the member variable
//      current_index. If there is no valid current item, then
//      current_index will be set to the same number as used.
//      NOTE: Setting current_index to be the same as used to
//            indicate "no current item exists" is a good choice
//            for at least the following reasons:
//            (a) For a non-empty sequence, used is non-zero and
//                a current_index equal to used indexes an element
//                that is (just) outside the valid range. This
//                gives us a simple and useful way to indicate
//                whether the sequence has a current item or not:
//                a current_index in the valid range indicates
//                that there's a current item, and a current_index
//                outside the valid range indicates otherwise.
//            (b) The rule remains applicable for an empty sequence,
//                where used is zero: there can't be any current
//                item in an empty sequence, so we set current_index
//                to zero (= used), which is (sort of just) outside
//                the valid range (no index is valid in this case).
//            (c) It simplifies the logic for implementing the
//                advance function: when the precondition is met
//                (sequence has a current item), simply incrementing
//                the current_index takes care of fulfilling the
//                postcondition for the function for both of the two
//                possible scenarios (current item is and is not the
//                last item in the sequence).

#include <cassert>
#include "Sequence.h"
#include <iostream>
using namespace std;

namespace CS3358_SSII_2015
{
   // =====================================================================

   sequence::sequence(size_type initial_capacity)
   :used(0), current_index(used), capacity(initial_capacity)

   // this function is a parameterized constructor for the sequence class
   {
      if( initial_capacity < 1 )          // trap invalid initial capacity
		  initial_capacity = 1;

      data = new value_type[capacity]; 
   }

   // =====================================================================
   
   sequence::sequence(const sequence& source)
   :used(source.used), current_index(source.current_index), capacity(source.capacity)

   // this function is a copy constructor for the sequence class;
   {
      data = new value_type[capacity]; 

      size_type i; 
      for( i = 0; i < source.used; i++ )
      {
         data[i] = source.data[i];
      }
   }

   // =====================================================================
   
   sequence::~sequence()
   
   // this function is the destructor for the sequence class
   {
      delete [] data;
   }

   // =====================================================================

   void sequence::resize(size_type new_capacity)

   // this function copies "this" into a temp object of the desired capacity,
   // deletes stale data memory (via the overloaded assignment operator), 
   // and returns the re-sized object reference to the caller
   {
      if( new_capacity < used )           // ensure new_capacity is big                          
         new_capacity = used;             // enough for current contents
      
      if( new_capacity <= 0 )             // ensure pre-condition is met
         new_capacity = 1;                // s.t. new_capacity > 0
   
      
      // create temp object w/new_capacity to hold "this" contents
      sequence tempSeq(new_capacity);
      tempSeq.used = used;
      tempSeq.current_index = current_index;
     
      // copy data from "this" into tempSeq
      size_type i;
      for( i = 0 ; i < used; i++ )
      {
         tempSeq.data[i] = data[i];
      }
	
      // send "this" and tempSeq objects to the operator=() function to
      //    * copy the tempSeq obj back into "this" 
      //    * repoint the "this" data pointer to the new memory store
      //    * delete stale memory
      
      *this = tempSeq;
   }

   // =====================================================================
   
   void sequence::start()

   // this function sets the current_index to the index of the first item
   // in the sequence (if there is no item, then this function still
   // satisfies the condition current_index == used when there's no
   // current item)
   {
      current_index = 0;
   }

   // =====================================================================
   
   void sequence::advance()

   // this function advances the index # of current_index
   {
      if( is_item() )
      {      
         
         if( (current_index+1) == used )  // check if current item is the
         {                                // last one in the sequence; 
            current_index = used;         // if so, there's no longer a 
         }                                // current item
         
         else
            current_index++;              // else the new current item is 
                                          // now the one immediately after 
                                          // the original current item
      }
   }

   // =====================================================================
   
   void sequence::insert(const value_type& entry)

   // this function inserts a user-entered value into the sequence before
   // the current item; if there was no current item, the new entry goes
   // at the front of the sequence; in either case, the entry becomes the
   // new current item
   {
      // resize array if capacity is too small for entry 
      if( (used + 1) > capacity )
      {
         size_type nuCapacity = capacity * 1.25;
                                                
         if( nuCapacity == capacity )
            nuCapacity = capacity + 1;

         resize(nuCapacity);
      }


      // insert the entry value into the array:

      // CASE 1: if there is no current item because the array 
      // is empty, then put the entry in the first index
      // NOTE: in this case, current_index is already 0
      if( used == 0 )                
      {
         data[current_index] = entry;               
         used++;
      }

      // CASE 2: if array is not empty, then insert entry before
      // the current item; if there is no current item, then 
      // insert entry at front of sequence;
      // METHOD: beginning at current item (if there is one) 
      // or at index[0] (if there's no current item), scoot the 
      // elements to the right by one index, then insert the
      // entry value into the hole created by the shift
      // NOTE: the newly inserted item is now the current item
      else                                 
      {
         size_type END;
         if( is_item() )                  // set END = current item
            END = current_index;          // if there is one   
         else
         {
            END = 0;                      // else END = 0 = insertion point 
            current_index = END;				 
		   }
		   
         size_type i;
         for( i = used; i > END; i-- )
         {
            data[i] = data[i-1];
         }
         data[END] = entry;
         used++;
      }
}

   // =====================================================================
   
   void sequence::attach(const value_type& entry)

   // this function inserts a user-entered value into the sequence after
   // the current item; if there was no current item, the new entry goes
   // at the end of the sequence; in either case, the entry becomes the
   // new current item
   {
      // resize array if capacity is too small for entry 
      if( (used + 1) > capacity )
      {
         size_type nuCapacity = capacity * 1.25;
                                                
         if( nuCapacity == capacity )
            nuCapacity = capacity + 1;

         resize(nuCapacity);
      }


      // attach the entry value into the array:

      // CASE 1: if there is no current item because the array 
      // is empty, then put the entry in the first index
      // NOTE: in this case, current_index is already 0
      if( used == 0 )                
      {
         data[current_index] = entry;               
         used++;
      }
      
      // CASE 2: if array is not empty and there is no current
      // item, then insert entry at end of sequence and make
      // it the current item
      else if( !is_item() )
      {
         data[used] = entry;        
         used++;
         current_index = (used-1);
      }

      // CASE 3: if array is not empty and current item exists,
      // then attach entry after the current item and set the
      // attached entry value to be the new current item
      // METHOD: beginning at current item + 1, scoot the 
      // elements to the right by one index, then insert the
      // entry value into the hole created by the shift, then
      // increment current_index
      else                                 
      {
         const size_type START = used;       // new final index 
         size_type END = (current_index+1);  // if there is one   
         size_type here = START;
         size_type i;
         for( i = START; i > END; i-- )
         {
            data[here] = data[here-1];
            here--;
         }
         data[END] = entry;
         used++;
         current_index++;
      }
   }

   // =====================================================================
   
   void sequence::remove_current()

   // this function removes the user-entered value from the sequence and
   // sets the next item to be the current item; if the user-entered value
   // was already at the end of the sequence, then there is no longer a 
   // current value;
   {
      // PRE-CONDITION: is_item returns true	  
      if( is_item() )
      {
         // CASE 1: if the current item is the last one in the sequence,
         // effectively remove it by decrementing used and setting the 
         // current_index = used (i.e., there's no longer a current item)
         if( (current_index + 1) == used )          
         {                                   
            used--;
            current_index = used;
         }
         // CASE 2: the current item is not the last one in the sequence
         else
         {
            size_type i;
            for( i = current_index; i < used; i++ )
            {
               data[i] = data[i+1];
            } 
            used--;
         }
      }
   }

   // =====================================================================
   
   sequence& sequence::operator=(const sequence& source)
      
   // this function overloads the assignment operator for the sequence class
   // and copies the rhs instance into the lhs instance
   {
      if( data == source.data )        // trying to trap self-assessment
         return *this;
               
      if( capacity > 0 )                   
         delete [] data;                   

      capacity = source.capacity;    
      data = new value_type[capacity];  
      used = source.used;
      current_index = source.current_index;      
      size_type i;
    
      for( i = 0; i < used; i++ )   
      {
         data[i] = source.data[i];
      }

      return *this;   
   }

   // =====================================================================

   sequence::size_type sequence::size() const
   {
      return used;
   }

   // =====================================================================
   
   bool sequence::is_item() const
   
   // this function returns true if there is a current item and false 
   // if there is not; (NOTE: the condition of current_index == used 
   // indicates that no current item exists
   {
      return ( current_index != used );
   }

   // =====================================================================
   
   sequence::value_type sequence::current() const
   {
      assert( is_item() );

      return data[current_index];
   }
}

