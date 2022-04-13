#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#define N 100
int flag_for_open = 0;
FILE *input_file = NULL;

typedef struct list_1
{
    wchar_t data[N];
    struct Node* node;
    struct list* next;
}List_string;

typedef struct list
{
    int data;
    struct Node* node;
    struct list* next;
}List;

void wscanfp(wchar_t* str)
{
    int c = 0;
    c = getwc(stdin);
    int i = 0;
    while(c!=(int)'\n'){
        str[i] = (wchar_t) c;
        i++;
        c = getwchar();
    }
    str[i] = '\0';
}

int wscanf_value(FILE* input)
{
    int c = 0, fl = 0, return_value = 0;
    while((c=getwc(input))!=(int)'\n'){
        if(c>=48&&c<=57) return_value = return_value*10 + (c-48);
        else fl = 1;
    }
    if(fl == 1) return -1;
    else return return_value;
}

typedef struct stack
{
    struct Node* node;
    struct stack* next;
}Stack;

typedef struct _Node
{
    int ID;
    wchar_t Name[N];
    wchar_t Author[N];
    int Release_year;
    wchar_t Publishing_house[N];
    int Reader_rating;
    int Price;
    wchar_t Genre[N];
    wchar_t Brief_description[10 * N];
    struct Node* left;
    struct Node* right;
    short height;
}Node;

short height(Node* p)
{
    return p?p->height:-1;
}

void fixheight(Node* p)
{
    short hl = height((Node *) p->left);
    short hr = height((Node *) p->right);
    if (hl==hr&&hl==-1) p->height = 0;
    else {
        if (hl > hr) p->height = hl + 1;
        else {
            p->height = hr + 1;
        }
    }
}

short balance_factor(Node* p)
{
    return height((Node *) p->right) - height((Node *) p->left);;
}

