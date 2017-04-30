class Solution {
public:
    vector<vector<int>> subsetsWithDup( vector<int> &nums ) 
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
        int previous;
        for( int i = start; i < nums.size(); ++i )
        {
            sub.push_back( nums[i] );
            backtrack( powerSet, sub, nums, i + 1 );
            sub.pop_back();
            while( i + 1 < nums.size() && nums[i + 1] == nums[i] ) {
                ++i;
            }
        }
    }
};
