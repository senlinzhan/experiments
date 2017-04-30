class Solution 
{
public:
    string addBinary( const string &a, const string &b ) 
    {
        string sumString;
        
        int plusOne = 0;
        auto left = a.crbegin();
        auto right = b.crbegin();
        
        while( left != a.crend() || right != b.crend() ) 
        {
            char ch;
            if( left == a.crend() ) 
            {
                ch = addTwoDigits( *right, '0', plusOne );
                ++right;
            } 
            else if( right == b.crend() ) 
            {
                ch = addTwoDigits( '0', *left, plusOne );
                ++left;
            }
            else 
            {
                ch = addTwoDigits( *left, *right, plusOne );
                ++left;
                ++right;
            }

            sumString.push_back( ch );
        }

        if( plusOne == 1 ) {
            sumString.push_back( '1' );
        }
        
        reverse( sumString.begin(), sumString.end() );
        return sumString;
    }

    char addTwoDigits( char a, char b, int &plusOne )
    {
        int value = ( a - '0' ) + ( b - '0' ) + plusOne;
        plusOne = ( value >= 2 ? 1 : 0 );
        value = value % 2;
        return static_cast<char>( value + '0' );
    }
};
