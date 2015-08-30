// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     compile-time array whose size is IntSet::MAX_SIZE;
//     the member variable data references the array.
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
//     from data[used] through data[IntSet::MAX_SIZE - 1].
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

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

IntSet::IntSet()
{
    used = 0;
}

int IntSet::size() const
{
    return used;
}


bool IntSet::isEmpty() const
{
    return ( used == 0 ); 
}

bool IntSet::contains(int anInt) const
{
    if( !isEmpty() ){
        for( int i = 0; i < used; i ++ )
        {
            if( data[i] == anInt )
            {
                return true;
            }
        }
    }
    
	return false;  
}

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
    int foundCount = 0;
    
    if( this->isEmpty() )
        return true;
	else if( this->size() > otherIntSet.size() )
	{
	    return false;
	}
	else
    {
        for( int thisI = 0; thisI < this->size(); thisI ++ )
        {
            for( int otherI = 0 ; otherI < otherIntSet.size(); otherI ++ )
            {
                if( this->data[thisI] == otherIntSet.data[otherI] )
                    foundCount ++;
            }
        }
        
        if( foundCount == this->size() )  
            return true;
        else 
            return false;
	}
}

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
    if (used > 0)
    {
        out << data[0];
        for (int i = 1; i < used; ++i)
            out << "  " << data[i];
    }
}

IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
    assert( ( this->size() + (otherIntSet.subtract(*this)).size() ) <= MAX_SIZE );
    
    IntSet tempArray;
    int tempIndex = 0;
	bool tempFlag = false;

    // copy this IntSet into tempArray
    for( int i = 0; i < this->size(); i ++ )
    {
        tempArray.data[tempIndex] = this->data[i];
	tempArray.used ++;
        tempIndex ++;
    }
    
    // add contents of otherIntSet to tempArray, ignoring dupes
    for( int i2 = 0; i2 < otherIntSet.size(); i2 ++ )
    {
        for( int i = 0; i < this->size(); i ++ )
	{
	    if( otherIntSet.data[i2] == this->data[i] )
	    {
	        tempFlag = true;	// flag data matches (we don't want dupes)
	    }
	}

        if( !tempFlag )				
        {	
	    tempArray.data[tempIndex] =	otherIntSet.data[i2]; // save the unique data
	    tempArray.used ++; // increment used
            tempIndex ++; // increment index
	}   
		
	tempFlag = false; // reset tempFlag for next iteration
    }    
          
    return tempArray;   
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
    IntSet tempArray;	 // create a temp IntSet object 
			 // to hold the intersection values
    int tempIndex = 0;
    int strikeCount = 0; // var to count # times that
			 // a value in IntSet does not
			 // match a value in otherIntSet
    bool removeSuccess;  // flag handle to call remove()

    // copy this IntSet into tempArray
    for( int i = 0; i < this->size(); i ++ )
    {
        tempArray.data[tempIndex] = this->data[i];
	tempArray.used ++;
        tempIndex ++;
    }
    
    // remove the contents of tempArray if they're not contained
    // in the intersection of thisIntSet and otherIntSet
    for( int i = 0; i < this->size(); i ++ )
    {
	for( int i2 = 0; i2 < otherIntSet.size(); i2 ++ )
	{
	    if( this->data[i] != otherIntSet.data[i2] )
	        strikeCount ++;
	}

	if( strikeCount == otherIntSet.size() )
	{
	    // strikeCount == otherIntSet.size(), then the
	    // IntSet value was not found in otherIntSet, so
	    // it's not part of the intersection and should be
	    // removed. . . 
	    removeSuccess = ( tempArray.remove(this->data[i]) ); // remove the value
	}

	if( removeSuccess )  // here, i'm just using var f in a statement
	                     // in order to get rid of a compiler warning; 
	    strikeCount = 0; // reset the strike count for the next iteration
	else
	    strikeCount = 0;
        }    
          
    return tempArray; // return intersection values
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
    // create temporary array object to hold values unique to this intSet
    IntSet tempArray;		
    int tempIndex = 0;
    bool tempFlag = false; // the tempFlag marks dupes

    for( int i = 0; i < this->size(); i ++ )
    {
        for( int i2 = 0; i2 < otherIntSet.size(); i2 ++ )
        {
            if( this->data[i] == otherIntSet.data[i2] )
            {
	        tempFlag = true; // if flag data matches, then true 
            }
	}
        
        if( !tempFlag ) // if tempFlag == false, then there's no dupes, so
        {	
	    tempArray.data[tempIndex] =	this->data[i]; // save the unique data 
	    tempArray.used ++; // increment the used variable
	    tempIndex ++; // increment the element index
	}   
		
        tempFlag = false; // reset the tempFlag for next iteration
    }    
    
    return tempArray; // return the IntSet obj
}

void IntSet::reset()
{
    used = 0; // set the # relevant elements to 0
}

bool IntSet::add(int anInt)
{
    assert( size() < MAX_SIZE );
    
    if ( !contains(anInt) )
    {
        data[used] = anInt; // if anInt doesn't exist in the intSet,
        used ++;            // then add it to the end of the intSet
        return true;        // and return true.
    }
    else
        return false;
}

bool IntSet::remove(int anInt)
{
    if( contains(anInt) )                   // if anInt is in the set,
    {
        for( int i = 0; i < used; i ++ )    // then iterate through the set
        {
            if( data[i] == anInt )          // until index value == anInt is found; 
            {
                if( i == ( used - 1 ) )     // now, if anInt occurs at the end of the intSet,
                {
                    used --;                // we're done, so decrement the used variable
                    return true;            // and return true;
                }   
                else                        // else, make a hole where anInt is located
                {
                    for( int i2 = i; i2 < used; i2 ++ ) // and then backfill the hole by shifting 
                    {
                        data[i2] = data[i2 + 1];        // all trailing relevant values to the left; 
                    }
                    used --;                // now, we're done, so decrement the used variable    
                    return true;            // and return true; 
                }
            }
        }
    }
			
    return false; // if intSet does not contain anInt, return false 
}

bool equal(const IntSet& is1, const IntSet& is2)
{
    if(	is1.size() == is2.size() && // check if sets are same length
        is1.isSubsetOf(is2) &&	    // check if sets are subsets of each other
	is2.isSubsetOf(is1) )
	return true;                // if both conditions hold, return true
    else
        return false;		    // else return false
}


