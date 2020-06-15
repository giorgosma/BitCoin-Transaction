#include "structures.h"
#include "functions.h"

//Hash_Table users, senderHashTable, receiverHashTable;

int hash_function(char *user_id, int size_of_hash_table){
    unsigned long long int hash_number = 0;
    char *p_str = user_id;
    int ascii;
    int i = 0;
    while(*p_str != '\0'){
        ascii = (int)(*p_str);
        p_str++;
        hash_number = hash_number + ascii*calc_power(2, i);
        i++;
    }
    return (hash_number % size_of_hash_table);
}

int calc_power(int number, int x){
    if( x == 0)
        return 1;
    int new_number = number;
    while(x > 1){
        new_number = new_number*number;
        x--;
    }
    return new_number;
}

void create_Hash_Table(Hash_Table *hash_table, int size){
// void create_Hash_Table(Hash_Node **hash_table, int size){
    *hash_table = malloc(size * sizeof(Hash_Node));
    //(*hash_table) = malloc(size * sizeof(Hash_Node));
    int i = 0;
    for (i = 0; i < size; i++){
        //((*hash_table)[i]) = malloc(sizeof(Hash_Node));
        //((*hash_table)[i]).a = i*i;
        ((*hash_table)[i]).bucket_size_of_bytes = 0;
        ((*hash_table)[i]).bucket_list = NULL;
        
    }
}

void delete_Hash_Table(Hash_Table hash_table, int size, int DEL_WALLET){
// void delete_Hash_Table(Hash_Node *hash_table, int size){
    int i = 0;
    for (i = 0; i < size; i++){
        delete_Buckets_List((hash_table[i]).bucket_list, (hash_table[i]).bucket_size_of_bytes / sizeof(Bucket), DEL_WALLET);
    }
    free(hash_table);
    printf("- DELETE HASH TABLE\n");
}

void print_Hash_Table(Hash_Table hash_table, int size, int bucket_size){
    printf("---PRINTING HASH TABLE---\n");
    int i;
    for (i = 0; i < size; i++){
        printf("SEAT: [ %d ]\n", i);
        print_Buckets_List((hash_table[i]).bucket_list, bucket_size);
        printf("\n");
    }
}

// Bucket_List

void create_Buckets_List(Buckets_List **l){
    *l = malloc(1*sizeof(Buckets_List));
    (*l)->bucket = NULL;
    (*l)->next = NULL;
    (*l)->previous = NULL;
}

Buckets_List *get_last_Buckets_List(Buckets_List *l){
    Buckets_List *last_node = l;
    if (last_node->next != NULL)
        last_node = get_last_Buckets_List(last_node->next);
    return last_node;  
}


void add_Buckets_List_node(Buckets_List *l, Bucket *bucket){
    printf("- ADD NEW BUCKET LIST NODE\n");
    if (l->bucket != NULL){            // list != empty
        if(l->next == NULL){            // list->next == empty
            Buckets_List *new_listnode;
            create_Buckets_List(&new_listnode);
            l->next = new_listnode;
            new_listnode->previous = l;
            new_listnode->bucket = bucket;
        }
        else{                           // list->next != empty
            add_Buckets_List_node(l->next, bucket);
        }
    }
    else{                               // list == empty
        l->bucket = bucket;
    }
}

int empty_Buckets_List(Buckets_List *l){
    return (l == NULL);
}

void delete_Buckets_List(Buckets_List *l, int bucket_size, int DEL_WALLET){
    if (!empty_Buckets_List(l)){
        delete_Buckets_List(l->next, bucket_size, DEL_WALLET);
        printf("- DELETE BUCKET LIST NODE\n");
        delete_Bucket(l->bucket, bucket_size, DEL_WALLET);
        free(l);
    }
}

void print_Buckets_List(Buckets_List *l, int bucket_size){
    print_Bucket(l->bucket, bucket_size);
    if (!empty_Buckets_List(l->next)){
        printf("|-->|\n");
        print_Buckets_List(l->next, bucket_size);
    }
}

Buckets_List *get_next_Buckets_List_Node(Buckets_List *l){
    return l->next;  
}

// Bucket

int create_Bucket(Bucket **b, int size_of_bytes){
    int bucket_size = size_of_bytes / sizeof(Bucket);
    //printf("sizeof(Bucket) %ld, bucket_size %d\n", sizeof(Bucket), bucket_size);
    if (bucket_size == 0){
        printf("ERROR: \t THE SIZE OF BYTES DOESN'T FIT FOR A BUCKET\n");
        return 0;
    }
    printf("- CREATE NEW BUCKET\n");
    *b = malloc(bucket_size * sizeof(Bucket));
    int i;
    for (i = 0; i < bucket_size; i++)
        ((*b)[i]).pointer_to_wallet = NULL;
    return bucket_size;
}

