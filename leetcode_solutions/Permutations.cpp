class Solution {
public:
    vector<vector<int>> permute( vector<int> &nums )
    {
        vector<vector<int>> allPermute;
        vector<bool> visited( nums.size(), false );
        
        vector<int> permute;
        backtrack( allPermute, permute, visited, nums, 0 );
        return allPermute;
    }

    void backtrack( vector<vector<int>> &allPermute, vector<int> &permute, 
                    vector<bool> &visited, vector<int> &nums, int start )
    {
        if( start == nums.size() )
        {
            allPermute.push_back( permute );
            return;
        }

        for( int i = 0; i < nums.size(); ++i )
        {
            if( !visited[i] )
            {
                visited[i] = true;
                permute.push_back( nums[i] );
                backtrack( allPermute, permute, visited, nums, start + 1 );
                permute.pop_back();
                visited[i] = false;
            }
        }
    }
};
