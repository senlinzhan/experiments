class Solution {
public:
    int climbStairs( int n ) 
    {
        if( n < 1 ) 
        {
            return 0;
        } 
        vector<int> buffer( n + 1, 0 );
        buffer[1] = 1;
        buffer[2] = 2;
        return helper( buffer, n );
    }
private:
    int helper( vector<int> &buffer, int n ) 
    {
        if( buffer[n] != 0 ) 
        {
            return buffer[n];
        } 
        else 
        {
            buffer[n] = helper( buffer, n - 1 ) + helper( buffer, n - 2 );
            return buffer[n];
        }
    }
};
