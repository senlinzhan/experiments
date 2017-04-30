class Solution {
public:
    string longestCommonPrefix( const vector<string> &strs )
    {
        if( strs.empty() ) {
            return "";
        }

        string commomPrefix = strs.front();

        for( int i = 1; i < strs.size() && !commomPrefix.empty(); ++i ) {
            commomPrefix = commom( commomPrefix, strs[i] );
        }
        return commomPrefix;
    }
private:
    string commom( const string &s1, const string &s2 ) 
    {
        int i = 0;
        for( ; i < s1.size() && i < s2.size(); ++i ) 
        {
            if( s1[i] != s2[i] ) {
                break;
            }
        }
        return s1.substr( 0, i );
    }
};
