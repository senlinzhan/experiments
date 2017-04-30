#include <climits>

class Solution {
public:
    int reverse( int x )
    {
        long long reverseInteger = 0;
        
        bool isNegative = ( x < 0 );
        x = isNegative ? -x : x;

        while( x != 0 ) 
        {
            reverseInteger = reverseInteger * ( 10 ) + x % 10;
            x /= 10;
        }
        
        reverseInteger = isNegative ? -reverseInteger : reverseInteger;

        return reverseInteger > INT_MAX || reverseInteger < INT_MIN ? 0 : reverseInteger;
    }
};
