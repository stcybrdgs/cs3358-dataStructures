// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

// ========================================================================

void IntSet::resize(int new_capacity)

// this function creates a new IntSet object with the desired capacity
// and then copies the relevant data from the "this" IntSet into it 
// and then returns the revised object to the calling function
// (the temporary memory allocation is destroyed upon losing scope)
{
   if( new_capacity < used )	    // make sure new_capacity is at least		
      new_capacity = used;          // big enough for current contents

     
   else if( new_capacity == 0 )     // if new_capacity is 0, then
      new_capacity = 1;             // new_capacity = DEFAULT_CAPACITY

    
   IntSet tempSet(new_capacity);    // create a temp IntSet obj w/new cap            
   tempSet.used = used;             // and copy "this" IntSet into it						

   for( int i = 0; i < used; i ++ )   
   {								  
      tempSet.data[i] = data[i];
   }

   *this = tempSet;                 // re: memory management, the stale 
                                    // stale memory for this.data is 
                                    // deleted by overloaded operator=() 
}

// ========================================================================

IntSet::IntSet(int initial_capacity)

// this function is a parameterized constructor for the IntSet class
{
   capacity = initial_capacity;    // DEFAULT_CAPACITY = 1;
   used = 0;                       
   data = new int[capacity];       
}

// ========================================================================

IntSet::IntSet(const IntSet& src)

// this function is a copy constructor for the IntSet class;
{
   used = src.used;  
   capacity = src.capacity;        
   data = new int[capacity];      
   for( int i = 0; i < src.used; i ++ )
   {
      data[i] = src.data[i];
   }
}

// ========================================================================

IntSet::~IntSet()

// this function is the destructor for the IntSet class 
{
   delete [] data;
}

// ========================================================================

IntSet& IntSet::operator=(const IntSet& rhs)

// this function overloads the assignment operator for the IntSet class
// and copies the rhs instance into the lhs instance
{
   if( capacity > 0 )                  // this conditional will be useful   
      delete [] data;                  // if the DEFAULT_CAPACITY is ever 
                                       // changed to 0 by a maintenance
                                       // programmer 
   capacity = rhs.capacity;    
   data = new int[capacity];  
   used = rhs.used;          
    
   for( int i = 0; i < used; i ++ )   
   {
      data[i] = rhs.data[i];
   }

   return *this;               
}

// ========================================================================

int IntSet::size() const

// this function returns the object's current number of relevant data items 
// per the "used" member variable
{
   return used;					
}

// ========================================================================

bool IntSet::isEmpty() const

// this function returns true if the object's data array contains no
// relevant data values and returns false if one or more relevant
// data values exist   
{
   return ( used == 0 );		
}

// ========================================================================

bool IntSet::contains(int anInt) const

// this function iterates through the relevant data values, and 
// if anInt is found, it returns true, else it returns false
{
   if( !isEmpty() ){
      for( int i = 0; i < used; i ++ )	// iterate thru relevant values
	  {
         if( data[i] == anInt )			
         {
            return true;                // return true if anInt == found
         }
      }
   }
    
   return false;                        // return false if anInt != found
}

// ========================================================================

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const

// this function checks to see if IntSet is a subSet of otherIntSet,
// if so, it returns true, else it returns false
{
   // if IntSet is empty, it's automatically a subset, so return true
   if( isEmpty() )              
      return true; 

   // if IntSet size > otherIntSet size, it's not a subset, so return false
   else if( used > otherIntSet.used )   
   {
      return false;
   }

   // else if IntSet size <= otherIntSet size, then confirm that all 
   //    relevant values in IntSet exist in otherIntSet
   else
   {
      int foundCount = 0;           

      int thisNumVals = used;               // outer loop sentinel
      int otherNumVals = otherIntSet.used;  // inner loop sentinel

      for( int thisI = 0; thisI < thisNumVals; thisI ++ )
      {
         for( int otherI = 0 ; otherI < otherNumVals; otherI ++ )
         {
            if( this->data[thisI] == otherIntSet.data[otherI] )
            foundCount ++;
         }
      }
       
      if( foundCount == used )              // if all IntSet vals == found				  
         return true;                       // then return true 				
      else 
      return false;                         // else return false					
   }
}

