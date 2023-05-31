/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * School of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>


//트리 각 노드를 구조체로 선언
typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

//스텍에 필요한 기호상수와 배열,변수 선언
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE]; //스텍은 최대 20개의 원소만 저장 가능
int top = -1;


//큐에 필요한 기호상수와 배열,변수 선언
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE]; //큐는 최대 20개의 원소만 저장 가능
int front = -1;
int rear = -1;



int initializeBST(Node** h);          //트리 초기화 함수

void recursiveInorder(Node* ptr);	  //순환함수 이용하여 inorder 방식으로 트리 순회하는 함수
 
void iterativeInorder(Node* ptr);     //스텍을 이용하여 inorder 방식으로 트리 순회하는 함수
Node* pop();                          //스텍의 pop() 함수 구현 : 스텍에서 원소를 꺼냅
void push(Node* aNode);               //스텍의 push() 함수 구현: 스텍에 원소를 넣음

void levelOrder(Node* ptr);	          //큐를 이용하여 같은 레벨에 있는 트리 노드를 순회하는 함수
Node* deQueue();                      //큐의 dequeue() 함수 구현: 큐에서 원소를 꺼냄 
void enQueue(Node* aNode);            //큐의 enqueue() 함수 구현: 큐에 원소를 넣음

int insert(Node* head, int key);      //트리 노드 삽입 함수
int deleteNode(Node* head, int key);  //트리 노드 삭제 함수
int freeBST(Node* head);              //동적 할당 해제 함수
void freeNode(Node* ptr);             //실질적 동적 할당 해제 함수

void printStack();                    //스텍 상황 출력 함수



int main()
{
    printf("\n\n[----- [Park Mira] [2020069006] -----]\n\n");


	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}



/*트리 초기화 함수*/
int initializeBST(Node** h) {

	/*트리가 비어있지 않을 경우*/
    //헤드노드의 여부로 판단하여 트리를 초기화함.
	if(*h != NULL)
		freeBST(*h);

	/*헤드노드 생성*/
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	//root 노드를 가리켜야하나, 현재 root 노드가 없으므로 NULL로 설정함
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}


/*순환함수 이용하여 inorder 방식으로 트리 순회하는 함수*/
void recursiveInorder(Node* ptr)
{
	if(ptr) {
		recursiveInorder(ptr->left);   //왼쪽 노드 끝까지 탐색
		printf(" [%d] ", ptr->key);    //현재 노드의 key값 출력
		recursiveInorder(ptr->right);  //오른쪽 노드 끝까지 탐색
	}
}


/*스텍을 이용하여 inorder 방식으로 트리 순회하는 함수*/
void iterativeInorder(Node* node)
{
    /*반복문과 스텍을 이용함*/
	for(;;) 
	{
		for(;node; node = node->left) //현재 노드 주소값이 NULL 일때까지 반복: 탐색하지 않은 왼쪽 끝까지 탐색
			push(node); //탐색하지 않은 왼쪽 끝까지 노드를 순회하면서 지나치는 노드를 스텍에 쌓아둠.

        //node는 NULL값이며, 가장 최근에 스텍에 push된 값이 이번에 출력될 값임. 그래서 node 값을 대체함.
		node = pop(); //

		if(!node) //node가 NULL일 경우에는 반복문을 벗어남 == 모든 순회가 끝났을 경우
            break;

		printf(" [%d] ", node->key); //현재 노드 값을 출력함.

		node = node->right; //왼쪽-부모 값을 출력 했으므로 오른쪽 노드도 확인하기 위해 노드값을 현재 노드의 right 값으로 변경
	}
}


/*큐를 이용하여 같은 레벨에 있는 트리 노드를 순회하는 함수*/
void levelOrder(Node* ptr)
{
	// int front = rear = -1: 큐의 첫 상태는 처음과 끝이 모두 -1임.

	/*노드가 비어있을 경우*/
	if(!ptr) return; 

	/*root 노드를 큐에 넣음*/
	enQueue(ptr);

	/*반복문과 큐를 이용함*/
	for(;;)
	{
		ptr = deQueue(); //현재 노드를 저장함
		
		/*
		1. 현재 노드의 key값을 출력함.
		2. 현재 노드의 왼쪽,오른쪽 노드를 각각 순서대로 큐에 삽입함.
		3. 이 과정을 반복하다보면, 
		   현재노드 출력(왼쪽 노드, 오른쪽 노드를 저장한 후에, ptr=deQueue에서 왼쪽 노드가 현재 노드로 설정되고, 그 다음 번에는 ptr=deQueue에서 오른쪽 노드가 현재 노드로 설정)
		   ->현재노드 왼쪽 노드 출력
		   ->현재노드 오른쪽 노드 출력이 반복되어 같은 레벨이 출력됨.
		*/
		if(ptr) {
			printf(" [%d] ", ptr->key); //현재 노드의 key 값 출력

			if(ptr->left) 
				enQueue(ptr->left);

			if(ptr->right)
				enQueue(ptr->right);
		}

		//ptr이 NULL일 경우는 모든 트리노드를 순회하였다는 뜻이므로 반복문을 종료함.
		else
			break;

	}

}


