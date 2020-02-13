#ifndef bstree_h
#define bstree_h
#include <cstddef>
#include <memory>
#include <utility>
#include <iostream>
#include <algorithm>
#include <queue>
#include <initializer_list>

template<typename T> class bstree {

    struct Node{
        T key;
        Node *parent;

        std::unique_ptr<Node> left; 
        std::unique_ptr<Node> right;

        Node();

        Node(const T& x, Node *parent_in = nullptr): key{x}, parent{parent_in}, left{nullptr}, right{nullptr} 
        {
        } 
        Node(const Node& lhs); 
        
        Node& operator=(const Node&) noexcept;
        Node& operator=(Node&&) noexcept;
        
        Node(Node&& lhs) = delete;

        std::ostream& print(std::ostream& ostr) const noexcept
        {
            return ostr << key << ", " << std::flush;
        }
        
        bool isLeaf() const noexcept
        {
           return (!left && !right) ? true : false;
        }

        std::ostream& debug_print(std::ostream& ostr) const noexcept;
    };

   bool remove(const T& x, Node *p) noexcept; 

   bool insert(const T& x, std::unique_ptr<Node>& p) noexcept;

   void move(bstree&& lhs) noexcept
   {
       root = std::move(lhs.root);
       size = lhs.size;
       lhs.size = 0;
   }
   
   template<typename Functor> void in_order(Functor f, const std::unique_ptr<Node>& current) const noexcept; 

   /*
    * reset deletes the Node managed by dest by move-assigning src to dest, which transfers ownership of the raw pointer managed by src to dest.
    * It also reassigns the parent pointer after the move so the tree it is valid.
    */
   void reset(std::unique_ptr<Node>& src, std::unique_ptr<Node>& dest) noexcept
   {
       if (!src)
           
           dest.reset();
           
       else {
           
          Node *parent = dest->parent; 

          // This deletes the Node managed by dest, and transfers ownership of the pointer managed by src to dest.
         
          dest = std::move(src); 
       
          dest->parent = parent; // Set the parent pointer to be the Node that had been the parent of dest (before it was delete immediately above).
       }
   }
 
   std::unique_ptr<Node> root; 
   std::size_t size;

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
      
      NodeLevelOrderPrinter (const bstree<T>& tree, std::ostream& (Node::*pmf_)(std::ostream&) const noexcept, std::ostream& ostr_in):  ostr{ostr_in}, current_level{0}, pmf{pmf_}
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

  std::size_t height(const std::unique_ptr<Node>& node) const noexcept;

 public:

    bstree() : root{nullptr}, size{0}
    {
    } 

   ~bstree() = default;

    bstree(const bstree& lhs) : size{lhs.size}
    {
       // This will invoke Node(const Node&), passing *lhs.root, which will duplicate the entire tree rooted at lhs.root.
       root = std::make_unique<Node>(*lhs.root); 
    }

    bstree(const std::initializer_list<T>& list) noexcept : size{0}
    {
        for (const auto& x : list)
            insert(x);
    }

    bstree(bstree&& lhs)
    {
       move(std::move(lhs));
    }

    bstree& operator=(const bstree& lhs) noexcept; 

    bstree& operator=(bstree&& lhs) noexcept;   

    bool empty() const noexcept
    {
       return (size == 0) ? true : false;
    }
    
    bool insert(const T& x) noexcept;
    
    bool remove(const T& x)
    {
      bool rc = remove(x, root.get()); 
      
      if (rc) --size;
      return rc;
    }

    template<typename Functor> void in_order(Functor f) const noexcept
    {
        return in_order(f, root);
    }
    
    template<typename Functor> void preorder(Functor f) const noexcept; 

    template<typename Functor> void postorder(Functor f) const noexcept; 

    template<typename Functor> void levelOrderTraverse(Functor f) const noexcept;

    Node* find(const T&);
    
    std::ostream& print(std::ostream& ostr) const noexcept
    {
        std::cout << "tree::size = " << size << ". contents = { ";

        in_order([](const auto& x) { std::cout << x << ", " << std::flush; });
        
        std::cout << "} " << std::endl;
        return ostr;
    }
    
    friend std::ostream& operator<<(std::ostream& ostr, const bstree& tree)
    {
        return tree.print(ostr);
    }

    void printlevelOrder(std::ostream& ostr) const noexcept;

    void debug_printlevelOrder(std::ostream& ostr) const noexcept;

    std::size_t height() const noexcept
    {
       if (empty()) 
          return 0;
       else
          return height(root);
    }
};

