#ifndef bst_h_18932492374
#define bst_h_18932492374

/*
 * This is an implementation that does contain a recursive remove method.
 */

#include <memory>
#include <utility>
#include <queue>
#include <stack>
#include <algorithm>
#include <stdlib.h>
#include <initializer_list>
#include "value-type.h"
#include <iostream>  
#include <exception>


template<class Key, class Value> class bstree; // forward declarations of template classes.

template<class Key, class Value> class bstree {

  public:

    // Container typedef's used by STL.
    using key_type   = Key;
    using mapped_type = Value;

    using value_type = __value_type<Key, Value>::value_type;// = std::pair<const Key, Value>;  
    using difference_type = long int;
    using pointer         = value_type*; 
    using reference       = value_type&; 

  private:
   /*
    * The bstree consists of a tree Nodes managed by std::unique_ptr<Node>, and each Node contains left and right children and 
      a pair<const Key, Value>. The pair is declared inside a wrapper class __value_type whose assignment operators provide
      greater convenience. 
    */ 
   class Node {

        friend class bstree<Key, Value>;    

    public:   
        
        Node()
        {
            parent = nullptr;
        }
     
        // The copy constructor 
        Node(const Node& lhs);
        
        /* 
          Do we need constructor or the one below it?

        Node(const Key& key, const Value& value, Node *parent_in=nullptr) : __vt{key, value}, parent{parent_in}
        {
           left = std::make_unique<Node>();    
           right = std::make_unique<Node>(); 

           left->parent = right->parent = this;
        }
        */
        
        Node(const Key& key, const Value& value, Node *parent_in=nullptr) : __vt{key, value}, parent{parent_in}, left{nullptr}, right{nullptr} 
        {
        }
      
        Node& operator=(const Node&) noexcept; 

        Node(Node&&); // ...but we allow move assignment and move construction.

       ~Node() = default; // members __vt, __left and right are all implicitly deleted. 

        std::ostream& print(std::ostream& ostr) const noexcept; 

        std::ostream& debug_print(std::ostream& ostr) const noexcept;

        friend std::ostream& operator<<(std::ostream& ostr, const Node& node) noexcept
        { 
            node.print(ostr);
            return ostr;
        }
        
        Node& operator=(Node&&) noexcept;
        
    private:

        __value_type<Key, Value> __vt;  // Convenience wrapper for std::pair<const Key, Value>
                                        // Has necessary constructors and assignment operators.
                              
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node *parent;

        constexpr const Key& key() const noexcept 
        {
           return __vt.__get_value().first; //  'template<typename _Key, typename _Value> struct __value_type' does not have members first and second.
        } 

        constexpr const Value& value() const noexcept 
        { 
           return __vt.__get_value().second; 
        }  
        
        constexpr Value& value() noexcept 
        { 
           return __vt.__get_value().second; 
        }

    }; 

   class NodeLevelOrderPrinter {
   
      std::ostream& ostr;
      int current_level;
      int height_;
       
      std::ostream& (Node::*pmf)(std::ostream&) const noexcept;

      void display_level(std::ostream& ostr, int level) const noexcept
      {
        ostr << "\n" << "current level = " <<  level << '\n'; 
         
        // Provide some basic spacing to tree appearance.
        /*
        std::size_t num = height_ - level + 1;
      
        std::string str( num, ' ');
      
        ostr << str; 
         */ 
      }
      
      public: 
      
      NodeLevelOrderPrinter (const bstree<Key, Value>& tree, std::ostream& (Node::*pmf_)(std::ostream&) const noexcept, std::ostream& ostr_in):  ostr{ostr_in}, current_level{0}, pmf{pmf_}
      { 
          height_ = tree.height(); 
      }

      NodeLevelOrderPrinter (const NodeLevelOrderPrinter& lhs): ostr{lhs.ostr}, current_level{lhs.current_level}, height_{lhs.height_}, pmf{lhs.pmf} {}
      
      void operator ()(const Node *pnode, int level)
      { 
          // Did current_level change?
          if (current_level != level) { 
         
              current_level = level;
         
              display_level(ostr, level);       
          }
         
          (pnode->*pmf)(std::cout);
         
          std::cout << '\n' << std::flush;
      }
   };

  private: 

    std::unique_ptr<Node> root; 

    int size;

    template<typename Functor> void DoInOrderTraverse(Functor f, const std::unique_ptr<Node>& root) const noexcept;
    template<typename Functor> void DoPostOrderTraverse(Functor f,  const std::unique_ptr<Node>& root) const noexcept;
    template<typename Functor> void DoPreOrderTraverse(Functor f, const std::unique_ptr<Node>& root) const noexcept;

    void copy_tree(const bstree<Key, Value>& lhs) noexcept;

    const Node *min(const Node *current) const noexcept;
   
    const Node *getSuccessor(const Node *current) const noexcept;
   
    const std::unique_ptr<Node>& get_unique_ptr(const Node *pnode) const noexcept;

    std::pair<bool, const Node *> findNode(const key_type& key, const Node *current) const noexcept; 

    int  height(const Node *pnode) const noexcept;
    int  depth(const Node *pnode) const noexcept;
    bool isBalanced(const Node *pnode) const noexcept;

    void move(bstree<Key, Value>&& lhs) noexcept;
    
    Node *find(Key key, const std::unique_ptr<Node>&) const noexcept;

  public:
/*

Some of the std::map insert methods:

    template< class InputIt >
    void insert( InputIt first, InputIt last );
    
    void insert( std::initializer_list<value_type> ilist );
    
    insert_return_type insert(node_type&& nh);
    
    iterator insert(const_iterator hint, node_type&& nh);
    
    void insert( std::initializer_list<value_type> ilist );
    
    insert_return_type insert(node_type&& nh);
    
    iterator insert(const_iterator hint, node_type&& nh);

    template< class InputIt >
    void insert( InputIt first, InputIt last );
*/

    //++std::pair<iterator,bool> insert( const value_type& value );
    //++std::pair<iterator,bool> insert( value_type&& value );
    
/*
 From std::map insert_or_assign methods

    template <class M>
    pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);

    template <class M>
    pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);

    template <class M>
    iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);

    template <class M>
    iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);


*/

    // One other stl typedef.
    using node_type       = Node; 
  
    bstree() noexcept : root{nullptr}, size{0} { }

   ~bstree() noexcept = default; // does post-order like member destruction

    bstree(std::initializer_list<value_type>& list) noexcept; 

    bstree(const bstree&) noexcept; 

    bstree(bstree&& lhs) noexcept
    {
        move(std::move(lhs)); 
    }

    bstree& operator=(const bstree&) noexcept; 

    bstree& operator=(bstree&&) noexcept;

    bstree<Key, Value> clone() const noexcept; 

    bool isEmpty() const noexcept;

    void test_invariant() const noexcept;

    const Value& operator[](Key key) const;

    Value& operator[](Key key);

    void insert(std::initializer_list<value_type>& list) noexcept; 

    void insert(const key_type& key, const mapped_type& value) noexcept
    {
        return insert_or_assign(key, value);
    }

    void insert_or_assign(const key_type& key, const mapped_type& value) noexcept; // TODO: std::pair<cont Key, Value>
  
    // TODO: Add methods that take a pair<const Key, Value>

    Value& operator[](const Key& key) noexcept; 

    const Value& operator[](const Key& key) const noexcept; 

    // TODO: Add emplace() methods and other methods like std::map have, like insert_or_assign().

    bool remove(Key key) noexcept;

    bool find(Key key) const noexcept
    {
       return find(key, root) ? true : false;
    }
    
    // Breadth-first traversal
    template<class Functor> void levelOrderTraverse(Functor f) const noexcept;

    // Depth-first traversals
    template<typename Functor> void inOrderTraverse(Functor f) const noexcept { return DoInOrderTraverse(f, root); }
    template<typename Functor> void preOrderTraverse(Functor f) const noexcept  { return DoPreOrderTraverse(f, root); }
    template<typename Functor> void postOrderTraverse(Functor f) const noexcept { return DoPostOrderTraverse(f, root); }

    void  printlevelOrder(std::ostream& ostr) const noexcept;

    void debug_printlevelOrder(std::ostream& ostr) const noexcept;

    int height() const noexcept;
    bool isBalanced() const noexcept;

    friend std::ostream& operator<<(std::ostream& ostr, const bstree<Key, Value>& tree) noexcept
    {
       tree.printlevelOrder(ostr);  
       return ostr;
    }
};

