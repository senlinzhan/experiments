class Solution 
{
public:
    int myAtoi( const string &str )
    {
        auto curr = str.cbegin();

        while( curr != str.cend() && *curr == ' ' ) {
            ++curr;
        }

        // if str is empty or all chacters in str are space, then return 0
        if( curr == str.cend() ) {
            return 0;
        }

        bool isNegative = false;
        if( *curr == '-' || *curr == '+' ) {
            isNegative = ( *curr == '-' );
            ++curr;
        }

        long long integer = 0;
        for( ; curr != str.cend() && isdigit( *curr ); ++curr ) {
            integer = integer * 10 + ( *curr - '0' );
            if( integer > INT_MAX ) {
                integer = ( isNegative ? INT_MIN : INT_MAX );
                return static_cast<int>( integer );
            }
        }

        integer = ( isNegative ? -integer : integer );
        return static_cast<int>( integer );
    }
};
