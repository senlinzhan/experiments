class Solution {
public:
    int minimumTotal( vector<vector<int>> &triangle ) 
    {
        if( triangle.empty() ) 
        {
            return 0;
        }

        vector<int> minPath( triangle.back() );
        for( int i = triangle.size() - 2; i >= 0; -- i ) 
        {
            for( int j = 0; j <= i; ++j ) 
            {
                minPath[j] = std::min( minPath[j], minPath[j + 1] ) + triangle[i][j];
            }
        }

        return minPath.front();
    }
};