template<class Key, class Value>
bstree<Key, Value>::Node::Node(const Node& lhs) : __vt{lhs.__vt}, left{nullptr}, right{nullptr}
{
   if (lhs.parent == nullptr) // If lhs is the root, then set parent to nullptr.
       parent = nullptr;

   // The make_unique<Node> calls will in turn recursively invoke the constructor again, resulting in the entire tree rooted at
   // lhs being copied.
   if (lhs.left  != nullptr) { 

       left = std::make_unique<Node>(*lhs.left);    
       left->parent = this;
   }
   
   if (lhs.right != nullptr) {

       right = std::make_unique<Node>(*lhs.right); 
       right->parent = this;
   }
}

template<class Key, class Value> typename bstree<Key, Value>::Node&  bstree<Key, Value>::Node::operator=(const typename bstree<Key, Value>::Node& lhs) noexcept
{
   if (&lhs == this) return *this;

   __vt = lhs.__vt;

   if (lhs.parent == nullptr) // If we are copying a root pointer, then set parent.
       parent = nullptr;

   // The make_unique<Node> calls below results in the entire tree rooted at lhs being copied.
   if (lhs.left  != nullptr) { 

       left = std::make_unique<Node>(*lhs.left);    
       left->parent = this;
   }
   
   if (lhs.right != nullptr) {

       right = std::make_unique<Node>(*lhs.right); 
       right->parent = this;
   }
  
   return *this;
}

