class Solution {
public:
    int maxSubArray( const vector<int> &nums )
    {
        if( nums.empty() ) {
            return 0;
        }
        vector<int> values( nums.size(), 0 );
        int maxValue = nums[0];
        values[0] = nums[0];
        
        for( int i = 1; i < nums.size(); ++i ) 
        {
            values[i] = std::max( values[i - 1] + nums[i], nums[i] );
            maxValue = std::max( maxValue, values[i] );
        }

        return maxValue;
    }
};
    
