delete implementation
=====================

issues
------

Is the same delete algorithm for binary search trees used in both the 2nd Ed. of CLRS as in the 3rd Edition? The pseudo code for delete differs betwen the 2nd and 3rd editions. The 2nd edition's illustration for delete shows
only the three main cases, while the 3rd edition further divides the 3rd case into two subcases. Is what the same basic approach (of replacing an internal node with its in-order successor used) in both the 2nd edition and 3rd?

* delete pseudo code in `CLRS, 2nd edtion <http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap13.htm>`_ 
* delete pseudo code in `CLRS, 3rd edition <http://ressources.unisciel.fr/algoprog/s00aaroot/aa00module1/res/%5BCormen-AL2011%5DIntroduction_To_Algorithms-A3.pdf>`_

And furthermore, is the delete algorithm essentially that same in technique as the Carrano algorithm (see textbook)? Are the only differents in implementation details?
