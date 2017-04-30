class Solution {
public:
    vector<int> getRow( int rowIndex )
    {
        if( rowIndex < 0 ) 
        {
            return { };
        }
        
        vector<int> row( rowIndex + 1, 1 );
        
        for( int i = 2; i <= rowIndex; ++i ) {
            fillRow( row, i );
        }
        
        return row;
    }
private:
    void fillRow( vector<int> &row, int n )
    {
        vector<int> curr( n + 1, 1 );
        for( int i = 1; i < n; ++i ) {
            curr[i] = row[i] + row[i - 1];
        }
        for( int i = 1; i < n; ++i ) {
            row[i] = curr[i];
        }
    }
};
