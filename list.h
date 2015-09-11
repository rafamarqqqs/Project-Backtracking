#ifndef _list_H_
#define _list_H_


typedef struct listnode ListNode;

struct listnode{
	int src;
	int dest;
	int weight;
	ListNode *next;
};

typedef struct list{
	ListNode *head;
} List;

typedef int (*Compare)(ListNode*, int, int, int);

List *createList();
int addListNode(List *, int, int, int, Compare);
int removeListNode(List *, int);
void freeList(List **);
int printList(List *);
int getSmaller(List *);
int searchNode(List *, int);

int compareMST(ListNode*, int, int, int);

#endif