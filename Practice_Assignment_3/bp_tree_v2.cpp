#include<bits/stdc++.h>
using namespace std;

int n_i=0;
int n_d=0; 
// BP node
class Node {
    bool IS_LEAF;
    int *key, size;
    int min_size;
    Node** ptr;
    friend class BPTree;
 
public:
    Node(int s, bool);
};
 
// BP tree
class BPTree {
    Node* root;
    int index_node;
    int data_node;
    void insertInternal(int,
                        Node*,
                        Node*);
    Node* findParent(Node*, Node*);
 
public:
    BPTree(int, int);
    void search(int);
    void insert(int);
    void display(Node*);            ////// leaf is node is referred to as data node
    Node* getRoot();

};
 
// Constructor of Node
Node::Node(int s, bool i_l = false)
{
    size=s;
    // min_size=m_s;
    IS_LEAF = i_l;
    if(i_l) n_d++;
    else n_i++;
    key = new int[s];
    ptr = new Node*[s + 1];
}
 
// Initialise the BPTree Node
BPTree::BPTree(int d_s, int i_s)
{
    this->index_node = i_s;
    this->data_node = d_s;
    root = nullptr;
}
 
// Function to find any element
// in B+ Tree
void BPTree::search(int x)
{
 
    // If tree is empty
    if (root == nullptr) {
        cout << "Tree is empty\n";
    }
 
    // Traverse to find the value
    else {
 
        Node* cursor = root;
 
        // Till we reach leaf node
        while (cursor->IS_LEAF == false) {
 
            for (int i = 0;
                 i < cursor->size; i++) {
 
                // If the element to be
                // found is not present
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }
 
                // If reaches end of the
                // cursor node
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
 
        // Traverse the cursor and find
        // the node with value x
        for (int i = 0;
             i < cursor->size; i++) {
 
            // If found then return
            if (cursor->key[i] == x) {
                cout << "Found\n";
                return;
            }
        }
 
        // Else element is not present
        cout << "Not found\n";
    }
}
 
// Function to implement the Insert
// Operation in B+ Tree
void BPTree::insert(int x)
{
 
    // If root is nullptr then return
    // newly created node
    if (root == nullptr) {
        root = new Node(data_node, true);
        root->key[0] = x;
        root->size = 1;
    }
 
    // Traverse the B+ Tree
    else {
        Node* cursor = root;
        Node* parent;
 
        // Till cursor reaches the
        // leaf node
        while (cursor->IS_LEAF
               == false) {
 
            parent = cursor;
 
            for (int i = 0;
                 i < cursor->size;
                 i++) {
 
                // If found the position
                // where we have to insert
                // node
                if (x < cursor->key[i]) {
                    cursor
                        = cursor->ptr[i];
                    break;
                }
 
                // If reaches the end
                if (i == cursor->size - 1) {
                    cursor
                        = cursor->ptr[i + 1];
                    break;
                }
            }
        }
 
        if (cursor->size < data_node) {
            int i = 0;
            while (x > cursor->key[i]
                   && i < cursor->size) {
                i++;
            }
 
            for (int j = cursor->size;
                 j > i; j--) {
                cursor->key[j]
                    = cursor->key[j - 1];
            }
 
            cursor->key[i] = x;
            cursor->size++;
 
            cursor->ptr[cursor->size]
                = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = nullptr;
        }
 
        else {
 
            // Create a newLeaf node
            Node* newLeaf = new Node(data_node, true);
 
            int virtualNode[data_node + 1];
 
            // Update cursor to virtual
            // node created
            for (int i = 0; i < data_node; i++) {
                virtualNode[i]
                    = cursor->key[i];
            }
            int i = 0, j;
 
            // Traverse to find where the new
            // node is to be inserted
            while (x > virtualNode[i]
                   && i < data_node) {
                i++;
            }
 
            // Update the current virtual
            // Node to its previous
            for (int j = data_node + 1;
                 j > i; j--) {
                virtualNode[j]
                    = virtualNode[j - 1];
            }
 
            virtualNode[i] = x;
            newLeaf->IS_LEAF = true;
 
            cursor->size = (data_node + 1) / 2;                 //this has to ebjhrgbseurtbseuiotg
            newLeaf->size
                = data_node + 1 - (data_node + 1) / 2;// tgyht9oierjhtgiorfgihdf
 
            cursor->ptr[cursor->size]
                = newLeaf;
 
            newLeaf->ptr[newLeaf->size]
                = cursor->ptr[data_node];
 
            cursor->ptr[data_node] = nullptr;
 
            // Update the current virtual
            // Node's key to its previous
            for (i = 0;
                 i < cursor->size; i++) {
                cursor->key[i]
                    = virtualNode[i];
            }
 
            // Update the newLeaf key to
            // virtual Node
            for (i = 0, j = cursor->size;
                 i < newLeaf->size;
                 i++, j++) {
                newLeaf->key[i]
                    = virtualNode[j];
            }
 
            // If cursor is the root node
            if (cursor == root) {
 
                // Create a new Node
                Node* newRoot = new Node(index_node);
 
                // Update rest field of
                // B+ Tree Node
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
            }
            else {
 
                // Recursive Call for
                // insert in internal
                insertInternal(newLeaf->key[0],
                               parent,
                               newLeaf);
            }
        }
    }
}
 
// Function to implement the Insert
// Internal Operation in B+ Tree
void BPTree::insertInternal(int x,
                            Node* cursor,
                            Node* child)
{
 
    // If we doesn't have overflow
    if (cursor->size < index_node) {
        int i = 0;
 
        // Traverse the child node
        // for current cursor node
        while (x > cursor->key[i]
               && i < cursor->size) {
            i++;
        }
 
        // Traverse the cursor node
        // and update the current key
        // to its previous node key
        for (int j = cursor->size;
             j > i; j--) {
 
            cursor->key[j]
                = cursor->key[j - 1];
        }
 
        // Traverse the cursor node
        // and update the current ptr
        // to its previous node ptr
        for (int j = cursor->size + 1;
             j > i + 1; j--) {
            cursor->ptr[j]
                = cursor->ptr[j - 1];
        }
 
        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }
 
    // For overflow, break the node
    else {
 
        // For new Interval
        Node* newInternal = new Node(index_node);
        int virtualKey[index_node + 1];
        Node* virtualPtr[index_node + 2];
 
        // Insert the current list key
        // of cursor node to virtualKey
        for (int i = 0; i < index_node; i++) {
            virtualKey[i] = cursor->key[i];
        }
 
        // Insert the current list ptr
        // of cursor node to virtualPtr
        for (int i = 0; i < index_node + 1; i++) {
            virtualPtr[i] = cursor->ptr[i];
        }
 
        int i = 0, j;
 
        // Traverse to find where the new
        // node is to be inserted
        while (x > virtualKey[i]
               && i < index_node) {
            i++;
        }
 
        // Traverse the virtualKey node
        // and update the current key
        // to its previous node key
        for (int j = index_node + 1;
             j > i; j--) {
 
            virtualKey[j]
                = virtualKey[j - 1];
        }
 
        virtualKey[i] = x;
 
        // Traverse the virtualKey node
        // and update the current ptr
        // to its previous node ptr
        for (int j = index_node + 2;
             j > i + 1; j--) {
            virtualPtr[j]
                = virtualPtr[j - 1];
        }
 
        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;
 
        cursor->size
            = (index_node + 1) / 2;
 
        newInternal->size
            = index_node - (index_node + 1) / 2;
 
        // Insert new node as an
        // internal node
        for (i = 0, j = cursor->size + 1;
             i < newInternal->size;
             i++, j++) {
 
            newInternal->key[i]
                = virtualKey[j];
        }
 
        for (i = 0, j = cursor->size + 1;
             i < newInternal->size + 1;
             i++, j++) {
 
            newInternal->ptr[i]
                = virtualPtr[j];
        }
 
        // If cursor is the root node
        if (cursor == root) {
 
            // Create a new root node
            Node* newRoot = new Node(index_node);
 
            // Update key value
            newRoot->key[0]
                = cursor->key[cursor->size];
 
            // Update rest field of
            // B+ Tree Node
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        }
 
        else {
 
            // Recursive Call to insert
            // the data
            insertInternal(cursor->key[cursor->size],
                           findParent(root,
                                      cursor),
                           newInternal);
        }
    }
}
 
// Function to find the parent node
Node* BPTree::findParent(Node* cursor,
                         Node* child)
{
    Node* parent;
 
    // If cursor reaches the end of Tree
    if (cursor->IS_LEAF
        || (cursor->ptr[0])->IS_LEAF) {
        return nullptr;
    }
 
    // Traverse the current node with
    // all its child
    for (int i = 0;
         i < cursor->size + 1; i++) {
 
        // Update the parent for the
        // child Node
        if (cursor->ptr[i] == child) {
            parent = cursor;
            return parent;
        }
 
        // Else recursively traverse to
        // find child node
        else {
            parent
                = findParent(cursor->ptr[i],
                             child);
 
            // If parent is found, then
            // return that parent node
            if (parent != nullptr)
                return parent;
        }
    }
 
    // Return parent node
    return parent;
}
 
// Function to get the root Node
Node* BPTree::getRoot()
{
    return root;
}

void BPTree::display(Node* node){
    cout<<"Number if index_node: "<<n_i<<endl;
    cout<<"Number if data_node: "<<n_d<<endl;
    for(int i=0;i<root->size;i++){
        cout<<root->key[i]<<endl;
    }
}
 
// Driver Code
int main()
{
    int i,d;
    i=3,d=4;
    BPTree* node = new BPTree(i,d);
 
    // Create B+ Tree
    node->insert(100);
    node->insert(200);
    node->insert(300);
    node->insert(400);
    node->insert(500);
    node->insert(600);
    node->insert(700);
    node->insert(800);
    node->insert(150);
    node->insert(350);
    node->insert(250);

 
    // Function Call to search node
    // with value 16
    node->search(16);
    node->display(node->getRoot());
 
    return 0;
}
