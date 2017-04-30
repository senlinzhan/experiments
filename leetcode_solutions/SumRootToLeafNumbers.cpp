class Solution {
public:
    int sumNumbers( TreeNode *root ) 
    {
        if( !root ) {
            return 0;
        }

        return sumHelper( root, 0 );
    }

    int sumHelper( TreeNode *root, int currSum )
    {
        currSum = currSum * 10 + root->val;
        if( !root->left && !root->right ) 
        {
            return currSum;
        }

        int sum = 0;
        if( root->left ) 
        {
            sum += sumHelper( root->left, currSum );
        }

        if( root->right ) 
        {
            sum += sumHelper( root->right, currSum );
        }
        return sum;
    }
};
