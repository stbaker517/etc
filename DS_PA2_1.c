#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node{
    char data[51];
    struct Node *prev,*next;
    int count;
}Node;

typedef struct LinkedList{
    Node *head,*tail;
}LinkedList;

LinkedList* createList();
Node* createNode();

void insertList(LinkedList*);
void sortList(LinkedList*);

void printList(LinkedList*);
void freeList(LinkedList*);

int main(){
    int n;
    LinkedList* LList=createList();
    scanf("%d",&n);
    
    for(int i=0;i<n;i++)
    {
        insertList(LList);
    }
    sortList(LList);
    printList(LList);
    freeList(LList);






}

LinkedList* createList()
{
    LinkedList* LList=(LinkedList*)malloc(sizeof(LinkedList));
    LList->head=createNode();
    LList->tail=createNode();
    LList->head->next=LList->tail;
    LList->tail->prev=LList->head;

    return LList;

}


Node* createNode()
{
    Node* node=(Node*)malloc(sizeof(Node));
    node->next=NULL;
    node->prev=NULL;
    node->count=1;
    return node;
}

void insertList(LinkedList* list)
{
    Node* prevoftarget=list->head;
    Node* newNode=createNode();

    scanf("%s",newNode->data);
    getchar();

    if(list->head->next!=list->tail)//엠티
    {
        while(prevoftarget->next!=list->tail&&strcmp(newNode->data,prevoftarget->next->data)>0)
        {
            prevoftarget=prevoftarget->next;
        }
    }

    if(strcmp(newNode->data,prevoftarget->next->data)==0)
    {
        prevoftarget->next->count++;
        free(newNode);
        return;
    }

    newNode->prev=prevoftarget;
    newNode->next=prevoftarget->next;
    newNode->next->prev=newNode;
    newNode->prev->next=newNode;

    return;

}

void sortList(LinkedList* list)
{
    Node* target=list->head->next;
    Node*set;
    Node*temp;
    while(target!=list->tail)
    {
        set=target->prev;
       while(set!=list->head&&set->count < target->count)
       {
            set=set->prev;
       }
       temp=target->next;
        target->prev->next=target->next;
        target->next->prev=target->prev;

        target->prev=set;
        target->next=set->next;
        
        target->prev->next=target;
        target->next->prev=target;

        target=temp;
    
    
    }




}
void printList(LinkedList* list)
{
    Node* target=list->head->next;
    while(target!=list->tail)
    {
        //printf("%s\n",target->data);
        printf("%s %d\n",target->data,target->count);
        target=target->next;
    }    
}

void freeList(LinkedList* list)
{
    Node*target=list->head;
    Node*remove=list->head;
    while(target!=NULL)
    {
        remove=target;
        target=target->next;
        free(remove);
    }
    free(list);
}