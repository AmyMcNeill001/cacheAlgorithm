//NOTE: this code uses X=5 for a cache that stores 5 webpages

#include <iostream>
using namespace std;

struct Node
{
    //for simplicity, this integer will represent the URL
    int URL;
    //this char will represent the webpage
    char page;
    //this integer will determine where in the hash table the page should go
    int hash;
    
    //these pointers keeps an ordering within the hash table
    Node* next;
    Node* previous;
    Node();
    Node(int myURL, char myPage, Node* myNext, Node* myPrevious);
    
    //overloaded equality operator for update function (in cache class)
    bool operator==(Node rhsObj);
    //overloaded assignment operator for update function (in cache class)
    Node operator=(Node rhsObj);
};

//this default constructor doesn’t matter
Node::Node()
{
    URL = -1;
    page = '0';
    hash = -1;
    next = NULL;
    previous = NULL;
}

Node::Node(int myURL, char myPage, Node* myNext, Node* myPrevious)
{
    URL = myURL;
    page = myPage;
    hash = URL % 5;
    next = myNext;
    previous = myPrevious;
}

bool Node::operator==(Node rhsObj)
{
    //NOTE: pointers are not compared because input will have a NULL pointer
    if( (this->URL == rhsObj.URL) && (this->page == rhsObj.page))
        return true;
    else
        return false;
}

Node Node::operator=(Node rhsObj)
{
    this->page = rhsObj.page;
    this->URL = rhsObj.URL;
    this->next = rhsObj.next;
    this->previous = rhsObj.previous;
    
    return *this;
}

class cache
{
public:
    //this is my hash table of nodes
    Node hashtable[4];
    //these are my pointers to keep track of the most recent and least recently accessed page
    Node* oldest;
    Node* recent;
    
    cache();
    //this function is just to insert the first few URL/web page pairs
    void insert(int pmURL, char pmPage);
    //this function does the updating as the user looks at new pages
    void update(int pmURL, char pmPage);
};

//this default constructor doesn’t matter
cache::cache()
{
    oldest = NULL;
    recent = NULL;
}

void cache::insert(int pmURL, char pmPage)
{
    if( oldest == NULL)
    {
        Node newNode(pmURL, pmPage, NULL, NULL);
        oldest = &newNode;
        hashtable[newNode.hash] = newNode;
        recent = oldest;
    }//END if statement for empty hashtable
    
    else if (oldest == recent)
    {
        cout << "Is this ever executing?" << endl;
        Node newNode(pmURL, pmPage, NULL, NULL);
        recent = &newNode;
        hashtable[newNode.hash] = newNode;
        oldest->next = recent;
        recent->previous = oldest;
    }//END else if statement for hash table with only one “node”
    
    else if (oldest->next == recent)
    {
        Node newNode(pmURL, pmPage, NULL, NULL);
        recent->next = &newNode;
        hashtable[newNode.hash] = newNode;
        newNode.previous = recent;
        recent = recent->next;
    }//END else if statement for two node cache
    
    else
    {
        Node* temp = oldest;
        
        Node newNode(pmURL, pmPage, NULL, NULL);
        hashtable[newNode.hash] = newNode;
        
        temp->next = &newNode;
        recent = &newNode;
        recent->previous = temp;
    }//END else statement for hash table with more than one “node”
}//END insert function definition

void cache::update(int pmURL, char pmPage)
{
    //create a Node with the user’s most recently loaded page
    Node testNode(pmURL, pmPage, NULL, NULL);
    
    if( hashtable[testNode.hash] == testNode)
    {
        hashtable[testNode.hash] = testNode;
        Node* temp = &testNode;
        recent->next = temp;
        testNode.previous = recent;
        recent = temp;
    }//END if statement for if the page visited is already stored in the cache
    
    else
    {
        hashtable[testNode.hash] = testNode;
        recent->next = &testNode;
        testNode.previous = recent;
        recent = &testNode;
    }//END else statement for page visited was not in the cache
    
    //ditch the last node
    Node* deleteMe = oldest;
    oldest = oldest->next;
    oldest->previous = NULL;
    (*deleteMe) = Node();
    
}//END update function definition

int main()
{
    //initialize a cache object
    cache myCache;
    
    //populate the object
    myCache.insert(1, 'a');
    myCache.insert(2, 'b');
    myCache.insert(3, 'c');
    myCache.insert(4, 'd');
    myCache.insert(5, 'e');
    
    //simulate a person using the browser and viewing new pages
    myCache.update(9, 'f');
    myCache.update(2, 'b');
    myCache.update(6, 'g');
    
}//END function main


