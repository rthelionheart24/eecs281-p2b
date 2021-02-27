// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR>
{
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node
    {
    public:
        explicit Node(const TYPE &val)
            : elt{val}, child{nullptr}, sibling{nullptr}, parent{nullptr}
        {
        }

        // Description: Allows access to the element at that Node's position.
        // There are two versions, getElt() and a dereference operator, use
        // whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data members of this
        // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
        // statement in PairingPQ's add_node() function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child;
        Node *sibling;
        Node *parent;
    }; // Node

    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
        num_nodes = 0;
        root = nullptr;
    } // PairingPQ()

    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
        // TODO
        num_nodes = 0;
        root = nullptr;

        for (InputIterator i = start; i < end; i++)
        {
            push(*i);
        }

    } // PairingPQ()

    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) : BaseClass{other.compare}
    {

        // TODO
        num_nodes = 0;
        root = nullptr;

        std::deque<Node *> to_be_inserted;
        to_be_inserted.push_back(other.root);

        while (!to_be_inserted.empty())
        {
            Node *next = to_be_inserted.front();
            if (next->child)
                to_be_inserted.push_back(next->child);
            if (next->sibling)
                to_be_inserted.push_back(next->sibling);

            push(next->elt);
            to_be_inserted.pop_front();
        }

    } // PairingPQ()

    // Description: Copy assignment operator.
    // Runtime: O(n)

    PairingPQ &operator=(const PairingPQ &rhs)
    {
        // TODO
        PairingPQ<TYPE> temp(rhs);

        std::swap(num_nodes, temp.num_nodes);
        std::swap(root, temp.root);

        return *this;
    } // operator=()

    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ()
    {
        // TODO
        if (empty())
        {
        }
        else
        {
            std::deque<Node *> to_be_deleted;
            to_be_deleted.push_back(root);

            while (!to_be_deleted.empty())
            {
                Node *next = to_be_deleted.front();
                if (next->child)
                    to_be_deleted.push_back(next->child);
                if (next->sibling)
                    to_be_deleted.push_back(next->sibling);

                delete next;
                next = nullptr;
                to_be_deleted.pop_front();
                num_nodes--;
            }
        }
    } // ~PairingPQ()

    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    //              You CANNOT delete 'old' nodes and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities()
    {
        // TODO

        std::deque<Node *> to_be_updated;
        to_be_updated.push_back(root);

        while (!to_be_updated.empty())
        {
            Node *next = to_be_updated.front();
            if (next->child)
                to_be_updated.push_back(next->child);
            if (next->sibling)
                to_be_updated.push_back(next->sibling);

            next->parent = nullptr;
            next->sibling = nullptr;
            next->child = nullptr;

            root = meld(next, root);
            to_be_updated.pop_front();
        }

    } // updatePriorities()

    // Description: Add a new element to the priority_queue. This is already done.
    //              You should implement push functionality entirely in the addNode()
    //              function, and this function calls addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val)
    {
        addNode(val);
    } // push()

    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop()
    {
        if (empty())
        {
        }
        else if (size() == 1)
        {
            delete root;
            root = nullptr;
            num_nodes = 0;
        }
        else
        {

            Node *head = root->child;
            delete root;
            root = nullptr;

            std::deque<Node *> to_be_melded;

            while (head)
            {
                to_be_melded.push_back(head);
                head->parent = nullptr;
                head = head->sibling;
            }

            while (to_be_melded.size() > 1)
            {
                Node *p1 = to_be_melded[0], *p2 = to_be_melded[1];

                p1->sibling = nullptr;
                p2->sibling = nullptr;

                Node *melded = meld(p1, p2);

                to_be_melded.push_back(melded);
                to_be_melded.pop_front();
                to_be_melded.pop_front();
            }

            root = to_be_melded.front();
            num_nodes--;
        }

    } // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const
    {
        return root->elt;
    } // top()

    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const
    {
        return num_nodes;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const
    {
        return root == nullptr;
    } // empty()

    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value)
    {
        // TODO
        node->elt = new_value;

        if (node == root)
            return;

        Node *eldest = node->parent->child;

        if (eldest == node)
        {
            node->parent->child = node->sibling;
            node->parent = nullptr;
            node->sibling = nullptr;
        }
        else
        {
            Node *temp = eldest;
            while (temp->sibling != node)
            {
                temp = temp->sibling;
            }

            temp->sibling = node->sibling;
            node->parent = nullptr;
            node->sibling = nullptr;
        }

        root = meld(node, root);
    } // updateElt()

    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node *addNode(const TYPE &val)
    {
        // TODO

        if (!root)
        {
            root = new Node(val);
            num_nodes = 1;
            return root;
        }

        Node *n = new Node(val);
        root = meld(n, root);
        num_nodes++;
        return n;

    } // addNode()

private:
    // TODO: Add any additional member functions or data you require here.

    Node *root;
    int num_nodes;

    Node *meld(Node *p1, Node *r)
    {
        if (this->compare(p1->elt, r->elt))
        {
            p1->parent = r;
            if (r->child)
            {
                p1->sibling = r->child;
            }
            r->child = p1;
            return r;
        }
        else if (this->compare(r->elt, p1->elt))
        {
            r->parent = p1;
            if (p1->child)
            {
                r->sibling = p1->child;
                p1->child = r;
            }
            else
                p1->child = r;

            return p1;
        }
        return r;
    }
};

#endif // PAIRINGPQ_H
