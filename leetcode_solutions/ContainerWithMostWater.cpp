class Solution {
public:
    int maxArea( vector<int> &height ) {
        if( height.size() < 2 ) {
            return 0;
        }
        
        int i = 0, j = height.size() - 1;
        int max_area = 0;
        
        while( i < j ) {
            max_area = std::max( max_area, std::min( height[i], height[j] ) * ( j - i ) );
            
            if( height[i] < height[j] ) {
                ++i;
            } else {
                --j;
            }
        }
        
        return max_area;
    }
};
