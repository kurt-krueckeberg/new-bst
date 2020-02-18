#include <cstdlib>
#include <utility>
#include <iostream>
#include <initializer_list>
#include "test.h"
#include "bst.h"

using namespace std;


int main(int argc, char** argv) 
{
  std::initializer_list<int> lst = {50, 39, 15, 65, 69, 50, 25, 20, 70, 100, 40, 34, 37, 30, 10, 33, 36, 38, 85, 90, 60, 35, 80, 89};

  bstree<int, int> tree;

  /*
   * bstree<Test, Test> tree;

  for (const auto& i : lst) 
      tree.insert(Test{i}, Test{i});
   */

  for (const auto& i : lst) 
      tree.insert(i, i);
  
  cout << tree;
   
  auto key_printer = [](const auto& pr) {
      const auto&[key, value] = pr;
      cout << key << ", ";
  };
  
  tree.printlevelOrder(cout, key_printer);
  
  cout << "floor(37) = " << tree.floor(37) << '\n';

  cout << "ceiling(37) = " << tree.ceiling(37) << '\n';
 
  cout << "floor(41) = " << tree.floor(41) << '\n';

  cout << "ceilling(41) = " << tree.ceiling(41) << '\n';

  auto printer_debug = [](const auto& pr) {
      const auto&[key, value] = pr;
      cout << key << ", ";
  };

  tree.printlevelOrder(cout, key_printer);

  for (auto& x : lst) {

     cout << "tree.remove(" << x << ")\n";
     tree.remove(x);
     tree.printlevelOrder(cout, key_printer);
  } 

  return 0;
  /*      
  cout << "This is the input tree " << tree << " printed in debug level order:\n";
  
  tree.debug_printlevelOrder(cout);

  cout << "We now start the removal loop\n";
  
  for (const auto& key : lst) {
      
      cout << "Tree before removal of Test{" << key << "} " << tree; 
      
      if (key == 65) {
          auto debug = 10;
          ++debug;
      }
               
      //tree.remove(Test{key});
      tree.remove(key);

      cout << "Tree after removal of Test{" << key << "}. " << tree; 
      cout << "\nLevel-order print after the removal of Test{" << key << "}\n";
      
      tree.printlevelOrder(cout, key_printer);

      cout << flush << "\nDebug print\n";

      tree.debug_printlevelOrder(cout);
  }

  return 0;
*/
/*

    bstree<Test, Test> tree1 = { 5, 15, 7, 17, 3, 13, 4, 14,  2, 12,  6, 16, 9, 19}; 
    
    cout << "tree1 = " << tree1 << '\n' << "Printing tree1 level order:\n";
    
    tree1.printlevelOrder(cout);
        
    bstree<Test> tree2 = { Test{15}, Test{25}, Test{17}, Test{27}, Test{13}, Test{23}, Test{14}, Test{24},  Test{12}, Test{41},  Test{16}, Test{6}, Test{19}, Test{20}}; 
    
    cout << "\ntree2 = " << tree2 << "\nPrinting tree2 level order:\n";
    
    tree2 = tree1; 
    tree2.printlevelOrder(cout);
    
   cout << "\nAfter 'tree2 = tree1', tree2 = " << tree2 << "\nPrinting tree2 level order:\n" << flush;
   tree2.printlevelOrder(cout);
   
   vector<int> vec = {-5, -15, -7, -17, -3, -13, -4, -14,  -2, -12,  -6, -16, -9, -19}; 
   for (const auto& ref : vec) {

     tree2.remove(ref); 

     cout << "After removing " << ref << " from tree2, tree2 = \n";
     
     tree2.printlevelOrder(cout);
     cout << flush;
   }
   
   return 0;
 * */
}

