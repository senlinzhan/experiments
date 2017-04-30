class Solution {
public:
    int maxPathSum( TreeNode *root ) 
    {
        if( !root ) 
        {
            return 0;
        }
        
        int maxPath = root->val;
        doMaxPathSum( root, maxPath );
        return maxPath;
    }

    int doMaxPathSum( TreeNode *root, int &maxPath ) 
    {
        if( !root ) 
        {
            return 0;
        }

        int leftPath = std::max( 0, doMaxPathSum( root->left, maxPath ) );
        int rightPath = std::max( 0, doMaxPathSum( root->right, maxPath ) );
        maxPath = std::max( maxPath, leftPath + rightPath + root->val );
        return std::max( leftPath + root->val, rightPath + root->val );
    }
};