template<class Key, class Value> inline bstree<Key, Value>::bstree(std::initializer_list<value_type>& list)  noexcept : bstree()
{
   insert(list);
}

template<class Key, class Value> inline bstree<Key, Value>::bstree(const bstree<Key, Value>& lhs) noexcept
{ 
   root = std::make_unique<Node>(*lhs.root); 
   size = lhs.size;
}

template<class Key, class Value> inline void bstree<Key, Value>::move(bstree<Key, Value>&& lhs) noexcept  
{
  root = std::move(lhs.root); 

  size = lhs.size;

  lhs.size = 0;
}

template<class Key, class Value> bstree<Key, Value>& bstree<Key, Value>::operator=(const bstree<Key, Value>& lhs) noexcept
{
  if (this == &lhs)  {
      
      return *this;
  }

  // This will implicitly delete all Nodes in 'this', and set root to a duplicate tree of Nodes.
  root = std::make_unique<Node>(*lhs.root); 
 
  size = lhs.size; 

  return *this;
}

template<class Key, class Value> bstree<Key, Value>& bstree<Key, Value>::operator=(bstree<Key, Value>&& lhs) noexcept
{
  if (this == &lhs) return *this;
  
  move(std::move(lhs)); 

  return *this;
}

template<class Key, class Value> inline std::ostream& bstree<Key, Value>::Node::print(std::ostream& ostr) const noexcept
{
  ostr << "[ " << key() << ", " << value() << "] " << std::flush;  
  return ostr; 
}

template<class Key, class Value> std::ostream& bstree<Key, Value>::Node::debug_print(std::ostream& ostr) const noexcept
{
   ostr << " {["; 
 
   ostr << key() << ']';

   if (parent) 
      ostr << ", parent->key() =" << parent->key(); 
   else
      ostr << ", parent = nullptr";
 
   if (!left) 
     ostr << ", left = nullptr";
   else
      ostr << ", left->key() = " <<  left->key();
   
   if (!right) 
     ostr << ", right = nullptr";
   else
      ostr << ", right->key() = " << right->key();
   
   ostr << "}";
 
   return ostr;
}


