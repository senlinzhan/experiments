class Solution {
public:
    vector<string> generateParenthesis( int n )
    {
        vector<string> result;
        string s;
        generate( result, s, n, n );
        return result;
    }

    void generate( vector<string> &result, string &s, int left, int right )
    {
        if( left == 0 && right == 0 ) {
            result.push_back( s );
            return;
        }

        if( left > 0 ) {
            s.push_back( '(' );
            generate( result, s, left - 1, right );
            s.pop_back();
        }
        if( right > left ) {
            s.push_back( ')' );
            generate( result, s, left, right - 1 );
            s.pop_back();
        }
    }
};
