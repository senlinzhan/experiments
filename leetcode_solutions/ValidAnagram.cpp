class Solution {
public:
    bool isAnagram( const string &s, const string &t ) 
    {
        array<char, 256> table1{ 0 };
        array<char, 256> table2{ 0 };
        
        for( auto ch : s ) 
        {
            ++table1[ch];
        }

        for( auto ch : t ) 
        {
            ++table2[ch];
        }

        return table1 == table2;
    }
};