// Breadth-first traversal. Useful for display the tree (with a functor that knows how to pad with spaces based on level).
template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::levelOrderTraverse(Functor f) const noexcept
{
   std::queue< std::pair<const Node*, int> > queue; 

   Node* proot = root.get();

   if (proot == nullptr) return;
      
   auto initial_level = 1; // initial, top root level is 1.
   
   // 1. pair.first  is: const tree<Key, Value>::Node23*, the current node to visit.
   // 2. pair.second is: current level of tree.
   queue.push(std::make_pair(proot, initial_level));

   while (!queue.empty()) {

       /*
        std::pair<const Node *, int> pair_ = queue.front();
        const Node *current = pair_.first;
        int current_level = pair_.second;
       */

        auto[current, current_level] = queue.front(); // C++17 unpacking.

        f(current, current_level);  
        
        if(current->left)
            queue.push(std::make_pair(current->left.get(), current_level + 1));  

        if(current->right)
            queue.push(std::make_pair(current->right.get(), current_level + 1));  

        queue.pop(); 
   }
}

template<typename Key, typename Value> inline void  bstree<Key, Value>::debug_printlevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(*this, &Node::debug_print, ostr);  
  
  levelOrderTraverse(tree_printer);
  
  ostr << std::flush;
}

/*
template<class Key, class Value> bstree<Key, Value>::Node::Node(Key key, const Value& value, Node *ptr2parent)  : parent{ptr2parent}, left{nullptr}, right{nullptr}, \
        __vt{key, value}
{
}
*/
template<class Key, class Value> inline bstree<Key, Value>::Node::Node(Node&& node) : __vt{std::move(node.__vt)}, left{std::move(node.left)}, right{std::move(node.right)}, parent{node.ptr2parent} 
{
}

template<class Key, class Value> inline bool bstree<Key, Value>::isEmpty() const noexcept
{
  return root == nullptr ? true : false;
}

/*
 * Input:  pnode is a raw Node *.
 * Return: A reference to the unique_ptr that manages pnode.
 */
template<class Key, class Value> const std::unique_ptr<typename bstree<Key, Value>::Node>& bstree<Key, Value>::get_unique_ptr(const Node *pnode) const noexcept
{
  if (pnode->parent == nullptr) { // Is pnode the root? 

     return root; 

  } else {

     return (pnode->parent->left.get() == pnode) ? pnode->parent->left : pnode->parent->right;  
  }
}

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::DoInOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (current == nullptr) {

      return;
   }

   DoInOrderTraverse(f, current->left);

   f(current->__get_pair()); 

   DoInOrderTraverse(f, current->right);
}

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::DoPreOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (current == nullptr) {

      return;
   }

   f(current->__get_pair()); 

   DoPreOrderTraverse(f, current->left);

   DoPreOrderTraverse(f, current->right);
}

template<class Key, class Value> template<typename Functor> void bstree<Key, Value>::DoPostOrderTraverse(Functor f, const std::unique_ptr<Node>& current) const noexcept
{
   if (current == nullptr) {

      return;
   }

   DoPostOrderTraverse(f, current->left);

   DoPostOrderTraverse(f, current->right);

   f(current->__get_pair()); 
}

/*
 * Algorithm taken from page 290 of Introduction to Algorithms by Cormen, 3rd Edition, et. al.
 */
template<class Key, class Value> typename bstree<Key, Value>::Node *bstree<Key, Value>::find(Key key, const std::unique_ptr<Node>& current) const noexcept
{
  if (!current || current->key() == key)
     return current.get();
  if (key < current->key())
     return find(key, current->left);
  else return find(key, current->right);
}

/*
 * Returns pair<bool, const Node *>, where
 * If key found, {true, Node * of found node}
 * If key not node found, {false, Node * of leadf node where insert should occur}
*/
template<class Key, class Value> std::pair<bool, const typename bstree<Key, Value>::Node *> bstree<Key, Value>::findNode(const key_type& key, const typename bstree<Key, Value>::Node *current) const noexcept
{
  const Node *parent = nullptr;

  while (current != nullptr) {

     if (current->key() ==  key) return {true, current}; 

      parent = current;

      current = (key < current->key()) ? current->left.get() : current->right.get(); 
  }
  
  return {false, parent}; 
}

template<class Key, class Value> const typename bstree<Key, Value>::Node *bstree<Key, Value>::min(const typename bstree<Key, Value>::Node *current) const noexcept
{
  while (current->left != nullptr) {

       current = current->left;
  } 

  return current;  
}

/*
  If the right subtree of node current is nonempty, then the successor of x is just the left-most node in the right subtree, which is found by calling min(current.right.get()). 
  On the other hand, if the right subtree of node x is empty and x has a successor y, then y is the lowest ancestor of x whose left child is also an ancestor of x.
  Returns: The pointer to successor node or nullptr if there is no successor (because the input node was the largest in the tree)
 
 */
