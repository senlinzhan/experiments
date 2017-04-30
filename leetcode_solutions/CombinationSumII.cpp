class Solution {
public:
    vector<vector<int>> combinationSum2( vector<int> &candidates, int target ) 
    {
        vector<vector<int>> allComb;
        vector<int> comb;

        sort( candidates.begin(), candidates.end() );
        helper( candidates.begin(), candidates.end(), allComb, comb, target );
        return allComb;
    }

    template <typename RandomIterator>
    void helper( RandomIterator first, RandomIterator last, vector<vector<int>> &allComb, 
                 vector<int> &comb, int target )
    {
        if( target == 0 ) 
        {
            allComb.push_back( comb );
            return;
        }
        
        for( auto iter = first; iter != last && *iter <= target; ++iter ) 
        {
            if( iter == first || *iter != *( iter - 1 ) ) {
                comb.push_back( *iter );
                helper( iter + 1, last, allComb, comb, target - *iter );
                comb.pop_back();
            }
        }
    }
};
