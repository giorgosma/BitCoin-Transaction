#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int compare_different_strings(char *name1, char *name2);
void generate_transaction_id(char *Bigest_Transaction_id, char **new_tramsaction_id);
int read_from_file(char *namefile, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, Bitcoins_List *bitcoin_list, int btc_value);
Wallet *insert_new_user(Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char *user_id);
int connect_bucket_with_wallet(Buckets_List *list, Wallet *wallet, int size_of_bucket);
Hash_Table init_Hash_Table(Hash_Table hash_table, int size_of_hash_table, int bucket_size_of_bytes);
int init_Buckets_List(Buckets_List **bucket_list, int bucket_size_of_bytes);
int init_Bucket(Bucket **bucket, int bucket_size_of_bytes);
int delete_all(Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, Bitcoins_List *bitcoin_list, Transactions_List *transactions_list);
int search_hash_table_for_name(Hash_Table hash_table, char *name, int size_of_hash_table);
Bitcoins_List *init_Bitcoins_List(Bitcoins_List *bitcoin_list);
Bitcoin *insert_new_bitcoin(Bitcoins_List *bitcoin_list, int btc_id);
Bitcoin_Tree *init_Bitcoin_Tree(Bitcoin_Tree *btc_tree);
int connect_bitcoin_with_Wallet(Wallet *wallet, Bitcoin *bitcoin, int btc_value);
Transactions_List *init_Transactions_List(Transactions_List *transaction_list);
Transactions_List *read_transactionfile(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *filename, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char **Bigest_Transaction_id);
Transactions_List *get_first_column(Transactions_List *transactions_list);
int search_Transactions_List(Transactions_List *transactions_list, char *transaction_id);
Wallet *get_Hash_Tables_record(Hash_Table hash_table, char *name, int size_of_hash_table);
int calc_senders_balance(Hash_Table users, char *sender, int value, int size_of_hash_table);
int check_transaction_parameters(Transactions_List *transactions_list, char *transaction_id, Hash_Table users, int size_of_hash_table, Hash_Table users2, int size_of_hash_table2, char *sender, char *receiver, int value);
Transaction *insert_new_transaction(Transactions_List *transactions_list, char *transaction_id, char *date_time, int number_of_bitcoins);
int check_valid_date_time(Transaction *Last_Valid_Transaction, char *date_time);
void get_substring(char *string_name, int start, int number_of_characters, char *substring);
void inform_wallets_for_transaction(Hash_Table senders_table, int size_of_senders, Hash_Table receivers_table, int size_of_receivers, Transaction *new_transaction, char *sender, char *receiver, int value);
Bitcoin_Tree *create_btc_tree_children(Bitcoin_Tree *btc_tree_node, int *value, char *receiver, Transaction *transaction);

int read_functions(int size_of_sender_hash_table, int size_of_receiver_hash_table, int bucket_size_of_bytes, int bitcoin_value, char *bitCoinBalancesFile, char *transactionsFile);
Transactions_List *request_Transaction(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *InputLine, 
                                        Hash_Table sender_hash_table, int size_of_sender_hash_table, 
                                        Hash_Table receiver_hash_table, int size_of_receiver_hash_table, char **Bigest_Transaction_id);
Transactions_List *read_requestTransactions_from_file(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *filename, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char **Bigest_Transaction_id);
int request_Transactions(Hash_Table sender_hash_table, int size_of_sender_hash_table, Hash_Table receiver_hash_table, 
                    int size_of_receiver_hash_table, Transactions_List **transactions_list, Transaction **Last_Valid_Transaction,
                    char **Biggest_Transaction_id, char *line_input);

int find_Earnings_Payments(Hash_Table hash_table, int sizeof_hash_table, char *input_line, int EARN_OR_PAY);
int find_Transactions(Wallet *w, int EARN_OR_PAY, char *start_time, char *finish_time, char *start_date, char *finish_date);