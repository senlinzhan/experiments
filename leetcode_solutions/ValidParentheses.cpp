class Solution 
{
public:
    bool isValid( const string &s )
    {
        auto str = s;
        
        stack<char> coll;
        while( !str.empty() ) 
        {
            char first = str.back();
            str.pop_back();
            
            if( first == ')' || first == ']' || first == '}' ) 
            {
                coll.push( first );
            } 
            else if( first == '(' || first == '[' || first == '{' ) 
            {
                if( coll.empty() ) 
                {
                    return false;
                }
                char second = coll.top();
                coll.pop();

                if( !valid( first, second ) ) {
                    return false;
                }
            } 
            else 
            {
                return false;
            }
        }

        return coll.empty();
    }

private:
    bool valid( char first, char second )
    {
        return ( first == '(' && second == ')' ) || 
               ( first == '[' && second == ']' ) || 
               ( first == '{' && second == '}' );
    }
};