template<typename T> bstree<T>::Node::Node(const typename bstree<T>::Node& lhs) : key{lhs.key}, left{nullptr}, right{nullptr}
{
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
}

template<typename T> typename bstree<T>::Node& bstree<T>::Node::operator=(const Node& lhs) noexcept
{
   if (&lhs == this) return *this;

   key = lhs.key;

   if (lhs.parent == nullptr) // If we are copying a root node, then set parent.
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
/*
template<typename T> bstree<T>::Node::Node(Node&& node) noexcept : key{std::move(node.key)}, left{std::move(node.left)}, right{std::move(node.right)}, parent{node.ptr2parent} 
{
}
*/
template<typename T> typename bstree<T>::Node& bstree<T>::Node::operator=(Node&& node) noexcept
{
   key = std::move(node.key);

   left = std::move(node.left);

   right = std::move(node.right);

   parent = node.parent;

   node.size = 0;
   return *this;
}

template<typename T> bstree<T>& bstree<T>::operator=(const bstree& lhs) noexcept
{
  if (this == &lhs)  {
      
      return *this;
  }

  // This will implicitly delete all Nodes in 'this', and set root to a duplicate tree of Nodes.
  root = std::make_unique<Node>(*lhs.root); 
 
  size = lhs.size; 

  return *this;
}

template<typename T> bool bstree<T>::insert(const T& x) noexcept
{
  if (!root) {
     root = std::make_unique<Node>(x);     
     ++size;
     return true;
  } 
  else {
     auto rc = insert(x, root);
     if (rc) ++size;
     return rc;
  }
};

template<typename T> bool bstree<T>::insert(const T& x, std::unique_ptr<typename bstree<T>::Node>& current) noexcept
{
    if (x < current->key) {

         if (!current->left) 
              current->left =  std::make_unique<Node>(x, current.get());
         else 
             insert(x, current->left);
     
     } else if (x > current->key) {
 
          if (!current->right) { 
              current->right = std::make_unique<Node>(x, current.get());
          }
          else
              insert(x, current->right);

     } else if (x == current->key) 
           return false; 
    
     return true;
}

/*
 * Returns true if found and removed, false if not found
 *
bool bstree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
{

   // If p is not nullptr and... 
   // ...if its key is less than current node and we still have nodes to search 
   if (p && x < p->key) 
      return remove(x, p->left);

   // ...else if its key is greater than current node and we still have nodes to search  
   else if (p && x > p->key)
      return remove(x, p->right);

   // ...else we found the key
   else if (p && p->key == x) { 

       // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...

       // ...If p doesn't have a left child, then...
       if (!p->left) 

           // ...remove p by replacing it with right child
           p = p->right; 

       // ...esle If p doesn't have a right child, then...
       else if (!p->right) 

            // ...remove p by replacing it with left child
            p = p->left; 
       
       // 2. Else if p has two children (ttat aren't nullptr). Swap the found key with its in-order predecessor

       else { // p is an internal node with two children. 

         std::shared_ptr<Node> q = p->left; // Note: This line not possible with unique_ptr

         while (q->right != nullptr) // locate in-order predecessor
                q = q->right;

          p->key = q->key; // Swap its key with p's key and...

          remove(q->key, p->left); // delete the swapped key, which is x. Start searching for x at p->left,
                                   // the root of the in-order predessor.  
       }
       return true;
   }
   return false;
}
*/

template<typename T> 
bool bstree<T>::remove(const T& x, typename bstree<T>::Node *p) noexcept
{
   // If p is not nullptr and... 
   // ...if its key is less than current node and we still have nodes to search 
   if (p && x < p->key) 
      return remove(x, p->left.get());

   // ...else if its key is greater than current node and we still have nodes to search  
   else if (p && x > p->key)
      return remove(x, p->right.get());

   // ...else we found the key
   else if (p && p->key == x) { 

       // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...
       Node *parent = p->parent;

       // ...If p doesn't have a left child, then...
       if (!p->left) { // TOD: Can we test !p->right first, too? 

           // ...remove p by replacing it with right child
           if (root.get() == p) //....If p is root, we can't use parent pointer.
               reset(p->right, root);

            else { 
              // We need the actual unique_ptr. Use the parent to get it.
              std::unique_ptr<Node>& punique = (parent->left.get() == p) ? parent->left : parent->right;
              
              reset(p->right, punique);  // TODO: What if p->right is nullptr, too? Then punique 
           }

        // ...else If p doesn't have a right child, then...
        } else if (!p->right) {

            // ...remove p by replacing it with left child
   
            if (root.get() == p) //....If p is root, the we can't use parent pointer.
                reset(p->left, root); 

            else { 
   
               // We need the actual unique_ptr. Use the parent to get it.
               std::unique_ptr<Node>& punique = (parent->left.get() == p) ? parent->left : parent->right;

               reset(p->left, punique); 
            }
   
         // 2. Else if p has two children (ttat aren't nullptr). Swap the found key with its in-order predecessor

         } else { // p is an internal node with two children. 
   
            Node *q = p->right.get(); 
   
            while (q->left != nullptr) // locate in-order successor
                   q = q->left.get();
   
             // Can't call std::swap here instead because the remove immediately following depends on q->key not changing
             //std::swap(p->key, q->key); // swap key with p's key and...
             p->key = q->key;
   
             remove(q->key, p->right.get()); // delete the swapped key, which is x. Start searching for x at p->left,
                                      // the root of the in-order predessor.  
         }
         return true;
   }
   return false;
}

template<typename T>
template<typename Functor> void bstree<T>::in_order(Functor f, const std::unique_ptr<typename bstree<T>::Node>& current) const noexcept 
{
   if (current == nullptr) {

      return;
   }

   in_order(f, current->left);

   f(current->key); 

   in_order(f, current->right);
}

template<typename T> inline void  bstree<T>::printlevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(*this, &Node::print, ostr);  
  
  levelOrderTraverse(tree_printer);
  
  ostr << std::flush;
}

