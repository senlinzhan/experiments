class Solution {
public:
    vector<int> plusOne( const vector<int> &digits )
    {
        int plusOne = 1;
        vector<int> result;
        result.reserve( digits.size() );
        
        for( auto iter = digits.crbegin(); iter != digits.crend(); ++iter ) 
        {
            int value = *iter + plusOne;
            plusOne = ( value >= 10 ? 1 : 0 );
            result.push_back( value % 10 );
        }
        
        if( plusOne == 1 ) 
        {
            result.push_back( 1 );
        }

        std::reverse( result.begin(), result.end() );
        
        return result;
    }
};

















