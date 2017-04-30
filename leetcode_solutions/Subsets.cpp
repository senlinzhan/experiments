class Solution {
public:
    vector<vector<int>> subsets( vector<int> &nums ) 
    {
        std::sort( nums.begin(), nums.end() );
        vector<vector<int>> powerSet;

        vector<int> sub;
        backtrack( powerSet, sub, nums, 0 );
        return powerSet;
    }

    void backtrack( vector<vector<int>> &powerSet, vector<int> &sub, const vector<int> &nums, int start )
    {
        powerSet.push_back( sub );
        for( int i = start; i < nums.size(); ++i )
        {
            sub.push_back( nums[i] );
            backtrack( powerSet, sub, nums, i + 1 );
            sub.pop_back();
        }
    }
};


