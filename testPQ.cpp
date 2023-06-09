// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to call updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes).
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues, you have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Eecs281PQ.h"
#include "BinaryPQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;

// Used to test a priority queue containing pointers to integers.
struct IntPtrComp
{
    bool operator()(const int *a, const int *b) const
    {
        return *a < *b;
    }
};

// TODO: Make sure that you're using this-compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, and call
// this function from main().
void testHiddenData(const string &pqType)
{
    struct HiddenData
    {
        int data;
    };
    struct HiddenDataComp
    {
        bool operator()(const HiddenData &a, const HiddenData &b) const
        {
            return a.data < b.data;
        }
    };

    cout << "Testing " << pqType << " with hidden data" << endl;

    // TODO: Add code here to actually test with the HiddenData type.

    Eecs281PQ<HiddenData, HiddenDataComp> *pq = nullptr;

    if (pqType == "Unordered")
    {
        pq = new UnorderedPQ<HiddenData, HiddenDataComp>;
    } // if
    else if (pqType == "Sorted")
    {
        pq = new SortedPQ<HiddenData, HiddenDataComp>;
    }
    else if (pqType == "Binary")
    {
        pq = new BinaryPQ<HiddenData, HiddenDataComp>;
    }
    else if (pqType == "Pairing")
    {
        pq = new PairingPQ<HiddenData, HiddenDataComp>;
    }

    pq->push({21});
    pq->push({23});
    assert(pq->size() == 2);
    assert(pq->top().data == 23);

    pq->push({100});
    assert(pq->size() == 3);
    assert(pq->top().data == 100);

    cout << "Hidden data test succeeded" << endl;

    delete pq;

} // testHiddenData()

// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq)
{
    vector<int> data;
    data.reserve(100);
    data.push_back(86);
    data.push_back(1);
    data.push_back(5);
    data.push_back(31);
    data.push_back(5);
    data.push_back(22);
    data.push_back(56);
    data.push_back(12);

    // NOTE: If you add more data to the vector, don't push the pointers
    // until AFTER the vector stops changing size!  Think about why.
    // You can add up to 100 values, or change the reserve if you want more.

    for (size_t i = 0; i < data.size(); ++i)
    {
        pq->push(&data[i]);
    } // for

    // Change the first value (which is pointed to by the pq), and check it.
    data[0] = 10;
    pq->updatePriorities();
    assert(*pq->top() == 56);
    cout << "updatePriority succeeded!\n";
} // testUpdatePrioritiesHelper()

// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType)
{
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;

    if (pqType == "Unordered")
    {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } // if
    else if (pqType == "Sorted")
    {
        pq = new SortedPQ<int *, IntPtrComp>;
    }
    else if (pqType == "Binary")
    {
        pq = new BinaryPQ<int *, IntPtrComp>;
    }
    else if (pqType == "Pairing")
    {
        pq = new PairingPQ<int *, IntPtrComp>;
    }

    if (!pq)
    {
        cout << "Invalid pq pointer; did you forget to create it?" << endl;
        return;
    } // if

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()

// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType)
{
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);

    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());

    // TODO: Add more testing here!

    pq->push(100);
    pq->push(21);
    pq->push(21);
    pq->push(23);
    assert(pq->size() == 4);
    assert(pq->top() == 100);
    pq->pop();
    assert(pq->size() == 3);
    assert(pq->top() == 23);

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()

// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().
//
void testPairing(vector<int> &vec)
{
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> *pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> *pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> *pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);

    pq1->push(3);
    pq2->pop();
    assert(pq1->size() == 3);
    assert(!pq1->empty());
    assert(pq1->top() == 3);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());

    cout << "Basic tests done." << endl;
    // TODO: Add more code to test addNode(), updateElt(), etc.

    //Test push()
    PairingPQ<int> *pq4 = new PairingPQ<int>();
    assert(pq4->empty());
    pq4->push(63);
    assert(pq4->top() == 63);
    pq4->push(100);
    assert(pq4->top() == 100);
    pq4->push(62);
    assert(pq4->top() == 100);
    pq4->push(81);
    pq4->push(2);
    pq4->push(10);
    assert(pq4->top() == 100);
    pq4->pop();
    assert(pq4->top() == 81);
    pq4->pop();
    assert(pq4->top() == 63);
    pq4->pop();
    assert(pq4->top() == 62);
    pq4->pop();
    assert(pq4->top() == 10);
    pq4->pop();
    assert(pq4->top() == 2);
    pq4->pop();
    assert(pq4->empty());

    //Test Copy ctor
    PairingPQ<int> *pq5 = new PairingPQ<int>();
    pq5->push(20);
    pq5->push(43);
    pq5->push(6);
    pq5->push(100);
    pq5->push(30);
    pq5->push(0);
    PairingPQ<int> *pq6 = new PairingPQ<int>(*pq5);

    while (!pq5->empty())
    {
        assert(pq5->top() == pq6->top());
        pq5->pop();
        pq6->pop();
    }

    //Test = operator
    PairingPQ<int> *pq7 = new PairingPQ<int>();
    pq7->push(20);
    pq7->push(43);
    pq7->push(6);
    pq7->push(100);
    pq7->push(30);
    pq7->push(0);
    PairingPQ<int> *pq8 = new PairingPQ<int>();
    *pq8 = *pq7;

    while (!pq7->empty())
    {
        assert(pq7->top() == pq8->top());
        pq7->pop();
        pq8->pop();
    }

    //Test updatePriorities()
    // PairingPQ<int> *pq9 = new PairingPQ<int *>();
    // vector<int> v = {20, 43, 6, 100, 30, 0};

    // for (size_t i = 0; i < v.size(); i++){
    //     pq9->push(&v[i]);

    // for (size_t i = 0; i < v.size(); i++)
    //     v[i] = 100 - v[i];

    // pq9->updatePriorities();

    // assert(*pq9->top() == 100);
    // pq9->pop();
    // assert(*pq9->top() == 94);
    // pq9->pop();
    // assert(*pq9->top() == 80);
    // pq9->pop();
    // assert(*pq9->top() == 70);
    // pq9->pop();
    // assert(*pq9->top() == 57);
    // pq9->pop();
    // assert(*pq9->top() == 0);
    // pq9->pop();
    // assert(pq9->empty());
    //PairingPQ<int>::Node *n = new PairingPQ<int>::Node(5);

    //Test update_elt
    PairingPQ<int> *pq10 = new PairingPQ<int>();
    pq10->push(20);
    pq10->push(43);
    pq10->push(6);
    PairingPQ<int>::Node *n = pq10->addNode(100);
    pq10->updateElt(n, 200);
    assert(pq10->top() == 200);

    n = pq10->addNode(30);
    pq10->updateElt(n, 300);
    assert(pq10->top() == 300);

    n = pq10->addNode(0);
    pq10->updateElt(n, 5);
    assert(pq10->top() == 300);

    cout
        << "Calling destructors" << endl;
    delete pq1;
    delete pq2;
    delete pq3;
    delete pq4;
    delete pq5;
    delete pq6;
    delete pq7;
    delete pq8;
    delete pq10;

    cout << "testPairing() succeeded" << endl;
} // testPairing()

int main()
{
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{"Unordered", "Sorted", "Binary", "Pairing"};
    unsigned int choice;

    cout << "PQ tester" << endl
         << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0)
    {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1)
    {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2)
    {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3)
    {
        pq = new PairingPQ<int>;
    } // else if
    else
    {
        cout << "Unknown container!" << endl
             << endl;
        exit(1);
    } // else

    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);
    testHiddenData(types[choice]);

    if (choice == 3)
    {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
