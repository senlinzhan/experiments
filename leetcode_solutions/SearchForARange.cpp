class Solution {
public:
    vector<int> searchRange( const vector<int> &nums, int target ) {
        int low = 0, high = nums.size() - 1;
        
        while( low <= high ) {
            int mid = ( low + high ) / 2;
            if( target < nums[mid] ) {
                high = mid - 1;
            } else if( target > nums[mid] ) {
                low = mid + 1;
            } else {
                int left = mid, right = mid;
                while( left > low && nums[left - 1] == target ) {
                    --left;
                }

                while( right < high && nums[right + 1] == target ) {
                    ++right;
                }

                return { left, right };
            }
        }

        return { -1, -1 };
    }
};

    















