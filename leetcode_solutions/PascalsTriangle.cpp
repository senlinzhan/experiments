class Solution 
{
public:
    vector<vector<int>> generate( int numRows ) 
    {
        vector<vector<int>> triangle;

        if( numRows < 1 ) {
            return triangle;
        }
        
        triangle.reserve( numRows );
        for( int i = 1; i <= numRows; ++i ) 
        {
            vector<int> row( i, 1 );
            for( int j = 1; j < i - 1; ++j ) 
            {
                row[j] = triangle[i - 2][j] + triangle[i - 2][j - 1];
            }
            triangle.push_back( std::move( row ) );
        }

        return triangle;
    }
};