void delete_Bucket(Bucket *b, int bucket_size, int DEL_WALLET){
    int i;
    if (DEL_WALLET)
        for (i = 0; i < bucket_size; i++)
            delete_Wallet((b[i]).pointer_to_wallet);
    free(b);
    printf("- DELETE BUCKET\n");
}

void print_Bucket(Bucket *b, int bucket_size){
    int i;
    for (i = 0; i < bucket_size; i++){
        print_Wallet((b[i]).pointer_to_wallet);
    }
}

int search_Bucket_for_name(Bucket *b, char *name, int bucket_size){
    int i = 0;
    for (i = 0; i < bucket_size; i++){
        if ((b[i]).pointer_to_wallet != NULL)
            if (strcmp(((b[i]).pointer_to_wallet)->user_id, name) == 0)
                return 1;
    }
    return 0;
}

Wallet *get_Buckets_record(Bucket *b,char *name,int bucket_size){
    int i = 0;
    for (i = 0; i < bucket_size; i++){
        if ((b[i]).pointer_to_wallet != NULL)
            if (strcmp(((b[i]).pointer_to_wallet)->user_id, name) == 0)
                return (b[i]).pointer_to_wallet;
    }
    return NULL;
}


// Wallet

void create_Wallet(Wallet **w){
    *w = malloc(1*sizeof(Wallet));
    (*w)->user_id = malloc(51 + sizeof(char));
    (*w)->transactions_list = NULL;
    (*w)->bitcoin_t_list = NULL;
}

void init_Wallet(Wallet *w, char *user_id){
    strcpy(w->user_id, user_id);
}

void delete_Wallet(Wallet *w){
    if (w == NULL)
        return;
    if (w->transactions_list != NULL)
        delete_Transactions_List(w->transactions_list, 0);
    if (w->bitcoin_t_list != NULL)
        delete_Bitcoin_Treenodes_List(w->bitcoin_t_list);
    free(w->user_id);
    free(w);
    printf("- DELETE WALLET\n");
}

void print_Wallet(Wallet *w){
    if (w == NULL)
        return;
    printf("USER: \t %s\n", w->user_id);
    if (w->transactions_list != NULL){
        printf("-TRANSACTIONS LIST:\n");
        print_Transactions_List(w->transactions_list);
    }
    if (w->bitcoin_t_list != NULL){
        printf("-BITCOINS LIST:\n");
        print_Bitcoin_Treenodes_List(w->bitcoin_t_list);
    }
}

// Transactions_List

void create_Transactions_List(Transactions_List **l){
    *l = malloc(sizeof(Transactions_List));
    (*l)->transaction = NULL;
    (*l)->next = NULL;
    (*l)->previous = NULL;
}


void add_Transactions_List_node(Transactions_List *l, Transaction *transaction){
    if (l->transaction != NULL){            // list != empty
        if(l->next == NULL){            // list->next == empty
            Transactions_List *new_listnode;
            create_Transactions_List(&new_listnode);
            l->next = new_listnode;
            new_listnode->previous = l;
            new_listnode->transaction = transaction;
        }
        else{                           // list->next != empty
            add_Transactions_List_node(l->next, transaction);
        }
    }
    else{                               // list == empty
        l->transaction = transaction;
    }
}

int empty_Transactions_List(Transactions_List *l){
    return (l == NULL);
}

void delete_Transactions_List(Transactions_List *l, int TRANS_DEL){
    if (!empty_Transactions_List(l)){
        delete_Transactions_List(l->next, TRANS_DEL);
        if (TRANS_DEL)
            if (l->transaction != NULL)
                delete_Transaction(l->transaction);
        free(l);
    }
}

void print_Transactions_List(Transactions_List *l){
    if (l->transaction != NULL)
        print_Transaction(l->transaction);
    if (!empty_Transactions_List(l->next)){
        print_Transactions_List(l->next);
    }
}

// Transaction

void create_Transaction(Transaction **t){
    *t = malloc(sizeof(Transaction));
    (*t)->transaction_id = malloc(16 + sizeof(char));
    (*t)->date_time = malloc(18 * sizeof(char));
    (*t)->size_of_btc_ids = 0;
    (*t)->array_of_pointers_to_btc = NULL;
}

void init_Transaction(Transaction *t, int size_of_btc, char *transaction_id, char *date_time){
    strcpy(t->transaction_id, transaction_id);
    strcpy(t->date_time, date_time);
    t->size_of_btc_ids = size_of_btc;
    t->array_of_pointers_to_btc = malloc((t->size_of_btc_ids) * sizeof(Bitcoin_Tree*));
}

