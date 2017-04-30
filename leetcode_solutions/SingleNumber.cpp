class Solution {
public:
    int singleNumber( const vector<int> &nums ) {
        int result = 0;
        
        for( auto elem : nums ) {
            result ^= elem;
        }
        
        return result;
    }
};
