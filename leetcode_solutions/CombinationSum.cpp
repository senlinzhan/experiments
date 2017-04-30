class Solution {
public:
    vector<vector<int>> combinationSum( vector<int> &candidates, int target ) 
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
        if( target == 0 ) {
            allComb.push_back( comb );
            return;
        }

        for( ; first != last && *first <= target; ++first ) {
            comb.push_back( *first );
            helper( first, last, allComb, comb, target - *first );
            comb.pop_back();
        }
    }
};