template<class Key, class Value>  const typename bstree<Key, Value>::Node* bstree<Key, Value>::getSuccessor(const typename bstree<Key, Value>::Node *current) const noexcept
{
  if (current->right != nullptr) return min(current->right);

  Node *ancestor = current->parent;

  // find the smallest ancestor of current whose left child is also an ancestor of current (by ascending the ancestor chain until we find the first ancestor that is a left child).
  while(ancestor != nullptr && current == ancestor->right.get()) {

       current = ancestor;

       ancestor = ancestor->parent;
  }
  return ancestor;
}

template<class Key, class Value> void bstree<Key, Value>::insert(std::initializer_list<value_type>& list) noexcept 
{
   for (const auto& [key, value] : list) 

      insert_or_assign(key, value);
}


/*
 Like the procedure find(), insert() begins at the root of the tree and traces a path downward. The pointer x traces the path, and the pointer parent is maintained as the parent of current.
 The while loop causes these two pointers to move down the tree, going left or right depending on the comparison of key[pnode] with key[x], until current is set to nullptr. This nullptr
 occupies the position where we wish to place the input item pnode. 
*/
/*
template<class Key, class Value> void bstree<Key, Value>::insert_or_assign(const key_type& key, const mapped_type& value) noexcept
{
    if (size == 0) { // tree is empty
        
        create_root(key, value);
    
        return; // TODO: return iterator?
    }
    
    if (auto [bFound, pnode] = findNode(key, root.get()); bFound == true) {

         const_cast<Node *>(pnode)->value() = value;        
         
         return; // TODO: Return iterator?

    // else if Not found, insert. pnode is the leaf node that will be the parent of the new node.
    } else {
        
       auto parent = const_cast<Node *>(pnode);
        
       std::unique_ptr<Node> pnew_node = std::make_unique<Node>(key, value, parent); 

       if (pnew_node->key() < parent->key()) 
           parent->left = std::move(pnew_node); 
       else 
           parent->right = std::move(pnew_node);
    }

    ++size;

    // TODO: return iterator ??
}
*/

/*
 * Algorithm from page 294 of Introduction to Alogorithm, 3rd Edition by Cormen, et. al
 *
 */
template<class Key, class Value> void bstree<Key, Value>::insert_or_assign(const key_type& key, const mapped_type& value) noexcept
{
  Node *parent = nullptr;
 
  Node *current = root.get();
 
  // parent will become the parent of the new node. One of its children (that is nullptr) will become the new node. 
  while (current) { 
 
      parent = current;
 
      if (key == current->key()) {

          current->value() = value;
          return;
      }
 
      else if (key < current->key())
            current = current->left.get();
       else current = current->right.get();
  }     
 
  std::unique_ptr<Node> node = std::make_unique<Node>(key, value, parent); 
  
  if (!parent)
     root = std::move(node); // tree was empty
  else if (node->key() < parent->key())
       parent->left = std::move(node);
  else 
       parent->right = std::move(node);  
}

