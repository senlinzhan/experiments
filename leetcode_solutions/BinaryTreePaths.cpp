class Solution {
public:
    vector<string> binaryTreePaths( TreeNode *root )
    {
        vector<string> result;
        
        if( !root ) {
            return result;
        }

        string str( std::to_string( root->val ) );
        addPaths( result, root, std::move( str ) );
        return result;
    }

    void addPaths( vector<string> &paths, TreeNode *root, string &&str )
    {
        if( !root->left && !root->right ) 
        {
            paths.push_back( std::move( str ) );
            return;
        }

        if( root->left ) {
            auto left = str;
            left.append( "->" ).append( std::to_string( root->left->val ) );
            addPaths( paths, root->left, std::move( left ) );
        }

        
        if( root->right ) 
        {
            auto right = str;
            right.append( "->" ).append( std::to_string( root->right->val ) );
            addPaths( paths, root->right, std::move( right ) );
        }
    }
};
