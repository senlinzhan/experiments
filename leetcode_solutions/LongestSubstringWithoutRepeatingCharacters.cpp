class Solution {
public:
    int lengthOfLongestSubstring( const string &str ) 
    {
        if( str.empty() ) 
        {
            return 0;
        }
        
        constexpr int SIZE = 256;
        array<int, SIZE> table;
        for_each( table.begin(), table.end(), []( int &elem ) {  elem = -1;  } );

        int maxLength = 0;
        
        for( int first = 0, last = 0; last < str.size(); ++last ) 
        {
            // invariant: elements in str[first, last] are unique
            
            int previous = table[str[last]];  // previous position
            
            if( previous >= 0 ) {
                first = std::max( first, previous + 1 );
            }
            
            table[str[last]] = last;
            maxLength = std::max( maxLength, last - first + 1 );
        }
        
        return maxLength;
    }  
};
