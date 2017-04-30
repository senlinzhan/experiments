class Solution {
public:
    int uniquePathsWithObstacles( const vector<vector<int>> &obstacleGrid ) {
        if( obstacleGrid.empty() ) {
            return 0;
        }    
        
        int rowNum = obstacleGrid.size();
        int colNum = obstacleGrid[0].size();
       
        vector<vector<int>> matrix( rowNum + 1, vector<int>( colNum + 1, 0 ) );
        matrix[0][1] = 1;
        for( int row = 1; row <= rowNum; ++row ) {
            for( int col = 1; col <= colNum; ++col ) {
                if( obstacleGrid[row - 1][col - 1] == 0 ) {
                    matrix[row][col] = matrix[row - 1][col] + matrix[row][col - 1];
                }
            }
        }
        
        return matrix.back().back();
    }
};
