#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define ALPHABET 26
#define MAX 1000000


typedef struct node { //허프만 트리에 들어갈 구조체
    int frequency;
    char character;
    struct node* left;
    struct node* right;
} Node;

Node** heap; //노드 구조체 배열을 선언하기 위해 더블 포인터 선언
int lastheap = 0; // 힙 사이즈를 위한 전역 선언
char codetable[MAX]; //허프만 부호 생성을 위한 배열
int codetable_index = -1;
char* huffmancode[1000000]; //허프만 부호가 저장되어있는 배열
void searchcode(Node* temp, char c); //허프만 부호를 찾아가는 배열
char* result[1000000];
void decoding_print(int index);


void add_heap(Node* newnode)//힙을 추가하기 위한 함수
{
    lastheap++;
    heap[lastheap] = newnode;//새로운 노드를 맨마지막 힙에 넣어준다
    int here = lastheap;//현재위치
    int parent = lastheap / 2;//부모위치

    while (parent >= 1)
    {
        if (heap[parent]->frequency > heap[here]->frequency)//부모노드와 현재위치노드의 빈도수를 비교하여 부모가 더 크다면
        {
            Node* temp = heap[parent]; /*현재 노드와 부모 노드 위치를 바꿔준다*/
            heap[parent] = heap[here];
            heap[here] = temp;
            here = parent;
            parent = here / 2;
        }
        else
        {
            break;
        }
    }
}

Node* delete_heap()//힙의 앞부분부터 빼서 삭제 해준다
{
    int parent = 1; //부모인덱스는 1부터 시작함
    int child = 2; //자식인덱스는 2부터 시작함


    if (lastheap <= 0)//힙에서 더이상 뺄게 없을때
    {
        printf("result\n\n");
        return 0;
    }

    Node* return_node = heap[1];//힙의 첫번째 부분을 루트노드이므로 반환할 노드로 빼준다
    Node* temp = heap[lastheap--];//힙의 맨 뒤에 노드를 빼줘서 temp에 넣는다

    while (child <= lastheap)//child가 마지막 힙일 때까지 반복
    {

        if ((child <= lastheap) && (heap[child]->frequency > heap[child + 1]->frequency)) //child노드가 child+1 보다 빈도수가 더 크다면

            child++;

        if (temp->frequency < heap[child]->frequency) break; //temp가 빈도수를 비교하여 제 위치에 왔으면 반복문 종료

        Node* temp = heap[parent]; /*현재 노드와 부모 노드 위치를 바꿔준다*/
        heap[parent] = heap[child];//현재 child노드를 부모노드에 대입
        heap[child] = temp;
        parent = child;//현재 child index를 parent로

        child *= 2;// 더 밑에 child로 이동

    }
    heap[parent] = temp; //맨 밑에 있던 temp노드가 제 자리로 찾아감
    return return_node;

}




void huffmantree()//허프만트리 함수
{
    Node* first = 0; //빈도수가 첫번째로 작은 노드
    Node* second = 0; //빈도수가 두번째로 작은 노드

    while (1)
    {
        first = delete_heap();/*delete_heap을 통해 노드 두개를 빼온다*/
        second = delete_heap();

        if (second == 0)
        {
            break;
        }
        Node* newone = (Node*)malloc(sizeof(Node));//새로 추가할 노드를 동적할당 시켜준다
        newone->frequency = first->frequency + second->frequency;/*새로 first노드와 second노드를 합쳐 새로운 노드를 만들어 준다*/
        newone->left = first;
        newone->right = second;
        newone->character = 0;
        add_heap(newone);//새로 만든 노드를 힙배열에 추가해준다
    }
    memset(huffmancode, 0, sizeof(huffmancode));//허프만 코드 배열 초기화
    searchcode(first->left, '0'); //first는 허프만트리의 root 노드를 가리킨다. 
    searchcode(first->right, '1');

}

void searchcode(Node* temp, char c) //재귀함수를 이용하여 허프만 부호를 입력한다
{
    codetable_index++;
    codetable[codetable_index] = c; //0 or 1를 문자 형태로 저장
    codetable[codetable_index + 1] = NULL;//마지막에는 항상 NULL로 초기화한다

    if (temp->left == 0 && temp->right == 0)//문자에 해당하는 노드일 경우
    {

        char* Huffcode = (char*)malloc(strlen(codetable) + 1);
        strcpy(Huffcode, codetable);
        huffmancode[(int)temp->character] = Huffcode;//huffcode라는 배열에 실제 문자 아스키코드에 해당하는 인덱스로 허프만부호 저장한다
    }
    else
    {
        searchcode(temp->left, '0');
        searchcode(temp->right, '1');
    }
    codetable[codetable_index] = NULL;//빠져나갈때 배열 비우고 가기
    codetable_index--;//index감소
    return;

}


