#include <iostream>
using namespace std;    //I know this isn't great coding practice but it makes things cleaner right now


struct node
{
    int data;           //data point of node
    node *next;         //pointer holding memory address of next node
    node *prev;         //pointer holding memory address of previous node
};


//Numbers are stored in the linked list as Little Endian decimal digits (due to less traversals compared to a big endian system)
class BigNumber 
{
private:
    node *head;         //head of a linked list of digits
    bool isNegative;    //sign bit

public:

    //constructor for BigNumber.  Basically just initializes a number with the value zero
    //TODO: add other constructors with parameters to initialize it to something other than zero
    //      (possible: int, int array of digits, other Linked Lists of digits)
    BigNumber()
    {
        head = NULL;
        insertAtHead(0);
        isNegative = 0;
    }

    //deprecated method... prints the linked list from head to the end
    void printList()
    {
        node *cur = head;
        while (cur != NULL)
        {
            cout << cur->data << " " ;
            cur = cur->next;
        }
    }

    //inserts numToInsert to the front of the linked list
    void insertAtHead(int numToInsert)
    {
        //insertion magic
        node *temp = new node;
        temp->data = numToInsert;
        temp->next = head;
        temp->prev = NULL;
        head=temp;
    }
    
    //inserts numToInsert after insertAt numbers in the linked list
    void insertAt(int numToInsert, int insertAt)  //insertAt is how many nodes to skip over.  0 is head.
    {
        //special case for inserting at the head... just call the correct method in the first place idiot
        if (insertAt == 0)
        {
            insertAtHead(numToInsert);
        }
        else
        {
            node *cur = head;
            int i = 1;
            //traverse while it still exists and we're still counting
            while (cur != NULL && i < insertAt)
            {
                cur = cur->next;
                i++;
            }
            //insertion magic
            node *temp = new node;
            temp->data = numToInsert;
            temp->next = cur->next;
            temp->prev = cur;
            cur->next = temp;
        }
    }

    void deleteAtTail()
    {
        //first check if firstNode is NULL or last node.
        node* cur = head;
        if(cur == NULL)
        {
            //do nothing, we can't delete nothing...
            return;
        }

        if(cur->next == NULL)
        {
            //probably don't want to ever be here for now.  this means our number would no longer exist
            delete cur;
            cur = NULL;
            return;
        }
        //ASSERTION: at this point, we can guarantee that cur->next exists, so no segfault for that

        //since cur->next exists, we can test cur->next->next without screwing up
        //traverse to the end of the linked list (or to the most significant digit)
        while (cur->next && cur->next->next != NULL)
        {
            cur = cur->next;
        }

        //cur is the second last value, cur->next is the last value, we want to delete it
        delete cur->next;
        cur->next = NULL;
    }


//-----------------OPERATOR OVERLOADING BENEATH THIS PART-----------------

    //operator overloading for incrementing with the ++ operator.
    void operator -- ()
    {
        if (!isNegative)
        {
            //first:  add the digits
            head->data--;
            node *cur = head;
            //while the current digit has a carry from the addition, keep going
            if (cur == NULL)
            {
                cout << "Something is broked, our number no longer exists";
            }
            else
            {
                //by the previous if statement, we guarantee that cur != NULL if we get here, so we can use cur->data
                while (cur->data < 0)
                {
                    if (cur->next != NULL)
                    {
                        cur->data = 9;
                        cur->next->data--;
                        if (cur->next->data == 0)
                        {
                            //remove last node
                            //TODO: probably could make this more efficient by writing deletion code right here
                            deleteAtTail();
                            cout <<"we deleted last node\n";
                        }
                        else
                        {
                            cur = cur->next;
                        }
                    }
                    else
                    {
                        //cur->next is null, but cur->data is good
                        if (cur->next == head->next)
                        {
                            //cout << "this should trigger during a zero crossover\n";
                            isNegative = !isNegative;
                            head->data+=2;  //one to balance out the negative, one to actually flip the side, so +=2
                        }
                    }
                        //cout << "no more infinity";
                }
            }
        }
        else
        {
            //the number is negative, so leave sign bit alone and pretend we're adding but with no negative testing
            head->data++;
            node *cur = head;
            //while the current digit has a carry from the addition, keep going
            while (cur->data > 9)
            {
                cur->data = 0;
                //make sure the node we're carrying to exists
                if(cur->next != NULL)
                {
                    cur->next->data++;
                }
                else
                {
                    //create new node with 1 in it for the next place value
                    node* temp = new node;
                    temp->data = 1;
                    temp->next = NULL;
                    temp->prev = cur;
                    cur->next = temp;
                }
                cur = cur->next;
            }
        }
    }