void delete_Transaction(Transaction *t){
    free(t->transaction_id);
    free(t->date_time);
    free(t->array_of_pointers_to_btc);
    free(t);
}

void print_Transaction(Transaction *t){
    printf("TRANSACTION: \t %s, DATE AND TIME: \t %s \n", t->transaction_id, t->date_time);
    int i;
    //printf("SIZE OF BTC IDS %d\n", t->size_of_btc_ids);
    //if (t->size_of_btc_ids = 0)
        for (i = 0; i < t->size_of_btc_ids; i++){
            printf("\t OWNER: \t %s, BITCOIN'S ID: \t %d, BITCOIN'S VALUE: \t %d\n", ((t->array_of_pointers_to_btc)[i])->owner, *(((t->array_of_pointers_to_btc)[i])->btc_id), ((t->array_of_pointers_to_btc)[i])->value);
            // print_Bitcoin_Tree_node((t->array_of_pointers_to_btc)[i]);
        }
}

// Bitcoin

void create_Bitcoin(Bitcoin **btc){
    *btc = malloc(sizeof(Bitcoin));
    (*btc)->btc_id = 0;
    (*btc)->btc_tree = NULL;
}

void init_Bitcoin(Bitcoin *btc, int bitcoin_id, Bitcoin_Tree *btc_tree){
    btc->btc_id = bitcoin_id;
    btc->btc_tree = btc_tree;
}

void delete_Bitcoin(Bitcoin *btc){
    delete_Bitcoin_Tree(btc->btc_tree);
    free(btc);
}

void print_Bitcoin(Bitcoin *btc){
    printf("BITCOIN: \t %d\n", btc->btc_id);
    print_Bitcoin_Tree(btc->btc_tree);
}

// Bitcoin Tree

void create_Bitcoin_Tree(Bitcoin_Tree **t){
    *t = malloc(1*sizeof(Bitcoin_Tree));
    (*t)->owner = NULL;
    (*t)->value = 0;
    (*t)->left_child = NULL;
    (*t)->right_child = NULL;
    (*t)->father = NULL;
    (*t)->btc_id = NULL;
    (*t)->pointer_to_transaction = NULL;
}

void init_Bitcoin_Tree_node(Bitcoin_Tree *t, int value, char *owner, int *btc_id, Transaction *transaction){
    
    if(t->owner != NULL){
        printf("ERROR: \t TRY TO INSERT NEW TREE NODE IN PREVIOUS NODE\n");
    }
    else{
        t->value = value;
        t->owner = owner;
        t->btc_id = btc_id;
        t->pointer_to_transaction = transaction;
    }   // after calling it give the father;
}

int empty_Bitcoin_Tree(Bitcoin_Tree *t){
    return (t == NULL);
}

void delete_Bitcoin_Tree(Bitcoin_Tree *t){
    if (!empty_Bitcoin_Tree(t)){
        delete_Bitcoin_Tree(t->left_child);
        delete_Bitcoin_Tree(t->right_child);
        free(t);
    }
}

void print_Bitcoin_Tree(Bitcoin_Tree *t){
    if (empty_Bitcoin_Tree(t))
        return;
    if (t->btc_id != NULL)
        printf("BITCOIN: \t %d, OWNER: \t %s, VALUE: \t %d\n", *(t->btc_id), t->owner, t->value);
    if (!empty_Bitcoin_Tree(t->father)){
        printf("\t SENDER'S: \t %s\n", (t->father)->owner);
        if ((t->father)->right_child != NULL)
            printf("\t SENDER'S REST: \t %d\n", ((t->father)->right_child)->value);
    }
    print_Bitcoin_Tree(t->left_child);
    print_Bitcoin_Tree(t->right_child);
}

void print_Bitcoin_Tree_node(Bitcoin_Tree *t){
    if (t == NULL)
        return;
    printf("BITCOIN: \t %d, OWNER: \t %s, VALUE: \t %d\n", *(t->btc_id), t->owner, t->value);
    // if (!empty_Bitcoin_Tree(t->father)){
    //     printf("SENDER'S: \t %s\n", (t->father)->owner);
    //     if ((t->father)->right_child != NULL)
    //         printf("SENDER'S REST: \t %d\n", ((t->father)->right_child)->value);
    // }
}

// Bitcoin List

void create_Bitcoins_List(Bitcoins_List **l){
    *l = malloc(sizeof(Bitcoins_List));
    (*l)->bitcoin = NULL;
    (*l)->next = NULL;
    (*l)->previous = NULL;
}