void make_heap(int i, int freq[]) //처음 힙배열에 노드를 만들어 주는 함수
{
    Node* cur = (Node*)malloc(sizeof(Node));

    cur->character = (char)(i + 65);
    cur->frequency = freq[i];
    cur->left = 0;
    cur->right = 0;
    add_heap(cur); //힙에 추가한다
}



void encoding_print(char str_[])// encoding된 허프만부호를 출력하는 함수
{
    int i = 0;
    int j = 0;
    int index = -1;
    while (1)
    {

        if (str_[i] == '\0') break;//입력받은 문자배열이 끝나면 종료
        j = str_[i]; //입력받은 문자를 아스키코드 숫자로 변환 후
        printf("%s", huffmancode[j]);//huffmancode 해당 인덱스에 저장된 허프만부호를 출력한다
        char* HuffDecode = (char*)malloc(strlen(codetable) + 1);//decode를 위해 출력되는 허프만부호를 이 배열에 담아둠
        strcpy(HuffDecode, huffmancode[j]);
        result[++index] = huffmancode[j];//result 배열에 허프만 부호 담아둠
        i++;
    }

    printf("\n\ndecoding to....\n\n");
    decoding_print(index);//자동으로 decoding 시작 
    return;
}


void decoding_print(int index)//decoding을 출력해주는 함수
{
    for (int i = 0; i <= index; i++)// result에 들어 있는 수까지 반복
    {
        for (int j = 65; j <= 122; j++)//huffmancode에 저장되어있는 허프만부호와 같으면
        {
            if (result[i] == huffmancode[j])
            {
                printf("%c", j);//그 부호에 해당하는 알파벳 출력
            }
        }
    }
        printf("\n\n\n");
        return;
}

void freeHUFFMANttree(Node *ptr)//동적할당 해제함수
{
    if (ptr)//ptr이 NULL이 아닐때 후위순회로 동적할당을 해제한다
    {
        freeHUFFMANttree(ptr->left);//ptr이 NULL이 나올때 까지 왼쪽으로 이동한다
        freeHUFFMANttree(ptr->right);//ptr->right로 이동한다
        free(ptr);//동적할당 해제시켜준다
    }
}

void freecode(char *delete)
{
    int i=0;
    while(1)
    {
        if(delete[i]!=NULL)        
        {
           free(delete);
        }  
        else
            break;
          i++;
    }
    printf("\ndelete clear\n\n");
    return;
}

int main()//메인함수
{
    int i = 0; 
    int j = 0;
    int Freq_big[ALPHABET] = { 0 }; //대문자알파벳 빈도수를 체크하는 배열
    int freq_small[ALPHABET + 32] = { 0 }; //소문자알파벳 빈도수를 체크하는 배열
    char str[100];//문자열을 받아오는 배열
    heap = (Node**)malloc(MAX * sizeof(Node*)); //힙 배열을 위해 heap을 MAX갯수만큼 동적할당 해준다
    memset(heap, 0, MAX * sizeof(Node*));//초기화
    printf("Input String : ");//문자열 입력
    scanf("%s", str);
    while (1)
    {

        if (str[i] == '\0') break;//문자열 끝이면 반복문 종료
        j = str[i];
        if (0 <= (j - 65) && (j - 65) <= 25)//대문자 빈도수세기
        {
            Freq_big[j - 65] += 1;
        }
        else if (32 <= (j - 65) && (j - 65) <= 57)//소문자 빈도수세기
        {
            freq_small[j - 65] += 1;

        }
        else//그외 문자는 허용하지 않음
        {
            printf("알파벳만 입력해주세요 \n");
            break;
        }
        i++;
    }

    for (int i = 0; i < ALPHABET; i++)//대문자를 힙배열에 넣기
    {
        if (Freq_big[i] > 0)
        {
            make_heap(i, Freq_big);
        }
    }
    for (int i = 32; i < ALPHABET + 32; i++)//소문자를 힙배열에 넣기
    {
        if (freq_small[i] > 0)
        {
            make_heap(i, freq_small);
        }
    }
    huffmantree();//허프만 트리 생성
    printf("encoding to....\n\n");
    encoding_print(str);//생성된 허프만 부호 출력

    freecode(huffmancode);//huffman부호 저장되어있는 배열 동적할당해제       
    freecode(result);//decode할 때 할당받은 메모리 동적할당 해제
    freeHUFFMANttree(heap[1]);//허프만 트리 동적할당해제

    free(heap);
    printf("\n\nall node is free\n");
    return 0;

}