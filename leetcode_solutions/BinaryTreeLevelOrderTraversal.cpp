class Solution {
public:
    vector<vector<int>> levelOrder( TreeNode *root ) 
    {
        if( !root ) {
            return { };
        }

        vector<vector<int>> result;
        vector<int> row;

        queue<TreeNode *> nodes;
        nodes.push( root );
        int num = nodes.size();
        
        while( !nodes.empty() ) 
        {
            auto front = nodes.front();
            nodes.pop();
            row.push_back( front->val );
            if( front->left ) {
                nodes.push( front->left );
            }
            if( front->right ) {
                nodes.push( front->right );
            }

            if( --num == 0 ) {
                num = nodes.size();
                vector<int> t;
                row.swap( t );
                result.push_back( std::move( t ) );
            }
        }

        return result;
    }
};










