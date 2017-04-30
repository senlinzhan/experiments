class Solution {
public:
    vector<vector<int>> combine( int n, int k ) {
        vector<vector<int>> allComb;
        vector<int> comb;
        backtrack( allComb, comb, 1, n, k );
        return allComb;
    }
    
    void backtrack( vector<vector<int>> &allComb, vector<int> &comb, int start, int n, int k )
    {
        if( k == 0 )
        {
            allComb.push_back( comb );
            return;
        }
        
        for( int i = start; i <= n; ++i )
        {
            comb.push_back( i );
            backtrack( allComb, comb, i + 1, n, k - 1 );
            comb.pop_back();
        }
    }
};