void add_Bitcoins_List_node(Bitcoins_List *l, Bitcoin *bitcoin){
    if (l->bitcoin != NULL){            // list != empty
        if(l->next == NULL){            // list->next == empty
            Bitcoins_List *new_listnode;
            create_Bitcoins_List(&new_listnode);
            l->next = new_listnode;
            new_listnode->previous = l;
            new_listnode->bitcoin = bitcoin;
        }
        else{                           // list->next != empty
            add_Bitcoins_List_node(l->next, bitcoin);
        }
    }
    else{                               // list == empty
        l->bitcoin = bitcoin;
    }
}

int empty_Bitcoins_List(Bitcoins_List *l){
    return (l == NULL);
}

void delete_Bitcoins_List(Bitcoins_List *l){
    if (!empty_Bitcoins_List(l)){
        delete_Bitcoins_List(l->next);
        delete_Bitcoin(l->bitcoin);
        free(l);
    }
}

void print_Bitcoins_List(Bitcoins_List *l){
    print_Bitcoin(l->bitcoin);
    if (!empty_Bitcoins_List(l->next)){
        print_Bitcoins_List(l->next);
    }
}

int search_Bitcoins_List(Bitcoins_List *bitcoin_list, int bitcoin){
    if (bitcoin_list->bitcoin == NULL)
        return 0;
    Bitcoins_List *temp_l = bitcoin_list;
    do{
        if((temp_l->bitcoin)->btc_id == bitcoin)
            return 1;
    }while((temp_l = temp_l->next) != NULL);
    return 0;
}

// Bitcoin Treenodes List

void create_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List **l){
    *l = malloc(sizeof(Bitcoin_Treenodes_List));
    (*l)->bitcoin = NULL;
    (*l)->next = NULL;
    (*l)->previous = NULL;
}


void add_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *l, Bitcoin_Tree *bitcoin){
    if (l->bitcoin != NULL){            // list != empty
        if(l->next == NULL){            // list->next == empty
            Bitcoin_Treenodes_List *new_listnode;
            create_Bitcoin_Treenodes_List(&new_listnode);
            l->next = new_listnode;
            new_listnode->previous = l;
            new_listnode->bitcoin = bitcoin;
        }
        else{                           // list->next != empty
            add_Bitcoin_Treenodes_List_node(l->next, bitcoin);
        }
    }
    else{                               // list == empty
        l->bitcoin = bitcoin;
    }
}

int empty_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l){
    return (l == NULL);
}

void delete_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l){
    if (!empty_Bitcoin_Treenodes_List(l)){
        delete_Bitcoin_Treenodes_List(l->next);
        free(l);
    }
}

void print_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l){
    if (l->bitcoin != NULL)
        print_Bitcoin_Tree_node(l->bitcoin);
    if (!empty_Bitcoin_Treenodes_List(l->next)){
        print_Bitcoin_Treenodes_List(l->next);
    }
}


Bitcoin_Treenodes_List *remove_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *l, Bitcoin_Tree *bitcoin){
    Bitcoin_Treenodes_List *new_l;

    //if(memcmp(l->bitcoin, bitcoin, sizeof(Bitcoin_Tree)) == 0){          // list->name found
    if ( *((l->bitcoin)->btc_id) == *(bitcoin->btc_id) ){
        if(l->previous == NULL){                     // first listnode
            //printf("first node\n");
            if (l->next != NULL){                    // more than one listnodes
                //printf("first node exist more\n");
                (l->next)->previous = NULL;
                new_l = l->next;
            }
            else{                                        // one last node
                new_l = NULL;
                create_Bitcoin_Treenodes_List(&new_l);
                //printf("first node last one\n");
            }
        }
        else if (l->next == NULL){                   // last listnode
            (l->previous)->next = NULL;
            //printf("last node\n");
        }
        else{                                           // middle listnode
            (l->previous)->next = l->next;
            (l->next)->previous = l->previous;
            //printf("middle node\n");
        }
        free(l);
    }
    else{                                               // list->name not here
        if(!empty_Bitcoin_Treenodes_List(l->next)){                // list->next not empty
            //printf("going for next noden\n");
            new_l = l;
            remove_Bitcoin_Treenodes_List_node(l->next, bitcoin);  // go to next
        }
        else                                            // list->next empty
            printf("DIDN'T FOUND %d ON THE Bitcoin_Treenodes_List\n", bitcoin->value);
    }
    return new_l;
}

Bitcoin_Treenodes_List *find_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *lt, int bitcoin){
    if(empty_Bitcoin_Treenodes_List(lt))
        return NULL;
    if (*((lt->bitcoin)->btc_id) == bitcoin)
        return lt;
    else{
        Bitcoin_Treenodes_List *found = find_Bitcoin_Treenodes_List_node(lt->next, bitcoin);
        return found;
    } 
}