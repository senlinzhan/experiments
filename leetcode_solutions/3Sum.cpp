class Solution 
{
public:
    vector<vector<int>> threeSum( vector<int> &nums ) 
    {
        vector<vector<int>> result;
        if( nums.size() < 3 ) {
            return result;
        }
   
        sort( nums.begin(), nums.end() );
        
        if( nums.front() > 0 || nums.back() < 0 ) {
            return result;
        }

        int n = nums.size();
        
        for( int i = 0; i < n - 2; ++i ) 
        {
            int begin = i + 1;
            int end = n - 1;
            
            while( begin < end ) 
            {
                int sum = nums[i] + nums[begin] + nums[end];
                if( sum == 0 ) 
                {
                    result.push_back( { nums[i], nums[begin], nums[end] } );
                    while( ++begin < end && nums[begin] == nums[begin - 1] )
                        ;
                    while( --end > begin && nums[end] == nums[end + 1] )
                        ;
                } 
                else if( sum < 0 ) 
                {
                    ++begin;
                } 
                else 
                {
                    --end;
                }
            }

            while( i < n - 2 && nums[i] == nums[i + 1] ) 
            {
                ++i;
            }
        }
        
        return result;
    }
};
