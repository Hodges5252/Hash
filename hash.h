/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil

namespace custom
{
    /************************************************
     * UNORDERED SET
     * A set implemented as a hash
     ************************************************/
    template <typename T>
    class unordered_set
    {
    public:
        //
        // Construct
        //
        unordered_set()
            : buckets(), numElements()
        {
        }
        unordered_set(unordered_set& rhs)
            : numElements(rhs.numElements)
        {
            for (int i = 0; i < 10; i++) // since this is an occupied array we need to manually initialize
                buckets[i] = rhs.buckets[i];
        }
        unordered_set(unordered_set&& rhs)
            : buckets(std::move(rhs.buckets)), numElements(std::move(rhs.numElements))
        {
        }
        template <class Iterator>
        unordered_set(Iterator first, Iterator last)
        {
            for(first; first != last; first++)
            {
                insert(*first);
            }
        }

        //
        // Assign
        //
        unordered_set& operator=(unordered_set& rhs)
        {
            for (int i = 0; i < 10; i++)        
                buckets[i] = rhs.buckets[i];
            numElements = rhs.numElements ;
            return *this;
        }
        unordered_set& operator=(unordered_set&& rhs)
        {
            for (int i = 0; i < 10; i++)
                buckets[i] = rhs.buckets[i];
            numElements = (std::move(rhs.numElements));
            return *this;
        }
        unordered_set& operator=(const std::initializer_list<T>& il)
        {
            return *this;
        }
        void swap(unordered_set& rhs)
        {
            custom::list<T> tempBuckets[10];
            for (int i = 0; i < 10; i++)
                tempBuckets[i] = buckets[i];
            for (int j = 0; j < 10; j++)
                buckets[j] = rhs.buckets[j];
            for (int k = 0; k < 10; k++)
                rhs.buckets[k] = tempBuckets[k];

            // this isn't as streamlined as using list, but list is rather constraining
            size_t temp = numElements;
            numElements = rhs.numElements;
            rhs.numElements = temp;
        }

        // 
        // Iterator
        //
        class iterator;
        class local_iterator;
        iterator begin() // all buckets
        {
            for (int i = 0; i < 10; i++)
            {
                if (!buckets[i].empty())
                {
                    return iterator(buckets + i, buckets + 10, buckets[i].begin());
                }
            }
            return iterator();
        }
        iterator end()//all buckets
        {
            return iterator(buckets + 10, buckets + 10, buckets[10].end());
        }
        local_iterator begin(size_t iBucket)//one bucket
        {
            return local_iterator(buckets[iBucket].begin());
        }
        local_iterator end(size_t iBucket)//one bucket
        {
            return local_iterator(buckets[iBucket].end());
        }

        //
        // Access
        //
        size_t bucket(const T& t) //returns index of bucket containing T
        {
            return numElements = t;
        }
        iterator find(const T& t);

        //   
        // Insert
        //
        custom::pair<iterator, bool> insert(const T& t);
        void insert(const std::initializer_list<T>& il);


        // 
        // Remove
        //
        void clear() noexcept
        {
            for (int i = 0; i < 10; i++)
            {
                buckets[i].clear();
            }
            numElements = 0;
        }
        iterator erase(const T& t);

        //
        // Status
        //
        size_t size() const
        {
            return numElements;
        }
        bool empty() const
        {
            if (numElements <= 0)
            {
                return true;
            }
            else
                return false;
        }
        size_t bucket_count() const //returns 10
        {
            return 10;
        }
        size_t bucket_size(size_t i) const //returns the size of a bucket
        {
            return buckets[i].size();
        }


#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        custom::list<T> buckets[10];   // exactly 10 buckets
        size_t numElements;                // number of elements in the Hash
    };


    /************************************************
     * UNORDERED SET ITERATOR
     * Iterator for an unordered set
     ************************************************/
    template <typename T>
    class unordered_set <T> ::iterator
    {
    public:
        // 
        // Construct
        //
        iterator()
            : pBucket(), pBucketEnd(), itList()
        {
        }
        iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList)
            : pBucket(pBucket), pBucketEnd(pBucketEnd), itList(itList)
        {
        }
        iterator(const iterator& rhs)
            : pBucket(rhs.pBucket), pBucketEnd(rhs.pBucketEnd), itList(rhs.itList)
        {
        }

