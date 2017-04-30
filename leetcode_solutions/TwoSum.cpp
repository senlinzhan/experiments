class Solution {
public:
    vector<int> twoSum( const vector<int> &nums, int target )
    {
        if( nums.size() < 2 ) 
        {
            return { };
        }

        unordered_map<int, int> coll;
        
        for( int i = 0; i < nums.size(); ++i ) 
        {
            auto pos = coll.find( target - nums[i] );
            if( pos == coll.cend() ) 
            {
                coll.insert( { nums[i], i } );
            } 
            else 
            {
                return { pos->second + 1, i + 1 };
            }
        }
        
        return { };
    }
};
