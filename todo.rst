delete implementation
=====================

issues
------

Is the same delete algorithm for binary search trees used in both the 2nd Ed. of CLRS as in the 3rd Edition? The pseudo code for delete differs betwen the 2nd and 3rd editions. The 2nd edition's illustration for delete shows
only the three main cases, while the 3rd edition further divides the 3rd case into two subcases. Is what the same basic approach (of replacing an internal node with its in-order successor used) in both the 2nd edition and 3rd?

* delete pseudo code in `CLRS, 2nd edtion <http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap13.htm>`_ 
* delete pseudo code in `CLRS, 3rd edition <http://ressources.unisciel.fr/algoprog/s00aaroot/aa00module1/res/%5BCormen-AL2011%5DIntroduction_To_Algorithms-A3.pdf>`_

And furthermore, is the delete algorithm essentially that same in technique as the Carrano algorithm (see textbook)? Are the only differents in implementation details, ane the same as that discussed by Sedgwich at
https://algs4.cs.princeton.edu/32bst/ and https://algs4.cs.princeton.edu/lectures/32BinarySearchTrees.pdf?

alternate implementation of bst
-------------------------------

Try Node::parent of type std::weak_ptr<NodeNode::parent of type std::weak_ptr<Node>. Does this enable returning std::unique_ptr<Node> uniformly from methods like getSuccessor, where currently we return a 'Node *'?
