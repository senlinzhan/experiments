class Solution {
public:
    bool isPalindrome( ListNode *head ) 
    {
        if( !head || !head->next ) {
            return true;
        }
        
        stack<ListNode*> nodeStack;
        for( auto curr = head; curr != nullptr; curr = curr->next )
        {
            nodeStack.push( curr );
        }
        
        int size = nodeStack.size() / 2;
        for( int i = 0; i < size; ++i ) {
            auto top = nodeStack.top();
            nodeStack.pop();
            if( head->val != top->val ) {
                return false;
            }
            head = head->next;
        }
        return true;
    }    
};
