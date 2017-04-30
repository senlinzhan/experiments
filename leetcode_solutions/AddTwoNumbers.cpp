class Solution {
public:
    ListNode* addTwoNumbers( ListNode *l1, ListNode *l2 )
    {        
        ListNode *head = nullptr;
        ListNode *tail = nullptr;
     
        int plusOne = 0;  
        ListNode *p1, *p2;
        for( p1 = l1, p2 = l2; p1 && p2; p1 = p1->next, p2 = p2->next ) 
        {
            int value = p1->val + p2->val + plusOne;

            plusOne = ( value >= 10 ? 1 : 0 );
            value = value % 10;

            addNode( head, tail, value );
        }
        
        for( ; p1 ; p1 = p1->next ) 
        {
            int value = p1->val + plusOne;

            plusOne = ( value >= 10 ? 1 : 0 );
            value = value % 10;
            
            addNode( head, tail, value );
        }
        
        for( ; p2 ; p2 = p2->next ) 
        {
            int value = p2->val + plusOne;

            plusOne = ( value >= 10 ? 1 : 0 );
            value = value % 10;

            addNode( head, tail, value );
        }
        
        if( plusOne == 1 ) {
            addNode( head, tail, 1 );
        }
        
        return head;
    }

private:
    void addNode( ListNode *&head, ListNode *&tail, int value )
    {
        if( !head ) 
        {
            head = tail = new ListNode( value );
        } 
        else 
        {
            tail->next = new ListNode( value );
            tail = tail->next;
        }
    }
};