/*트리 노드 삽입 함수*/
int insert(Node* head, int key)
{
	/*새로운 노드 생성*/
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	/*case1: root 노드가 없어 새로운 노드가 root 노드가 되는 경우*/
	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/*case2: 노드 삽입*/
	Node* ptr = head->left;  //현재 노드 위치 저장 
	Node* parentNode = NULL; //현재 노드의 전 위치(==부모 노드) 저장

	/*새로운 노드가 삽입될 적절한 위치를 찾을 떄까지 반복: 새로운 노드는 단말노드가 되기 때문에 새로운 노드가 추가되어야 하는 기존 단말노드까지 탐색함.*/
	while(ptr != NULL) {

		/*주어진 key 값을 가진 노드가 있을 경우 삽입을 종료함*/
		if(ptr->key == key) return 1;
		
		/*새로운 노드가 적절한 위치에 삽입될 수 있도록 위치를 탐색하는 과정*/
		parentNode = ptr; //현재 노드를 저장하여 현재 노드가 다음 노드를 가리킬 수 있도록 함.(==부모노드 저장)

		//새로운 노드의 key 값이 현재 노드의 key 값보다 클 경우, 현재 노드를 현재 노드의 오른쪽 노드로 이동함.
		if(ptr->key < key)
			ptr = ptr->right;
		
		//새로운 노드의 key 값이 현재 노드의 key 값보다 작을 경우, 현재 노드를 현재 노드의 왼쪽 노드로 이동함.
		else
			ptr = ptr->left;
	}

	//새로운 노드의 key 값이 현재 노드의 key 값보다 작을 경우, 새로운 노드를 부모 노드의 왼쪽 노드로 삽입
	if(parentNode->key > key)
		parentNode->left = newNode;

	//새로운 노드의 key 값이 현재 노드의 key 값보다 클 경우, 새로운 노드를 부모 노드의 오른쪽 노드로 삽입	
	else
		parentNode->right = newNode;

	return 1;
}


/*트리 노드 삭제 함수*/
int deleteNode(Node* head, int key)
{
	/*예외1: 헤드노드가 없을 경우*/
	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/*예외2: root노드가 없을 경우*/
	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}


	Node* root = head->left;    //root 노드 값을 저장
	Node* parent = NULL;        //부모노드를 저장할 변수 선언
	Node* ptr = root;           //현재노드를 저장할 변수 선언 및 root노드가 첫 노드이므로 root노드로 초기화

	
	/*
	주어진 Key 값을 가진 노드의 위치를 탐색하는 과정으로
	1) 현재노드가 NULL == 주어진 Key 값을 가진 노드가 없음.
    2) 주어진 Key 값을 가진 노드 발견식 반복문 빠져나옴
	*/
	while((ptr != NULL)&&(ptr->key != key)) {
		if(ptr->key != key) {
		//현재 노드의 key 값이 주어진 key 값과 다를 경우 현재 위치를 이동시킴.

			parent = ptr;		   //이동하기 전에 현재 노드를 부모 노드로 저장함

			if(ptr->key > key)     //현재 노드의 key 값이 주어진 key 값보다 클 경우 현재 노드를 현재 노드의 왼쪽 노드로 이동함.
				ptr = ptr->left;

			else
				ptr = ptr->right;  //현재 노드의 key 값이 주어진 key 값보다 작을 경우 현재 노드를 현재 노드의 오른쪽 노드로 이동함.
		}
	}

	/*예외3:주어진 Key 값을 가진 노드가 없음. */
	if(ptr == NULL)
	{
		printf("No node for key [%d]\n ", key);
		
		return -1;
	}

	
	/*case1: 삭제하려는 노드가 단말 노드일 경우*/
	if(ptr->left == NULL && ptr->right == NULL)
	{
		//root 노드를 제외하고 다른 노드가 1개 이상일 경우
		if(parent != NULL) { 
			if(parent->left == ptr)     //부모노드의 왼쪽이 삭제하려는 노드일 경우
				parent->left = NULL;
			else
				parent->right = NULL;   //부모노드의 오른쪽이 삭제하려는 노드일 경우
		} 
		
		//root 노드를 삭제할 경우
		else {
			
			head->left = NULL;

		}

		free(ptr); //동적 할당 해제함으로 노드를 삭제

		return 1;
	}


	/*case2: 삭제하려는 노드가 하나의 자식 노드를 가지고 있을 경우*/
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child; //자식노드를 저장할 변수 선언

		if (ptr->left != NULL)   //삭제하려는 노드의 왼쪽 노드가 있을 경우, 자식노드에 왼쪽 노드를 저장
			child = ptr->left;
		else                     //삭제하려는 노드의 오른쪽 노드가 있을 경우, 자식노드에 오른쪽 노드를 저장
			child = ptr->right;


		//root 노드를 제외하고 다른 노드가 1개 이상일 경우
		if(parent != NULL)
		{
			if(parent->left == ptr)
				parent->left = child;   //부모노드의 왼쪽 노드를 삭제하려는 노드의 자식 노드로 대체
			else
				parent->right = child;  //부모노드의 오른쪽 노드를 삭제하려는 노드의 자식 노드로 대체
		} 

		//root 노드를 삭제할 경우
		else { //현재 노드의 한 개뿐인 자식노드를 루트로 설정
			root = child;
		}

		free(ptr); //동적 할당 해제함으로 노드를 삭제

		return 1;
	}


	/*case3: 삭제하려는 노드가 두 개의 자식 노드를 가지고 있을 경우*/
	Node* candidate;          //삭제된 노드에 들어갈 노드를 저장할 후보자 변수 선언
	parent = ptr;             //삭제하려는 노드를 부모 노드로 저장
	candidate = ptr->right;   //우선, 삭제하려는 노드의 오른쪽 노드를 후보자 변수에 저장

	//삭제하려는 노드 기준으로 오른쪽 노드에서 가장 작은 값을 찾아 후보자 변수에 저장함
	while(candidate->left != NULL)
	{
		parent = candidate;
		candidate = candidate->left;
	}

	//부모노드의 오른쪽 노드와 후보자 노드가 같을 경우 부모노드의 오른쪽 노드를 후보자 노드의 오른쪽 노드로 변경
	if (parent->right == candidate)
		parent->right = candidate->right;
	
	//부모노드의 왼쪽 노드와 후보자 노드가 같을 경우 부모노드의 완쪽 노드를 후보자 노드의 오른쪽 노드(NULL)로 변경
	else
		parent->left = candidate->right;


	ptr->key = candidate->key; //삭제하려는 노드의 key 값을 후보자 key 값으로 변경

	free(candidate); //후보자 노드는 이제 필요없어지므로 동적 할당 해제

	return 1;
}


