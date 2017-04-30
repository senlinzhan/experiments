class Solution {
public:
    int uniquePaths( int m, int n ) 
    {
        if( m < 1 || n < 1 ) {
            return 0;
        }
        
        vector<vector<int>> matrix( m, vector<int>( n, 1 ) );
        
        for( int row = 1; row < m; ++row ) {
            for( int col = 1; col < n; ++col ) {
                matrix[row][col] = matrix[row][col - 1] + matrix[row - 1][col];
            }
        }
        
        return matrix.back().back();
    }
};
