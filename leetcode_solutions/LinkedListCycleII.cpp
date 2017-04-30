class Solution {
public:
    ListNode *detectCycle( ListNode *head )
    {
        if( !head || !head->next ) 
        {
            return nullptr;
        }
        
        bool hasCycle = false;
        auto slow = head;
        auto fast = head;
        while( fast && fast->next ) 
        {
            slow = slow->next;
            fast = fast->next->next;
            if( slow == fast ) 
            {
                hasCycle = true;
                break;
            }
        }

        if( !hasCycle ) {
            return nullptr;
        }

        slow = head;
        while( slow != fast ) {
            slow = slow->next;
            fast = fast->next;
        }

        return slow;
    }
};