/*
 * See Algorithm on page 295 of Introduction to Alogorithm, 3rd Edition by Cormen, et. al

Deletion
========

The overall strategy for deleting a node z from a binary search tree T has three basic cases but,
as we shall see, one of the cases is a bit tricky.

1. If z has no children, then we simply remove it by modifying its parent to replace z with NIL as its child.

2. If z has just one child, then we elevate that child to take z’s position in the tree
   by modifying z’s parent to replace z by z’s child.

3. If z has two children, then we find z’s successor y—which must be in z’s right subtree—and have y
   take z’s position in the tree. The rest of z’s original right subtree becomes y’s new right subtree,
   and z’s left subtree becomes y’s new left subtree. This case is the tricky one because, as we shall
   see, it matters whether y is z’s right child.

The procedure for deleting a given node z from a binary search tree T takes as arguments pointers to T and z.
It organizes its cases a bit differently from the three cases outlined previously by considering the four
cases shown in Figure 12.4.

1. If z has no left child (part (a) of the figure), then we replace z by its right child, which may or may not
be NIL . When z’s right child is NIL , this case deals with the situation in which z has no children. When z’s
right child is non- NIL , this case handles the situation in which z has just one child, which is its right
child.

2. If z has just one child, which is its left child (part (b) of the figure), then we replace z by its left
   child.

3. Otherwise, z has both a left and a right child. We find z’s successor y, which lies in z’s right subtree
   and has no left child (see Exercise 12.2-5). We want to splice y out of its current location and have it
   replace z in the tree.

    1. If y is z’s right child (part (c)), then we replace z by y, leaving y’s right child alone.

    2. Otherwise, y lies within z’s right subtree but is not z’s right child (part (d)).  In this case, we
       first replace y by its own right child, and then we replace z by y.

In order to move subtrees around within the binary search tree, we define a subroutine T RANSPLANT, which replaces
one subtree as a child of its parent with another subtree. When TRANSPLANT replaces the subtree rooted at node u
with the subtree rooted at node v, node u’s parent becomes node #’s parent, and u’s parent ends up having v as its
appropriate child.

TRANSPLANT (T, u, v)
 if u.p == NIL
     root = v;
 elseif u == u.p.left
      u.p.left = v
 else u.p.right = v
 if v != NIL
    v.p = u.p

Lines 1–2 handle the case in which u is the root of T . Otherwise, u is either a left child or a right child of its
parent. Lines 3–4 take care of updating u.p.left if u is a left child, and line 5 updates u.p.right if u is a right
child. We allow v to be NIL , and lines 6–7 update v.p if v is non-NIL . Note that TRANSPLANT does not attempt to
update v.left and v.right; doing so, or not doing so, is the responsibility of TRANSPLANT ’s caller.
*/
template<class Key, class Value> bool bstree<Key, Value>::remove(Key key) noexcept
{
  Node *pfound = find(key, root);
  
  if (pfound == nullptr) return false;

  // Get the managing unique_ptr<Node> whose underlying raw point is node? 
  std::unique_ptr<Node>& pnode = get_unique_ptr(pfound);

  // There are three cases to consider 
  // case 1: If both children are nullptr, we can simply delete pnode. 
  if (!pnode->left && !pnode->right) 
      
      pnode.reset();    
         
  } else if (pnode->left || pnode->right) { // Case 2:If pnode has just one child, then we elevate that child to take pnode's position in the tree
                                            // by modifying pnode's parent to replace pnode by it's child.

      Node *parent = pnode->parent;
            
      pnode = std::move(node->left);
      
      pnode->parent = parent;

  } else { // (pnode->left && p->right) == true
      /*
       Otherwise, pnode has both a left and a right child. We find pnode's successor y, which lies in pnode's right subtree
       and has no left child. We want to splice y out of its current location and have it replace pnode in the tree. There are
       two cases o consider:
      
       1. If y is pnode's right child, then we replace pnode by y, leaving y’s right child alone.
      
       2. Otherwise, y lies within pnode's right subtree but is not pnode's right child (part (d)). In this case, we first
          replace y by its own right child, and then we replace pnode by y.
      */
      std::unique_ptr<Node>& successor = getSuccessor(pnode);

      pnode->__vt = std::move(successor->__vt);  // move the successor's key and value into pnode. Do not alter pnode's parent or left and right children.

      successor.reset(); // safely delete leaf pnode successor
  }  

  --size; 

  return true; 
}

