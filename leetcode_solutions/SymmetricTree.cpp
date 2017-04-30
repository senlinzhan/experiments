class Solution {
public:
    bool isSymmetric( TreeNode *root ) {
        if( !root ) {
            return true;
        }
        return isSymmetric( root->left, root->right );
    }

    bool isSymmetric( TreeNode *left, TreeNode *right ) {
        if( !left && !right ) {
            return true;
        }

        if( left && right ) {
            return left->val == right->val &&
                   isSymmetric( left->left, right->right ) &&
                   isSymmetric( left->right, right->left );
        }

        return false;
    }
};
