// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include <algorithm>
#include <utility>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR>
{
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
    } // BinaryPQ

    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass{comp}, data{start, end}
    {
        updatePriorities();

    } // BinaryPQ

    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ()
    {
    } // ~BinaryPQ()

    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities()
    {
        std::size_t i = size() - 1;
        while (i > 0)
        {

            fixDown(i);
            i--;
        }
        fixDown(0);

    } // updatePriorities()

    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val)
    {
        data.push_back(val);
        fixUp(size() - 1);
    } // push()

    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop()
    {
        data[0] = data.back();
        data.pop_back();
        if (!empty())
            fixDown(0);
    } // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const
    {
        return data[0];

    } // top()

    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const
    {
        return data.size();
    } // size()

    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const
    {
        return (data.size() == 0);
    } // empty()

private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().

    void fixUp(std::size_t k)
    {
        while (k > 0 && this->compare(data[(k - 1) / 2], data[k]))
        {
            std::swap(data[(k - 1) / 2], data[k]);
            k = (k - 1) / 2;
        }
    }

    void fixDown(std::size_t k)
    {

        while (2 * k + 1 <= size() - 1)
        {
            std::size_t j = 2 * k + 1;
            if (j < size() - 1 && this->compare(data[j], data[j + 1]))
                j++;
            if (this->compare(data[j], data[k]))
                break;

            std::swap(data[k], data[j]);
            k = j;
        }
    }

}; // BinaryPQ

#endif // BINARYPQ_H