template<class Key, class Value> bool bstree<Key, Value>::remove(Key key) noexcept
{
  Node *pfound = find(key, root);
  
  if (pfound == nullptr) return false;

  // Getthe managing unique_ptr<Node> whose underlying raw point is pfound. 
  std::unique_ptr<Node>& pnode = get_unique_ptr(pfound);

  // There are three cases to consider 
  // case 1: If both children are nullptr, we can simply delete pnode. 
  if (!pnode->left && !pnode->right) 
      
      pnode.reset();    
         
  } else if (pnode->left && p->right) { // pnode has two children
      /*
       Otherwise, pnode has both a left and a right child. We find pnode's successor y, which lies in pnode's right subtree
       and has no left child. We want to splice y out of its current location and have it replace pnode in the tree. There are
       two cases o consider:
      
       1. If y is pnode's right child, then we replace pnode by y, leaving y’s right child alone.
      
       2. Otherwise, y lies within pnode's right subtree but is not pnode's right child (part (d)). In this case, we first
          replace y by its own right child, and then we replace pnode by y.
      */
      std::unique_ptr<Node>& successor = getSuccessor(pnode);

      pnode->__vt = std::move(successor->__vt);  // move the successor's key and value into pnode. Do not alter pnode's parent or left and right children.

      successor.reset(); // safely delete leaf pnode successor

  } else { 

     // Case 3:  If pnode has just one child, then we elevate that child to take pnode's position in the tree
     // by modifying pnode's parent to replace pnode by it's child.
      std::unique_ptr<Node>& pchild = pnode->left ? pnode->left : pnode->right; // Get the only child.

      //--std::unique_ptr<Node>& pnode_owner = (pnode->parent->left == pnode) ? pnode->parent->left : pnode->parent->right; // Get the owner of pnode. 

      Node *parent = pnode->parent; 
      
      pnode = std::move(pchild); // Will delete pnode's underlying memory and 
      
      pnode->parent = parent;
  }

  --size; 

  return true; 
}
template<class Key, class Value> void bstree<Key, Value>::transplant(std::unique_ptr<Node>& u, std::unique_ptr<Node>& v)
{
/*
TODO:
The code below deals with raw pointers and does not concern itself with freeing memory (I don't think). When you delete a unique_ptr<Node>, you delete not just one Node
but its entire subtree, but in this routine we only want to delete one node.

The question is how to best do this. How do we maintain the intent of this routine when modifying it for unique_ptr<Node>. unique_ptr<> methods:

 Node *pnode.>release(); // relinquishes ownership
 pnode.swap(pother);     // swaps raw pointers
unique_ptr<Node>>
 */
   if (!u->parent)       // u->parent == NIL implies u is the root.
       T->root = v; 
   else if (u == u->parent->left)
      u->parent->left = v; // Correct?  Originally u->parent->left = v;
   else 
      u->parent->right = v; // Correct?  Originally u->parent->right = v;
   if (v)               // u != NIL; 
      v->parent = u->parent 
}

template<class Key, class Value> inline int bstree<Key, Value>::height() const noexcept
{
   return height(root.get());
}

/*
 * Returns -1 is pnode not in tree
 * Returns: 0 for root
 *          1 for level immediately below root
 *          2 for level immediately below level 1
 *          3 for level immediately below level 2
 *          etc. 
 */
template<class Key, class Value> int bstree<Key, Value>::depth(const Node *pnode) const noexcept
{
    if (pnode == nullptr) return -1;

    int depth = 0;
      
    for (const Node *current = root; current != nullptr; ++depth) {

      if (current->key() == pnode->key()) {

          return depth;

      } else if (pnode->key() < current->key()) {

          current = current->left;

      } else {

          current = current->right;
      }
    }

    return -1; // not found
}

template<class Key, class Value> int bstree<Key, Value>::height(const Node* pnode) const noexcept
{
   if (pnode == nullptr) {

       return -1;

   } else {

      return 1 + std::max(height(pnode->left.get()), height(pnode->right.get()));
   }
}
 
template<class Key, class Value> bool bstree<Key, Value>::isBalanced(const Node* pnode) const noexcept
{
   if (pnode == nullptr || findNode(pnode->key(), pnode)) return false; 
       
   int leftHeight = height(pnode->leftChild);

   int rightHeight = height(pnode->rightChild);

   int diff = std::abs(leftHeight - rightHeight);

   return (diff == 1 || diff ==0) ? true : false; // return true is absolute value is 0 or 1.
}

// Visits each Node, testing whether it is balanced. Returns false if any node is not balanced.
template<class Key, class Value> bool bstree<Key, Value>::isBalanced() const noexcept
{
   std::stack<Node> nodes;

   nodes.push(root.get());

   while (!nodes.empty()) {

     const Node *current = nodes.pop();

     if (isBalanced(current) == false)  return false; 

     if (current->rightChild != nullptr) 
         nodes.push(current->rightChild);
 
     if (current->leftChild != nullptr) 
         nodes.push(current->leftChild);
   }

   return true; // All Nodes were balanced.
}

#endif