    //operator overloading for BigNum + (int)
    //TODO: Make this more efficient... yikes.
    BigNumber operator + (int numToAdd)
    {
        BigNumber temp = *this;
        for (int i = 0; i < numToAdd; i++)
        {
            ++temp;
        }
        return temp;
    }

    //operator overloading for incrementing with the ++ operator.
    void operator ++ ()
    {
        //first:  add the digits
        if (!isNegative)
        {
            head->data++;
            node *cur = head;
            //while the current digit has a carry from the addition, keep going
            while (cur->data > 9)
            {
                cur->data = 0;
                //make sure the node we're carrying to exists
                if(cur->next != NULL)
                {
                    cur->next->data++;
                }
                else
                {
                    //create new node with 1 in it for the next place value
                    node* temp = new node;
                    temp->data = 1;
                    temp->next = NULL;
                    temp->prev = cur;
                    cur->next = temp;
                }
                cur = cur->next;
            }
        }
        else
        {
            //number is negative, so we really need to "subtract"
            head->data--;
            node *cur = head;
            //while the current digit has a carry from the addition, keep going
            if (cur == NULL)
            {
                cout << "Something is broked, our number no longer exists";
            }
            else
            {
                //by the previous if statement, we guarantee that cur != NULL if we get here, so we can use cur->data
                while (cur->data < 0)
                {
                    if (cur->next != NULL)
                    {
                        cur->data = 9;
                        cur->next->data--;
                        if (cur->next->data == 0)
                        {
                            //remove last node
                            //TODO: probably could make this more efficient by writing deletion code right here
                            deleteAtTail();
                            cout <<"we deleted last node\n";
                        }
                        else
                        {
                            cur = cur->next;
                        }
                    }
                    else
                    {
                        //cur->next is null, but cur->data is good
                        if (cur->next == head->next)
                        {
                            //cout << "this should trigger during a zero crossover\n";
                            isNegative = !isNegative;
                            head->data+=2;  //one to balance out the negative, one to actually flip the side, so +=2
                            //cout << "isNeg is: " << isNegative << '\n';
                        }
                    }
                        //cout << "no more infinity";
                }
            }
        }
    }

    //operator overloading to cout the object (aka, this is the "toString()" method... stupid java)
    //does not work without the keyword "friend"... how nice.
    friend ostream& operator<< (ostream& os, const BigNumber &head)
    {
        if (head.isNegative)
        {
            os << '-';
        }
        node *cur = head.head;
        if (cur == NULL)
        {
            //do something for the top
            //EDIT: I thought there should be code here, but apparently it works fine when empty
        }
        else
        {
            //traverse to the end
            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            //cur->next == NULL at this point so cur->data is the last data point and cur->prev yields the next node

            //go backwards through the linked list and stream each number on the way
            while (cur->prev != NULL)
            {
                os << cur->data;
                cur = cur->prev;
            }
            //cur is now back to head, and the last thing we print is cur->data (aka, the least significant digit)
            os << cur->data << '\n';    //TODO: eventually get rid of this newline character when no longer debugging
        }
        return os;
    }
};

//main is currently for testing purposes
int main()
{
    BigNumber num1 = BigNumber();       //this initializes the number to zero

    
    num1 = num1 + 0;    //offset is what the number starts at
    cout << "num1 is: " << num1;

    // BigNumber num2 = num1 + 5;
    // cout << "num2 is: " << num2;

    --num1;
    cout << "--num1 is now: " << num1;
    --num1;
    cout << "--num1 is now: " << num1;
    --num1;
    cout << "--num1 is now: " << num1;
    ++num1;
    cout << "++num1 is now: " << num1;
    ++num1;
    cout << "++num1 is now: " << num1;
    ++num1;
    cout << "++num1 is now: " << num1;
    ++num1;
    cout << "++num1 is now: " << num1;
    

    return 0;
}
/*
CURRENTLY SUPPORTED MATHEMATICAL OPERATORS:
++ (increment by 1)
+ (int)
<< (used for cout)
-- (decrement by 1)


<deprecated> CURRENTLY SUPPORTED METHODS: <deprecated>
adding one: addOneToList(root)
displaying number: printNumber(root)
*/