// ========================================================================

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}

// ========================================================================

IntSet IntSet::unionWith(const IntSet& otherIntSet) const

// this function creates a temporary IntSet object to collect the union of
// relevant data values from IntSet and otherIntSet; the sequence 
// of values is maintained while duplicates are removed; if the union 
// requires more capacity, resize() is called;
{
   int totUnionVals = 0;                  // var to count # vals in union
    
   IntSet tempArray(capacity);            // create tempIntSet object

   int tempIndex = 0;                     // var to mark the index where
                                          // the union values from 
                                          // otherIntSet should begin
                                          // to be inserted

   bool tempFlag = false;                 // this flag used to mark dupes 
								
   
   // use an outer loop to copy IntSet values into tempArray;
   // use an inner loop to count up dupe values so that afterward
   // we can compute # values needed in final union:
   
   for( int i = 0; i < used; i ++ )
   {
      tempArray.data[i] = data[i];
      tempArray.used ++;
      tempIndex ++;

      for( int i2 = 0; i2 < otherIntSet.used; i2 ++ )
      {
         if( data[i] == otherIntSet.data[i2] )
            totUnionVals ++;
      }
   }


   // compute total # of vals needed in final union and resize if needed:

   totUnionVals = ( used + otherIntSet.used ) - totUnionVals;

   if( tempArray.capacity < totUnionVals ) 
      tempArray.resize(totUnionVals);
	
   
   // now insert the content from otherIntSet into tempArray, w/o dupes:

   int otherUsed = otherIntSet.size();    // sentinel for outer loop

   for( int i2 = 0; i2 < otherUsed; i2 ++ )
   {
      for( int i = 0; i < tempArray.used; i ++ )
      {
         if( otherIntSet.data[i2] == tempArray.data[i] )
         {
            tempFlag = true;              // flag the dupes 
         }
      }

      if( !tempFlag )                     // save non-dupes to tempArray
      {	
         tempArray.data[tempIndex] =      
         otherIntSet.data[i2];	
			
         tempArray.used ++;               
         tempIndex ++;                    
                                          
      }   
		
      tempFlag = false;                   // reset tempFlag for next pass
                                        
   }    
          
   return tempArray;   
}

// ========================================================================

IntSet IntSet::intersect(const IntSet& otherIntSet) const

// this function creates a temporary IntSet object to collect
// the intersection of relevant values from IntSet and otherIntSet;
// any values not in the intersection are removed before 
// returning the intersection to the calling function
{
   IntSet tempArray(capacity);      // create a temp IntSet object 
                                    // to hold the intersection vals

   int strikeCount = 0;             // this var counts # times that
                                    // a value in IntSet does not
                                    // match a value in otherIntSet

   for( int i = 0; i < used; i ++ ) // copy IntSet into tempArray
   {
      tempArray.data[i] = data[i];
      tempArray.used ++;
   }
    
   // now, remove the contents of tempArray if they're not contained
   // in the intersection of thisIntSet and otherIntSet:
	
   for( int i = 0; i < used; i ++ )
   {
      for( int i2 = 0; i2 < otherIntSet.used; i2 ++ )
      {
         if( data[i] != otherIntSet.data[i2] )
            strikeCount ++;     
      }

         if( strikeCount == otherIntSet.size() )
         {
            // if strikeCount == otherIntSet.size(), then the
            // IntSet value was not found in otherIntSet, so
            // it's not part of the intersection and should be
            // removed. . . 
			
            assert ( tempArray.remove(data[i]) == true );
            
            strikeCount = 0;        // reset count for the next loop        
         }    
   }       
   return tempArray;                // return the intersection values
}