/*실질적 동적 할당 해제 함수*/
void freeNode(Node* ptr)
{
	if(ptr) {
		//순환함수 이용해 동적할당 해제
		freeNode(ptr->left);   //왼쪽 끝까지 탐색
		freeNode(ptr->right);  //오른쪽 끝까지 탐색
		free(ptr);             //하나씩 해제
	}
}


/*동적 할당 해제 함수*/
int freeBST(Node* head)
{

	/*root노드가 없고, 헤드노드만 있을 경우*/
	if(head->left == head)
	{
		free(head);
		return 1;
	}

	//root노드 위치를 새로운 변수로 생성해 저장: 헤드는 마지막에 삭제해야하므로.
	Node* p = head->left;

	//root 노드 주소를 인수로 보냄
	freeNode(p);

	//마지막으로 헤드노드도 동적 할당 해제
	free(head);

	return 1;
}


/*스텍의 pop() 함수 구현 : 스텍에서 원소를 꺼냅*/
Node* pop()
{
	if (top < 0) return NULL;  //스텍이 비어있을 경우 NULL 값을 반환해 pop할 원소가 없음을 알림
	return stack[top--];       //top에 위치한 원소를 반환하고, top의 위치를 한 칸 줄임
}


/*스텍의 push() 함수 구현: 스텍에 원소를 넣음*/
void push(Node* aNode)
{
	stack[++top] = aNode; //스텍의 top을 한 칸 늘려 새로운 원소를 넣음
}


/*스텍 상황 출력 함수*/
void printStack()
{
	int i = 0;

	printf("--- stack ---\n");

	while(i <= top)//스텍의 탑 위치까지 출력
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
		i++; //위치 증가
	}
}


/*큐의 dequeue() 함수 구현: 큐에서 원소를 꺼냄 */
Node* deQueue()
{
	//큐가 비어있을 경우
	if (front == rear) {
		printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	//맨 앞에 있는 큐 값을 반환하기 위해 위치선정
	front = (front + 1) % MAX_QUEUE_SIZE;

	//가장 앞에 있는 큐 값 출력
	return queue[front];

}


/*큐의 enqueue() 함수 구현: 큐에 원소를 넣음*/
void enQueue(Node* aNode)
{
	//큐의 마지막 위치를 변수에 저장
	rear = (rear + 1) % MAX_QUEUE_SIZE;

	//큐가 가득찬 상황일 경우 원소를 더 이상 넣지 못함
	if (front == rear) {
		// printf("\n....Now Queue is full!!\n");
		return;
	}

	//큐에 원소 삽입
	queue[rear] = aNode;
}
