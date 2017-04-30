class Solution 
{
public:
    void setZeroes( vector<vector<int>> &matrix )
    {
        if( matrix.empty() ) 
        {
            return;
        }

        int rowNum = matrix.size();
        int colNum = matrix[0].size();
        
        vector<bool> rowMark( rowNum, false );
        vector<bool> colMark( colNum, false );
        
        for( int row = 0; row < rowNum; ++row ) 
        {
            for( int col = 0; col < colNum; ++col ) 
            {
                if( matrix[row][col] == 0 ) 
                {    
                    rowMark[row] = true;
                    colMark[col] = true;
                }
            }
        }

        for( int row = 0; row < rowNum; ++row ) 
        {
            if( rowMark[row] ) 
            {
                for( int col = 0; col < colNum; ++col ) 
                {
                    matrix[row][col] = 0;
                }
            } 
            else 
            {
                for( int col = 0; col < colNum; ++col ) 
                {
                    if( colMark[col] ) 
                    {
                        matrix[row][col] = 0;
                    }
                }
            }
        }
    }
};
