To do
=====

Test code in ~/b with thought-out use cases. Then add to ~/d/notes/bst.h


alternate implementation of bst
-------------------------------

Try Node::parent of type std::weak_ptr<NodeNode::parent of type std::weak_ptr<Node>. Does this enable returning std::unique_ptr<Node> uniformly from methods like getSuccessor, where currently we return a 'Node *'?