Node* rotateleft(Node* q)
{
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

Node* rotateright(Node* p)
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

void fill_stdin(Node* new_node)
{
    wprintf(L"\nВведите название книги: ");
    wscanfp(new_node->Name);
    wprintf(L"Введите автора книги: ");
    wscanfp(new_node->Author);
    wprintf(L"Введите год выпуска книги: ");
    new_node->Release_year = -1;
    while(new_node->Release_year==-1) {
        new_node->Release_year = wscanf_value(stdin);
        if(new_node->Release_year==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
    }
    wprintf(L"Введите название издательства книги: ");
    wscanfp(new_node->Publishing_house);
    wprintf(L"Введите рейтинг книги(от 0 до 100): ");
    new_node->Reader_rating = -1;
    while(new_node->Reader_rating==-1) {
        new_node->Reader_rating = wscanf_value(stdin);
        if(new_node->Reader_rating==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
        else if(new_node->Reader_rating>100){
            new_node->Reader_rating = -1;
            wprintf(L"Некорректный ввод, попробуйте еще раз\n");
        }
    }
    wprintf(L"Введите цену книги: ");
    new_node->Price = -1;
    while(new_node->Price==-1) {
        new_node->Price = wscanf_value(stdin);
        if(new_node->Price==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
    }
    wprintf(L"Введите жанр книги: ");
    wscanfp(new_node->Genre);
    wprintf(L"Введите краткое описание книги: ");
    wscanfp(new_node->Brief_description);
}

Node* balance(Node* p) // балансировка узла p
{
    fixheight(p);
    if( balance_factor(p)==2 )
    {
        if(balance_factor((Node *) p->right) < 0 )
            p->right = rotateright((Node *) p->right);
        return rotateleft(p);
    }
    if( balance_factor(p)==-2 )
    {
        if(balance_factor((Node *) p->left) > 0  )
            p->left = rotateleft((Node *) p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}

void Reset_to_zero(Node* node)
{
    node->Release_year = 0;
    node->Reader_rating = 0;
    node->Price = 0;
    for (int i = 0; i < N; i++)
        node->Name[i] = node->Author[i] = node->Publishing_house[i] = node->Genre[i] = '\0';
    for (int j = 0; j < 10 * N; j++)
        node->Brief_description[j] = '\0';
    node->left = node->right = NULL;
}

void Push(Stack** stack, Node* node)
{
    Stack* new_part = NULL;
    new_part = (Stack*)malloc(sizeof(Stack));
    if (new_part != NULL)
    {
        new_part->node = node;
        new_part->next = *stack;
        *stack = new_part;
    }
}

Node* Pop(Stack** stack)
{
    Node* node = NULL;

    if (stack == NULL)
        return EXIT_SUCCESS;

    node = (*stack)->node;
    *stack = (*stack)->next;

    return node;
}

int Count_books_in_stack(Stack* start)
{
    int count = 0;
    while (start->next != NULL)
    {
        count++;
        start = start->next;
    }
    return count;
}

void Print_stack(Stack* start)
{
    int i = 1;
    if (start->node == NULL)
        wprintf(L"Поиск не дал результатов.\n");
    else
    {
        Node* current_node = NULL;
        wprintf(L"Найденные книги по вашему запросу:\n");
        while (start->next != NULL)
        {
            current_node = start->node;
            wprintf(L"%d) Name: \"%S\". Release year: %d. Author: %S.\n", i, current_node->Name, current_node->Release_year, current_node->Author);
            i++;
            start = start->next;
        }
    }
}

void Search(Node* root, wchar_t key_word[], int key, Stack** stack) //Поиск
{
    Node* current_node = root;
    if (current_node != NULL)
    {
        if ((wcsstr(current_node->Name, key_word) != NULL) ||
            (wcsstr(current_node->Author, key_word) != NULL) ||
            (wcsstr(current_node->Genre, key_word) != NULL) ||
            (wcsstr(current_node->Publishing_house, key_word) != NULL) ||
            (current_node->Release_year == key) ||
            (current_node->Reader_rating == key) ||
            (current_node->Price == key))
        {
            if(key_word[0]!='\0') Push(stack, current_node);
            else if(current_node->ID==key) Push(stack, current_node);
        }
        if (current_node->left != NULL)
            Search((Node *) current_node->left, key_word, key, stack);
        if (current_node->right != NULL)
            Search((Node *) current_node->right, key_word, key, stack);
    }
}

void print_root(Node* node)
{
    if(node!=NULL){
        Node* left_node = node->left;
        Node* right_node = node->right;
        wprintf(L"Вершина %d, ", node->ID);
        if(left_node!=NULL) wprintf(L"left = %d, ", left_node->ID); else wprintf(L"left = NULL, ");
        if(right_node!=NULL) wprintf(L"right = %d, ", right_node->ID); else wprintf(L"right = NULL, ");
        wprintf(L"height = %d\n", node->height);
        print_root(left_node);
        print_root(right_node);
    }
}

void filling_file(Node* current_node)
{
    int c = 0;
    while ((c = getwc(input_file)) != ':');

    int i = 0;
    while((c = getwc(input_file))!=(int)'\n'){
        current_node->Name[i] = (wchar_t)c;
        i++;
    }
    current_node->Name[i] = '\0';

    while ((c = getwc(input_file)) != ':');

    fgetws(current_node->Author, N, input_file);
    i = 0;
    while (current_node->Author[i] != '\n') i++;
    current_node->Author[i] = '\0';

    while ((c = getwc(input_file)) != ':');
    current_node->Release_year = wscanf_value(input_file);
    if(current_node->Release_year==-1){
        wprintf(L"Невозможно открыть базу данных: неккоректное заполнение поля year, ID = %d\n", current_node->ID);
        flag_for_open = 1;
    }

    while ((c = getwc(input_file)) != ':');
    fgetws(current_node->Publishing_house, N, input_file);
    i = 0;
    while (current_node->Publishing_house[i] != '\n') i++;
    current_node->Publishing_house[i] = '\0';

    while ((c = getwc(input_file)) != ':');
    current_node->Reader_rating = wscanf_value(input_file);
    if(current_node->Reader_rating<0||current_node->Reader_rating>100){
        wprintf(L"Невозможно открыть базу данных: неккоректное заполнение поля Reting, ID = %d\n", current_node->ID);
        flag_for_open = 1;
    }

    while ((c = getwc(input_file)) != ':');
    current_node->Price = wscanf_value(input_file);
    if(current_node->Price==-1){
        wprintf(L"Невозможно открыть базу данных: неккоректное заполнение поля price, ID = %d\n", current_node->ID);
        flag_for_open = 1;
    }

    while ((c = getwc(input_file)) != ':');
    fgetws(current_node->Genre, N, input_file);
    i = 0;
    while (current_node->Genre[i] != '\n') i++;
    current_node->Genre[i] = '\0';

    while ((c = getwc(input_file)) != ':');
    fgetws(current_node->Brief_description, 10*N, input_file);
    i = 0;
    while (current_node->Brief_description[i] != '\n') i++;
    current_node->Brief_description[i] = '\0';
    while (c != 'I' && c != EOF) c = getwc(input_file);
}

Node* Add(Node* root, int k, int flag_for_mode) // вставка ключа k в дерево с корнем root
{
    if(!root){
            Node *new_node = (Node *) malloc(sizeof(Node));
            new_node->ID = k;
            new_node->height = 0;
            if (flag_for_mode == 1) filling_file(new_node);
            else fill_stdin(new_node);
            return new_node;
    }
    if( k<root->ID)
        root->left = Add((Node *) root->left, k, flag_for_mode);
    else
        root->right = Add((Node *) root->right, k, flag_for_mode);
    return balance(root);
}

Node* Open_data(char file_name[], Node* root, int* max_pointer)
{
    int ID = 0;
    int c = 0;
    Node* current_node = root;
    input_file = fopen(file_name, "r");
    while ((c = getwc(input_file)) != EOF&&(flag_for_open==0))
    {
        while ((c = getwc(input_file)) != ':');
        ID = wscanf_value(input_file);
        if (ID > *max_pointer)
            *max_pointer = ID;

        if (root == NULL)
        {
            root = Add(root, ID, 1);
            current_node = root;
        }
        else {
            root = Add(root, ID,1);
        }
    }
    fclose(input_file);
    return root;
}

Node* Add_book(Node* root, int* max)
{
    int ID = 0, Release_year = 0;
    float Reader_rating = 0, Price = 0;
    wchar_t Publishing_house[N] = { '\0' },
            Brief_description[10 * N] = { '\0' },
            Name[N] = { '\0' },
            Author[N] = { '\0' },
            Genre[N] = { '\0' };

    Node* new_node = NULL;
    *max += 1;
    root = Add(root, *max, 0);
    wprintf(L"Книга добавлена в базу.\n\n");
    return root;
}

void Print_node(Node* current_node, Stack* stack)
{
    wprintf(L"\nНайденная книга:\n");
    wprintf(L"ID книги: %d\nНазвание: %S\n", current_node->ID, current_node->Name);
    wprintf(L"Автор: %S\nГод выпуска: %d\n", current_node->Author, current_node->Release_year);
    wprintf(L"Издательство: %S\nОценка читателей: %d\n", current_node->Publishing_house, current_node->Reader_rating);
    wprintf(L"Цена: %d\n", current_node->Price);
    wprintf(L"Жанр: %S\nКраткое описание: %S\n", current_node->Genre, current_node->Brief_description);
    wprintf(L"\n");
    free(stack);
    stack = NULL;
    Push(&stack, NULL);
}

Node* Search_book(Stack* stack, Node* root)
{
    int k = 0, key = 0;
    wchar_t key_word[100] = { '\0' };
    wprintf(L"1) Поиск по ключевому слову.\n");
    wprintf(L"2) Поиск по идентификатору.\n");
    k = -1;
    while(k==-1){
        k = wscanf_value(stdin);
        if(k<1||k>2) {
            wprintf(L"Некорректный ввод, попробуйте еще раз\n");
            k = -1;
        }
    }
    if (k == 1)
    {
        wprintf(L"Введите ключевое слово: ");
        wscanfp(key_word);
        int i = 0, fl = 0, return_value = 0;
        while(key_word[i]){
            if(key_word[i]>=48&&key_word[i]<=57) return_value = return_value*10 + (key_word[i]-48);
            else fl = 1;
            i++;
        }
        if (fl==1) key = -1;
        else key = return_value;
    }
    else
    {
        wprintf(L"Введите идентификатор: ");
        key = -1;
        while(key==-1){
            key = wscanf_value(stdin);
            if(key==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
        }
        key_word[0] = '\0';
    }

    Search(root, key_word, key, &stack);

    Node* current_node = NULL;
    int count = Count_books_in_stack(stack);
    if (count > 1)
    {
        Print_stack(stack);
        wprintf(L"Введите номер искомой книги: ");
        k = -1;
        while(k==-1){
            k = wscanf_value(stdin);
            if((k<1)||(k>count)) {
                wprintf(L"Некорректный ввод, попробуйте еще раз\n");
                k = -1;
            }
        }

        for (int i = 0; i < k; i++)
            current_node = Pop(&stack);

        Print_node(current_node, stack);
    }
    if (count == 1)
    {
        current_node = Pop(&stack);
        Print_node(current_node, stack);
    }
    if (count == 0)
    {
        wprintf(L"Поиск не дал результатов.\n\n");
    }
    return current_node;
}

Node* findmin(Node* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}

Node* removemin(Node* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left==0 )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

Node* remmove(Node* p, int k) // удаление ключа k из дерева p
{
    if( !p ) return 0;
    if( k < p->ID )
        p->left = remmove(p->left, k);
    else if( k > p->ID )
        p->right = remmove(p->right, k);
    else
    {
        Node* q = p->left;
        Node* r = p->right;
        free(p);
        if( !r ) return q;
        Node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

void Redact_book(Stack* stack, Node* root)
{
    Node* current_node = NULL;
    wprintf(L"\nНайдите книгу для редактирования.\n");
    while (current_node == NULL)
        current_node = Search_book(stack, root);
    int red_command = -1;
    while (red_command != 9)
    {
        wprintf(L"Выберите поле для редактирования:\n");
        wprintf(L"1) Название книги\n2) Автор книги\n3) Год выпуска\n4) Издательство\n5) Оценка читателей\n6) Цена\n7) Жанр\n8) Краткое описание\n9) Завершить редактирование\n");
        while(red_command==-1) {
            red_command = wscanf_value(stdin);
            if(red_command==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
        }
        wprintf(L"Введите изменение: ");
        if (red_command == 1) {
            wscanfp(current_node->Name);
        }
        if (red_command == 2) {
            wscanfp(current_node->Author);
        }
        if (red_command == 3){
            current_node->Release_year = -1;
            while(current_node->Release_year==-1) {
                current_node->Release_year = wscanf_value(stdin);
                if(current_node->Release_year==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
            }
        }

        if (red_command == 4) {
            wscanfp(current_node->Publishing_house);
        }
        if (red_command == 5) {
            current_node->Reader_rating = -1;
            while(current_node->Reader_rating==-1) {
                current_node->Reader_rating = wscanf_value(stdin);
                if(current_node->Reader_rating==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
                else if(current_node->Reader_rating>100){
                    current_node->Reader_rating = -1;
                    wprintf(L"Некорректный ввод, попробуйте еще раз\n");
                }
            }
        }
        if (red_command == 6){
            current_node->Price = -1;
            while(current_node->Price==-1) {
                current_node->Price = wscanf_value(stdin);
                if(current_node->Price==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
            }
        }

        if (red_command == 7) {
            fgetws(current_node->Genre, N, stdin);
        }
        if (red_command == 8) {
            wscanfp(current_node->Brief_description);
        }
        if (red_command != 9)
            wprintf(L"Изменение применено.\n");
        if(red_command>9||red_command<1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
    }
    wprintf(L"\n");
}

void Rewrite(Node* root, FILE* output)
{
    if (root == NULL)
        return;
    else
    {
        fwprintf(output, L"ID:%d\n", root->ID);
        fwprintf(output, L"Name:%S\n", root->Name);
        fwprintf(output, L"Author:%S\n", root->Author);
        fwprintf(output, L"Year:%d\n", root->Release_year);
        fwprintf(output, L"Publish:%S\n", root->Publishing_house);
        fwprintf(output, L"Reting:%d\n", root->Reader_rating);
        fwprintf(output, L"Price:%d\n", root->Price);
        fwprintf(output, L"Genre:%S\n", root->Genre);
        fwprintf(output, L"Brief:%S\n", root->Brief_description);
        fwprintf(output, L"----------\n");
    }
    Rewrite(root->left, output);
    Rewrite(root->right, output);
}

void Print_list(List* list)
{
    Node* current_node = NULL;
    if (list == NULL)
        wprintf(L"Книг в библиотеке нет.\n");
    else
    {
        while (list != NULL)
        {
            current_node = list->node;
            wprintf(L"\nID книги: %d\nНазвание: %S\n", current_node->ID, current_node->Name);
            wprintf(L"Автор: %S\nГод выпуска: %d\n", current_node->Author, current_node->Release_year);
            wprintf(L"Издательство: %S\nОценка читателей: %d\n", current_node->Publishing_house, current_node->Reader_rating);
            wprintf(L"Цена: %d\n", current_node->Price);
            wprintf(L"Жанр: %S\nКраткое описание: %S\n", current_node->Genre, current_node->Brief_description);
            wprintf(L"\n");
            list = list->next;
        }
    }
}

void Print_list_string(List_string* list)
{
    Node* current_node = NULL;
    if (list == NULL)
        wprintf(L"Книг в библиотеке нет.\n");
    else
    {
        while (list != NULL)
        {
            current_node = list->node;
            wprintf(L"\nID книги: %d\nНазвание: %S\n", current_node->ID, current_node->Name);
            wprintf(L"Автор: %S\nГод выпуска: %d\n", current_node->Author, current_node->Release_year);
            wprintf(L"Издательство: %S\nОценка читателей: %d\n", current_node->Publishing_house, current_node->Reader_rating);
            wprintf(L"Цена: %d\n", current_node->Price);
            wprintf(L"Жанр: %S\nКраткое описание: %S\n", current_node->Genre, current_node->Brief_description);
            wprintf(L"\n");
            list = list->next;
        }
    }
}

void Insert(List** list, const int data, const Node* node)
{
    List* new_ptr = NULL, * prev = NULL, * current_ptr = *list;

    new_ptr = (List*)malloc(sizeof(List));

    if (new_ptr != NULL)
    {
        new_ptr->data = data;
        new_ptr->node = node;
        new_ptr->next = NULL;

        while (current_ptr != NULL && data < current_ptr->data)
        {
            prev = current_ptr;
            current_ptr = current_ptr->next;
        }

        if (node != NULL)
        {
            if (prev == NULL)
            {
                new_ptr->next = *list;
                *list = new_ptr;
            }
            else
            {
                prev->next = new_ptr;
                new_ptr->next = current_ptr;
            }
        }
    }
}

void Sort_Int(Node* root, List** list, const int command)
{
    Node* current_node = root;
    if (current_node != NULL)
    {
        if (command == 1)
            Insert(list, current_node->ID, current_node);
        if (command == 4)
            Insert(list, current_node->Release_year, current_node);
        if (command == 6)
            Insert(list, current_node->Reader_rating, current_node);
        if (command == 7)
            Insert(list, current_node->Price, current_node);
        if (current_node->left != NULL)
            Sort_Int(current_node->left, list, command);
        if (current_node->right != NULL)
            Sort_Int(current_node->right, list, command);
    }
}

void Insert_string(List_string** list, const wchar_t data[N], const Node* node) {
    List_string *new_ptr = NULL, *prev = NULL, *current_ptr = *list;

    new_ptr = (List_string *) malloc(sizeof(List_string));

    if (new_ptr != NULL) {
        int i = 0;
        for (i = 0; i < N; i++) new_ptr->data[i] = '\0';
        i = 0;
        for (wchar_t c = data[i]; c != '\0'; c = data[i]) {
            new_ptr->data[i] = data[i];
            i++;
        }

        new_ptr->node = node;
        new_ptr->next = NULL;

        while (current_ptr != NULL && wcscmp(data, current_ptr->data) > 0) {
            prev = current_ptr;
            current_ptr = current_ptr->next;
        }
        if (node != NULL)
        {
            if (prev == NULL)
            {
                new_ptr->next = *list;
                *list = new_ptr;
            }
            else
            {
                prev->next = new_ptr;
                new_ptr->next = current_ptr;
            }
        }
    }
}

void Sort_String(Node* root, List_string** list_1, const int command)
{
    Node* current_node = root;
    if (current_node != NULL)
    {
        if (command == 2)
            Insert_string(list_1, current_node->Name, current_node);
        if (command == 3)
            Insert_string(list_1, current_node->Author, current_node);
        if (command == 5)
            Insert_string(list_1, current_node->Publishing_house, current_node);
        if (command == 8)
            Insert_string(list_1, current_node->Genre, current_node);
        if (current_node->left != NULL)
            Sort_String(current_node->left, list_1, command);
        if (current_node->right != NULL)
            Sort_String(current_node->right, list_1, command);
    }
}

void UpDown_Int(List* list) {
    if(list->next!=NULL)
        UpDown_Int(list->next);
    Node* current_node = (Node*)malloc(sizeof(Node));
    current_node = list->node;
    wprintf(L"ID книги: %d\nНазвание: %S\n", current_node->ID, current_node->Name);
    wprintf(L"Автор: %S\nГод выпуска: %d\n", current_node->Author, current_node->Release_year);
    wprintf(L"Издательство: %S\nОценка читателей: %d\n", current_node->Publishing_house, current_node->Reader_rating);
    wprintf(L"Цена: %d\n", current_node->Price);
    wprintf(L"Жанр: %S\nКраткое описание: %S\n", current_node->Genre, current_node->Brief_description);
    wprintf(L"\n");
}

void UpDown_String(List_string* list) {
    if(list->next!=NULL)
        UpDown_String(list->next);
    Node* current_node = (Node*)malloc(sizeof(Node));
    current_node = list->node;
    wprintf(L"ID книги: %d\nНазвание: %S\n", current_node->ID, current_node->Name);
    wprintf(L"Автор: %S\nГод выпуска: %d\n", current_node->Author, current_node->Release_year);
    wprintf(L"Издательство: %S\nОценка читателей: %d\n", current_node->Publishing_house, current_node->Reader_rating);
    wprintf(L"Цена: %d\n", current_node->Price);
    wprintf(L"Жанр: %S\nКраткое описание: %S\n", current_node->Genre, current_node->Brief_description);
    wprintf(L"\n");
}

void Sort(Node* root)
{
    Stack* stack = NULL;
    Push(&stack, NULL);

    List* list = NULL;
    List_string* list_1 = NULL;
    int command = -1;
    int updown = -1;
    wprintf(L"\nВыберите тип сортировки:\n");
    wprintf(L"1) ↑\n");
    wprintf(L"2) ↓\n");
    while(updown==-1) {
        updown = wscanf_value(stdin);
        if(updown < 1 || updown > 2) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
    }
        wprintf(L"\nВыберите параметр для сортировки:\n");
        wprintf(L"1) ID книги.\n");
        wprintf(L"2) Название.\n");
        wprintf(L"3) Автор.\n");
        wprintf(L"4) Год выпуска.\n");
        wprintf(L"5) Издательство.\n");
        wprintf(L"6) Оценка читателей.\n");
        wprintf(L"7) Цена.\n");
        wprintf(L"8) Жанр.\n");
        while(command==-1) {
            command = wscanf_value(stdin);
            if (command < 1 || command > 8) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
        }

    if (command == 1 || command == 4 || command == 6 || command == 7)
    {
        Sort_Int(root, &list, command);
        if(updown == 1)
            Print_list(list);
        else
            UpDown_Int(list);
    }
    else
    {
        Sort_String(root, &list_1, command);
        if(updown == 1)
            Print_list_string(list_1);
        else
            UpDown_String(list_1);
    }
}

void Menu()
{
    int command = -1, max = 0, key = 0;
    int* max_pointer = &max;
    Node* root = NULL;
    Stack* stack = NULL;
    Push(&stack, NULL);

    wprintf(L"Открытие базы данных...\n");
    root = Open_data("library.txt", root, max_pointer); //Открывает файл с базой данных и записываем в дерево
    if(flag_for_open!=1) wprintf(L"База данных открыта.\n\n");

    else return;

    while (command != 6)
    {
        while (command == -1)
        {
            wprintf(L"1) Добавить книгу.\n");
            wprintf(L"2) Удалить книгу.\n");
            wprintf(L"3) Редактировать книгу.\n");
            wprintf(L"4) Поиск книги.\n");
            wprintf(L"5) Сортировать книги.\n");
            wprintf(L"6) Завершить работу.\n");
            wprintf(L"0) Вывод дерева.\n");
            while(command==-1) {
                command = wscanf_value(stdin);
                if(command==-1) wprintf(L"Некорректный ввод, попробуйте еще раз\n");
            }
        }
        if (command == 1) {
            root = Add_book(root, max_pointer);
        }
        if (command == 2)
        {
            Node* current_node = NULL;
            wprintf(L"\nНайдите книгу для удаления.\n");
            while (current_node == NULL)
                current_node = Search_book(stack, root);
            root = remmove(root, current_node->ID);
            wprintf(L"Книга удалена.\n\n");
        }
        if (command == 3)
        {
            Redact_book(stack, root);
        }
        if (command == 4)
        {
            wprintf(L"\n");
            Search_book(stack, root);
        }
        if (command == 5)
        {
            Sort(root);
        }
        if (command == 6)
        {
            FILE* output = NULL;
            output = fopen("library.txt", "w");
            Rewrite(root, output);
            fclose(output);
            free(root);
        }
        if(command == 0){
            print_root(root);
            wprintf(L"\n");
        }
        if(command!= 6)
            command = -1;
    }
}

int main()
{
    setlocale(0, "");
    Menu();
    return EXIT_SUCCESS;
}
