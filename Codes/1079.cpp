#include <stdio.h>
#include <stdlib.h>

#define ArraySize 1000

int n, m;
int heights[ArraySize + 2] = {0};
int max_area = 0;

typedef int DataType;
typedef struct LinkNode
{
    DataType data; struct LinkNode *next;
} LinkNode;

typedef struct { 
    LinkNode *top; int cnt;
} LinkStack;

LinkStack stack;

void InitStack(LinkStack *stack);
void ClearStack(LinkStack *stack);
int IsEmpty(LinkStack *stack);
void Push(LinkStack *stack, DataType data);
DataType Top(LinkStack *stack);
DataType Pop(LinkStack *stack);
LinkNode *CreateNode(DataType data);
void DestroyNode(LinkNode *node);
void InsertAfterPtr(LinkNode *ptr, LinkNode *node);
void DeleteNextPtr(LinkNode *ptr);
void Solve(LinkStack* stack);

int main()
{
    scanf("%d%d", &n, &m);
    InitStack(&stack);
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int dot;
            scanf("%d", &dot);
            if(dot == 0) heights[j]++;
            else heights[j] = 0;
        }
        ClearStack(&stack);
        Solve(&stack);
    }
    printf("%d\n", max_area);
}

void Solve(LinkStack* stack) // 实现算法
{
Push(stack,0);
    for(int j=1;j<=m+1;j++){
        while(!IsEmpty(stack)){
            if(heights[j]>=heights[Top(stack)]){
                Push(stack,j);
                break;
            }
            int t = Pop(stack);
            int area = heights[t]*(j - Top(stack) - 1);
            if(area>max_area)
                max_area = area;
        }
    }
}

void Push(LinkStack *stack, DataType data) // 入栈
{
LinkNode *node = CreateNode(data);
    if (node == NULL)
    {
        printf("overflow\n");
        exit(-1);
    }
    else
    {
        InsertAfterPtr(stack->top, node);
        stack->cnt++;
    }
}

DataType Pop(LinkStack *stack) // 出栈
{ 
DataType ret = Top(stack);
    DeleteNextPtr(stack->top);
    stack->cnt--;
    return ret;
    return 0;
}

void InitStack(LinkStack *stack)
{
    DataType data = 0;
    stack->top = CreateNode(data);
    stack->cnt = 0;
}

void ClearStack(LinkStack *stack)
{
    while (!IsEmpty(stack)) Pop(stack);
}

int IsEmpty(LinkStack *stack)
{
    return (stack->top->next == NULL);
}

DataType Top(LinkStack *stack)
{
    if (IsEmpty(stack)) { printf("underflow\n"); exit(-1); } 
    else { return stack->top->next->data; }
}

LinkNode *CreateNode(DataType data)
{
  LinkNode *node;

  node = (LinkNode *)malloc(sizeof(LinkNode));
  node->data = data;
  node->next = NULL;

  return node;
}

void DestroyNode(LinkNode *node)
{
  //注意：若结构体中的数据域是由动态内存分配产生的，也应释放其内存空间
  free(node);
}

void InsertAfterPtr(LinkNode *ptr, LinkNode *node)
{
    LinkNode *next = ptr->next;
 
    node->next = next;
    ptr->next = node;
}

void DeleteNextPtr(LinkNode *ptr)
{
    LinkNode *next = ptr->next;
    LinkNode *next_next = (next != NULL ? next->next : NULL);

    if (next != NULL) 
    {  
       ptr->next = next_next; 
       DestroyNode(next); 
    }
}