        //
        // Assign
        //
        iterator& operator = (const iterator& rhs)//all three
        {
            pBucket = rhs.pBucket;
            pBucketEnd = rhs.pBucketEnd;
            itList = rhs.itList;
            return *this;
        }

        //
        // Compare
        //
        bool operator != (const iterator& rhs) const //one OR other OR other
        {
            return (pBucket != rhs.pBucket || pBucketEnd != rhs.pBucketEnd || itList != rhs.itList);
        }
        bool operator == (const iterator& rhs) const //one AND other AND other
        {
            return (pBucket == rhs.pBucket && pBucketEnd == rhs.pBucketEnd && itList == rhs.itList);
        }

        // 
        // Access
        //
        T& operator * ()//ust itlist
        {
            return *itList;
        }

        //
        // Arithmetic
        //
        iterator& operator ++ ();
        iterator operator ++ (int postfix)
        {
            if (itList++ == nullptr)
            {
                if (pBucket != pBucketEnd)
                {
                    pBucket++;
                }
            }
            return *this;
        }

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif
        custom::list<T>* pBucket; //current bucket (iterates up through each one)
        custom::list<T>* pBucketEnd;//last bucket (for asserts)
        typename list<T>::iterator itList; // use this for stuff
    };


    /************************************************
     * UNORDERED SET LOCAL ITERATOR
     * Iterator for a single bucket in an unordered set
     ************************************************/
    template <typename T>
    class unordered_set <T> ::local_iterator
    {
    public:
        // 
        // Construct
        //
        local_iterator()
            :itList()
        {
        }
        local_iterator(const typename custom::list<T>::iterator& itList)
            :itList(itList)
        {
        }
        local_iterator(const local_iterator& rhs)
            :itList(rhs.itList)
        {
        }

        //
        // Assign
        //
        local_iterator& operator = (const local_iterator& rhs)
        {
            itList = rhs.itList;
            return *this;
        }

        // 
        // Compare
        //
        bool operator != (const local_iterator& rhs) const
        {
            return rhs.itList != itList;
        }
        bool operator == (const local_iterator& rhs) const
        {
            return rhs.itList == itList;
        }

        // 
        // Access
        //
        T& operator * ()
        {
            return *itList;
        }

        // 
        // Arithmetic
        //
        local_iterator& operator ++ ()
        {
            itList++;
            return *this;
        }
        local_iterator operator ++ (int postfix)
        {
            iterator itReturn = *this;
            ++(*this);
            return itReturn;
        }

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif
        typename list<T>::iterator itList;
    };


    /*****************************************
     * UNORDERED SET :: ERASE
     * Remove one element from the unordered set
     ****************************************/
    template <typename T>
    typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
    {
        return iterator();
    }

    /*****************************************
     * UNORDERED SET :: INSERT
     * Insert one element into the hash
     ****************************************/
    template <typename T>
    custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
    {
        return custom::pair<custom::unordered_set<T>::iterator, bool>(iterator(), true);
    }
    template <typename T>
    void unordered_set<T>::insert(const std::initializer_list<T>& il)
    {
    }

    /*****************************************
     * UNORDERED SET :: FIND
     * Find an element in an unordered set
     ****************************************/
    template <typename T>
    typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
    {
        return iterator();
    }

    /*****************************************
     * UNORDERED SET :: ITERATOR :: INCREMENT
     * Advance by one element in an unordered set
     ****************************************/
    template <typename T>
    typename unordered_set <T> ::iterator& unordered_set<T>::iterator::operator ++ ()
    {
        if (itList.p != nullptr)
        {
            itList++;
        }
        if (itList.p == nullptr && (pBucket != pBucketEnd))
        {
            pBucket++;
        }
        return *this;
    }

    /*****************************************
     * SWAP
     * Stand-alone unordered set swap
     ****************************************/
    template <typename T>
    void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
    {
        std::swap(lhs.numElements, rhs.numElements);
        std::swap(lhs.buckets, rhs.buckets);
    }
}
