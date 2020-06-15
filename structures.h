#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Transaction Transaction;
typedef struct Bitcoins_List Bitcoins_List;
typedef struct Buckets_List Buckets_List;
/*
    BITCOIN
*/

typedef struct Bitcoin_Tree{
    char *owner;                        // pointer to user_id
    int value;                          // bitcoin's current value
    struct Bitcoin_Tree *left_child;    // pointer to receiver of transaction
    struct Bitcoin_Tree *right_child;   // pointer to sender's rest value of bitcoin
    struct Bitcoin_Tree *father;        // pointer to sender of transaction
    unsigned int *btc_id;               // pointer to bitcoin_id
    Transaction *pointer_to_transaction;// pointer to Transaction_node -> transaction_id, date    
}Bitcoin_Tree;

typedef struct Bitcoin{
    unsigned int btc_id;                // bitcoin_id
    Bitcoin_Tree *btc_tree;             // pointer to Bitcoin Tree
}Bitcoin;

typedef struct Bitcoins_List{        
    Bitcoin *bitcoin;                   // pointer to bitcoin , for user's Wallet
    struct Bitcoins_List *next;
    struct Bitcoins_List *previous;
}Bitcoins_List;

typedef struct Bitcoin_Treenodes_List{        
    Bitcoin_Tree *bitcoin;                   // pointer to bitcoin Treenode
    struct Bitcoin_Treenodes_List *next;
    struct Bitcoin_Treenodes_List *previous;
}Bitcoin_Treenodes_List;

/*
    TRANSACTION
*/

typedef struct Transaction{
    char *transaction_id;                // transaction_id
    char *date_time;                            // date and time, will see??
    int size_of_btc_ids;                        // size of array's pointers to Bitcoin Treenodes
    Bitcoin_Tree **array_of_pointers_to_btc;    // array of pointers to Bitcoin Treenodes
}Transaction;

typedef struct Transactions_List{            
    Transaction *transaction;                   // pointer to transaction
    struct Transactions_List *next;          
    struct Transactions_List *previous;
}Transactions_List;

/*
    WALLET
*/

typedef struct Wallet{
    char *user_id;                                  // pointer to user_id
    Transactions_List *transactions_list;     // pointer to Transactions List
    Bitcoin_Treenodes_List *bitcoin_t_list;    // pointer to Bitcoins Treenodes List
}Wallet;

/*
    HASH TABLE
*/

typedef struct Hash_Node{
    Buckets_List *bucket_list;  // cast it to pointer to Bucket List
    int bucket_size_of_bytes;
}Hash_Node;


typedef Hash_Node* Hash_Table;

/*
    BUCKET
*/

typedef struct Bucket{
    Wallet *pointer_to_wallet;
}Bucket;

typedef struct Buckets_List{
    Bucket *bucket;
    struct Buckets_List *next;
    struct Buckets_List *previous;
}Buckets_List;

int hash_function(char *user_id, int size_of_hash_table);
int calc_power(int number, int x);

void create_Hash_Table(Hash_Table *hash_table, int size);
void delete_Hash_Table(Hash_Table hash_table, int size, int DEL_WALLET);
void print_Hash_Table(Hash_Table hash_table, int size, int bucket_size);

void create_Buckets_List(Buckets_List **l);
void add_Buckets_List_node(Buckets_List *l, Bucket *bucket);
Buckets_List *get_last_Buckets_List(Buckets_List *l);
int empty_Buckets_List(Buckets_List *l);
void delete_Buckets_List(Buckets_List *l, int bucket_size, int DEL_WALLET);
void print_Buckets_List(Buckets_List *l, int bucket_size);
Buckets_List *get_next_Buckets_List_Node(Buckets_List *l);

int create_Bucket(Bucket **b, int bucket_size);
void delete_Bucket(Bucket *b, int bucket_size, int DEL_WALLET);
void print_Bucket(Bucket *b, int bucket_size);
int search_Bucket_for_name(Bucket *b, char *name, int bucket_size);
Wallet *get_Buckets_record(Bucket *b,char *name,int bucket_size);

void create_Wallet(Wallet **w);
void init_Wallet(Wallet *w, char *user_id);
void delete_Wallet(Wallet *w);
void print_Wallet(Wallet *w);

void create_Transactions_List(Transactions_List **l);
void add_Transactions_List_node(Transactions_List *l, Transaction *transaction);
int empty_Transactions_List(Transactions_List *l);
void delete_Transactions_List(Transactions_List *l, int TRANS_DEL);
void print_Transactions_List(Transactions_List *l);

void create_Transaction(Transaction **t);
void init_Transaction(Transaction *t, int size_of_btc, char *transaction_id, char *date_time);
void delete_Transaction(Transaction *t);
void print_Transaction(Transaction *t);

void create_Bitcoin(Bitcoin **btc);
void init_Bitcoin(Bitcoin *btc, int bitcoin_id, Bitcoin_Tree *btc_tree);
void delete_Bitcoin(Bitcoin *btc);
void print_Bitcoin(Bitcoin *btc);

void create_Bitcoin_Tree(Bitcoin_Tree **t);
void init_Bitcoin_Tree_node(Bitcoin_Tree *t, int value, char *owner, int *btc_id, Transaction *transaction);
int empty_Bitcoin_Tree(Bitcoin_Tree *t);
void delete_Bitcoin_Tree(Bitcoin_Tree *t);
void print_Bitcoin_Tree(Bitcoin_Tree *t);
void print_Bitcoin_Tree_node(Bitcoin_Tree *t);

void create_Bitcoins_List(Bitcoins_List **l);
void add_Bitcoins_List_node(Bitcoins_List *l, Bitcoin *bitcoin);
int empty_Bitcoins_List(Bitcoins_List *l);
void delete_Bitcoins_List(Bitcoins_List *l);
int search_Bitcoins_List(Bitcoins_List *bitcoin_list, int bitcoin);
void print_Bitcoins_List(Bitcoins_List *l);

void create_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List **l);
void add_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *l, Bitcoin_Tree *bitcoin);
int empty_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l);
void delete_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l);
void print_Bitcoin_Treenodes_List(Bitcoin_Treenodes_List *l);
Bitcoin_Treenodes_List *remove_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *l, Bitcoin_Tree *bitcoin);
Bitcoin_Treenodes_List *find_Bitcoin_Treenodes_List_node(Bitcoin_Treenodes_List *lt, int bitcoin);