template<typename T> void bstree<T>::debug_printlevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(*this, &Node::debug_print, ostr);  
  
  levelOrderTraverse(tree_printer);
  
  ostr << std::flush;
}
/* 
 * Compute the "height" of a tree -- the number of  nodes along the longest path from the root node  down to the farthest leaf node.
 * From:  https://www.geeksforgeeks.org/level-order-tree-traversal/
   Other code and comments at https://algorithmsandme.com/level-order-traversal-of-binary-tree/
 */
template<typename T> std::size_t bstree<T>::height(const std::unique_ptr<Node>& current) const noexcept
{
/*
   if (!node)  
      return 0;  

  else {  

      // compute the height of each subtree 
      int lheight = height(node->left.get());  

      int rheight = height(node->right.get());  

      // Use the larger height
      if (lheight > rheight)  
          return(lheight + 1);  
      else
         return(rheight + 1);  
  }  
  */
//From: algorithmsandme.com/level-order-traversal-of-binary-tree
  if (!current) return 0;
 
  int lh = height(current->left);
  int rh = height(current->right);
 
  return 1 + std::max(lh, rh);
}  

template<typename T> template<typename Functor> void bstree<T>::levelOrderTraverse(Functor f) const noexcept
{
   std::queue< std::pair<const Node*, int> > queue; 

   const Node* proot = root.get();

   if (!proot) return;
      
   auto initial_level = 1; // initial, top root level is 1.
   
   // 1. pair.first  is: const bstree<T>::Node*, the current node to visit.
   // 2. pair.second is: current level of tree.
   queue.push(std::make_pair(proot, initial_level));

   while (!queue.empty()) {

       /*
        std::pair<const Node *, int> pair_ = queue.front();
        const Node *current = pair_.first;
        int current_level = pair_.second;
       */

        auto[current, current_level] = queue.front(); 

        f(current, current_level);  

        if(current->left)
            queue.push(std::make_pair(current->left.get(), current_level + 1));  

        if(current->right)
            queue.push(std::make_pair(current->right.get(), current_level + 1));  

        queue.pop(); 
   }
}

template<class T> std::ostream& bstree<T>::Node::debug_print(std::ostream& ostr) const noexcept
{
   ostr << " {["; 
 
   //--ostr << key << "]: this=" << this;
   ostr << key << ']';

   if (parent) 
      ostr << ", parent->key =" << parent->key; 
   else
      ostr << ", parent = nullptr";
 
   if (!left) 
     ostr << ", left = nullptr";
   else
      ostr << ", left->key = " <<  left->key;
   
   if (!right) 
     ostr << ", right = nullptr";
   else
      ostr << ", right->key = " << right->key;
   
   ostr << "}";
 
   return ostr;
}

/*
    Queue<TreeNode> queue = new LinkedList<>();
    ArrayList<Integer> traversal = new ArrayList<>();
 
    if(root == null) return traversal;
 
    queue.add(root);
 
    while (!queue.isEmpty()){

        TreeNode current = queue.poll();
        traversal.add((int)current.getValue());
     
        if(current.getLeft()!= null)
            queue.add(current.getLeft());
        if(current.getRight()!= null)
            queue.add(current.getRight());
    }
    return traversal;
*/
#endif