// ========================================================================

IntSet IntSet::subtract(const IntSet& otherIntSet) const

// this function creates a temporary IntSet object to collect
// only the unique elements among IntSet and otherIntSet (ie, the
// dupes between the two sets are effectively removed from the
// final collection by simply being ignored);
{
   // create temporary array object to hold val unique to this intSet

   IntSet tempArray(capacity);		
   bool tempFlag = false;           // the tempFlag marks dupes
   int tempIndex = 0;               // var to track index of tempArray

   for( int i = 0; i < used; i ++ )
   {
      for( int i2 = 0; i2 < otherIntSet.used; i2 ++ )
      {
         if( data[i] == otherIntSet.data[i2] )
         {
            tempFlag = true;	    // use flag to mark dupes
                                    // to be ignored in the 
                                    // final collection
         }
      }

      // save each unique value to the tempArray:
        
      if( !tempFlag )				
      {	
         tempArray.data[tempIndex] = data[i];	
         tempArray.used ++;         
         tempIndex ++;
      }   
	
      
      tempFlag = false;             // reset flag for next iteration:         
   }    
    
   return tempArray;                // return the IntSet obj
}

// ========================================================================

void IntSet::reset()

// this function simply resets the "used" member variable to 0 to indicate
// that no relevant values exist in the class object
{
   used = 0;                        
}

// ========================================================================

bool IntSet::add(int anInt)

// this function passes the anInt parameter to the contains() function 
// to see if it already exists in the subject object; if not, then 
// the anInt value is appended to the tail of the data[] array;
{
   int nuCapacity = capacity;       // var to hold adjusted capacity

   if ( !contains(anInt) )          // make sure anInt isn't in the IntSet
   { 
      if( (used + 1) > capacity )   // resize IntSet if it's full 	
      {								
         nuCapacity = capacity * 1.5;

         if( nuCapacity == capacity )
            nuCapacity ++;

         resize(nuCapacity);
      }

      data[used] = anInt;           // if anInt doesn't exist in the intSet,
      used ++;                      // then add it to the end of the intSet

      return true;                  // and return true.
   }
   else
      return false;                 // else return false
}

// ========================================================================

bool IntSet::remove(int anInt)

// this function first uses the contains() method to check if the anInt
// parameter exists in the object's data[] array; 
// if not, it returns false;
// if so, then it iterates thru the data[] array until if finds the 
// index of the target value; once the index is found, the following 
// logic applies:
// *   if the index is the ultimate value in the array, then 
//     simply decrement the "used" member variable and return true;
// *   if the index is between indices [0] and [ultimate] inclusive,
//     then decrement the "used" member variable and overwrite the  
//     index by shifting all trailing elements leftward by one index
{
   if( contains(anInt) )                   
   {
      for( int i = 0; i < used; i ++ )    
      {
         if( data[i] == anInt )         // find the index of anInt 
         {
            if( i == ( used - 1 ) )     // if index is [ultimate]
            {
               used --;                 // then decrement "used" 
               return true;             // and return true;
            }   
            else                        
            {
               // else, make a hole at anInt index and backfill by
               // shifting all trailing elements leftward:
                	
               for( int i2 = i; i2 < used; i2 ++ ) 
               {
                  data[i2] = data[i2 + 1];	
               }

               used --;                   
               return true;            
            }
         }
      }
   }
									
   return false;                        // return false if intSet  
                                        // doesn't contain anInt
}

// ========================================================================

bool operator==(const IntSet& is1, const IntSet& is2)

// this function checks to see if is1 == is2 by confirming if:
// *   each object's elements are subsets of one another
// *   each object's data[] array contains the same # elements
{
   if ( is1.size() == is2.size() &&      
        is1.isSubsetOf(is2) &&     
        is2.isSubsetOf(is1) )
      return true;                    
   else
      return false;              
}