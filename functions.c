#include "structures.h"
#include "functions.h"

int compare_different_strings(char *name1, char *name2){
    if (strlen(name1) > strlen(name2))
        return 1;
    else if (strlen(name1) < strlen(name2))
        return 0;
    else{
        int ret = strcmp(name1, name2);
        if (ret > 0)
            return 1;
        else 
            return 0;
    }
}

void generate_transaction_id(char *Bigest_Transaction_id, char **new_tramsaction_id){
    *new_tramsaction_id = malloc(16*sizeof(char));
    strcpy(*new_tramsaction_id, Bigest_Transaction_id);
    //printf("STRLEN: %ld\n", strlen(Bigest_Transaction_id));
    if (strlen(Bigest_Transaction_id) < 15){
        // *new_tramsaction_id = malloc((strlen(Bigest_Transaction_id) + 1)*sizeof(char));
        // strcpy(*new_tramsaction_id, Bigest_Transaction_id);
        //(*new_tramsaction_id)[strlen(Bigest_Transaction_id)] = '0';
        strcat(*new_tramsaction_id, "0");
        //(*new_tramsaction_id)[strlen(Bigest_Transaction_id)+1] = '\0';
    }
    else{
        int i, ascci_token;
        for (i = 15; i > 0; i--){
            ascci_token = (*new_tramsaction_id)[i];
            if ((ascci_token >= 48 && ascci_token < 57) || (ascci_token >= 65 && ascci_token < 90) || (ascci_token >= 97 && ascci_token < 122)){
                ((*new_tramsaction_id)[i])++;
                break;
            }
        }        
    }
}

int read_from_file(char *namefile, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, Bitcoins_List *bitcoin_list, int btc_value){
    int bucket_size;
    int i;

    char *InputLine = NULL;
    FILE *fp_in;
    size_t sz = 0;
    fp_in = fopen(namefile,"r");
    while(getline(&InputLine, &sz, fp_in) != EOF){      //get the whole line from file
        const char s[2] = " ";
        char *token;
        token = strtok(InputLine, s);
        int first_key = 1;
        while( token != NULL) {
            Wallet *user_wallet;
            if (first_key){
                char name[50];
                sscanf(InputLine, "%s", name);
                printf("INSERT NEW USER: \t %s \n", name);
                if(search_hash_table_for_name(hash_table, name, size_of_hash_table)){
                    printf("ERROR:\n \t NAME: %s ALREADY EXISTS IN USERS BASE\n", name);
                    token = NULL;
                    continue;
                }
                else{
                    user_wallet = insert_new_user(hash_table, size_of_hash_table, hash_table2, size_of_hash_table2, name);
                    first_key = 0;
                }
            }
            else{
                sscanf(token, "%d", &i);
                if(search_Bitcoins_List(bitcoin_list, i))
                    printf("ERROR: \n \t BITCOIN %d ALREADY EXISTS IN BICOINS BASE\n", i);
                else{
                    printf("INSERT NEW BITCOIN: \t %d \n", i);
                    Bitcoin *btc = insert_new_bitcoin(bitcoin_list, i);
                    connect_bitcoin_with_Wallet(user_wallet, btc, btc_value);
                }
            }
            token = strtok(NULL, s);
        }
        first_key = 1;  
        printf("\n");     
    }
    free(InputLine);
    fclose(fp_in);
    return bucket_size;
}


Wallet *insert_new_user(Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char *user_id){
    Wallet *w = NULL;
    create_Wallet(&w);
    init_Wallet(w, user_id);
    create_Transactions_List(&(w->transactions_list));
    create_Bitcoin_Treenodes_List(&(w->bitcoin_t_list));
    
    int i = hash_function(user_id, size_of_hash_table);
    int i2 = hash_function(user_id, size_of_hash_table2);
    printf("SENDER'S HASH SEAT: \t %d\n", i);
    printf("RECEIVER'S HASH SEAT: \t %d\n", i2);
    
    int size_of_bucket = (hash_table[i]).bucket_size_of_bytes / sizeof(Bucket);
    connect_bucket_with_wallet((hash_table[i]).bucket_list, w, size_of_bucket);    
    size_of_bucket = (hash_table2[i2]).bucket_size_of_bytes / sizeof(Bucket);
    connect_bucket_with_wallet((hash_table2[i2]).bucket_list, w, size_of_bucket);
    return w;
}

int connect_bucket_with_wallet(Buckets_List *list, Wallet *wallet, int size_of_bucket){
    Buckets_List *temp_list = list;

    int j, seat = 0;
    while (seat == 0){
        for (j = 0; j < size_of_bucket; j++){
            if ( (temp_list->bucket[j]).pointer_to_wallet == NULL){
                (temp_list->bucket[j]).pointer_to_wallet = wallet;
                seat = 1;
                printf("ADD WALLET TO BUCKET LISTNODE\n");
                break;
            }
        }
        if (seat == 0){
            if (temp_list->next == NULL){
                printf("CREATE NEW BUCKET LIST NODE\n");
                Bucket *bucket;
                init_Bucket(&bucket, (size_of_bucket * sizeof(Bucket)));
                add_Buckets_List_node(temp_list, bucket);
                temp_list = get_last_Buckets_List(temp_list);
            }
            else{
                printf("GO TO NEXT BUCKET LIST NODE\n");
                temp_list = temp_list->next;
            }
        }
    }
}

Hash_Table init_Hash_Table(Hash_Table hash_table, int size_of_hash_table, int bucket_size_of_bytes){
    create_Hash_Table(&hash_table, size_of_hash_table);
    int i, size_of_bucket;
    for (i = 0; i < size_of_hash_table; i++){
        (hash_table[i]).bucket_size_of_bytes = bucket_size_of_bytes;
        size_of_bucket = init_Buckets_List( &((hash_table[i]).bucket_list), bucket_size_of_bytes);
    }
    return hash_table;
}

int init_Buckets_List(Buckets_List **bucket_list, int bucket_size_of_bytes){
    create_Buckets_List(&(*bucket_list));
    Bucket *bucket;
    int size_of_bucket = init_Bucket(&bucket, bucket_size_of_bytes);
    add_Buckets_List_node(*bucket_list, bucket);
    return size_of_bucket;
}

int init_Bucket(Bucket **bucket, int bucket_size_of_bytes){
    int size_of_bucket = create_Bucket(&(*bucket), bucket_size_of_bytes);
    return size_of_bucket;
}

int delete_all(Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, Bitcoins_List *bitcoin_list, Transactions_List *transactions_list){
    delete_Hash_Table(hash_table, size_of_hash_table, 1);
    delete_Hash_Table(hash_table2, size_of_hash_table2, 0);
    delete_Bitcoins_List(bitcoin_list);
    delete_Transactions_List(transactions_list, 1);
}

int search_hash_table_for_name(Hash_Table hash_table, char *name, int size_of_hash_table){
    int i = hash_function(name, size_of_hash_table);
    Buckets_List *temp_l = hash_table[i].bucket_list;
    do{
        if(search_Bucket_for_name(temp_l->bucket, name, (hash_table[i]).bucket_size_of_bytes / sizeof(Bucket)))
            return 1;
    }while((temp_l = get_next_Buckets_List_Node(temp_l)) != NULL);
    return 0;
}

Bitcoins_List *init_Bitcoins_List(Bitcoins_List *bitcoin_list){
    create_Bitcoins_List(&bitcoin_list);
    return bitcoin_list;
}

Bitcoin *insert_new_bitcoin(Bitcoins_List *bitcoin_list, int btc_id){
    Bitcoin *bitcoin;
    create_Bitcoin(&bitcoin);
    Bitcoin_Tree *btc_tree = NULL;
    btc_tree = init_Bitcoin_Tree(btc_tree);
    init_Bitcoin(bitcoin, btc_id, btc_tree);
    add_Bitcoins_List_node(bitcoin_list, bitcoin);
    return bitcoin;
}

Bitcoin_Tree *init_Bitcoin_Tree(Bitcoin_Tree *btc_tree){
    create_Bitcoin_Tree(&btc_tree);
    return btc_tree;
}

int connect_bitcoin_with_Wallet(Wallet *wallet, Bitcoin *bitcoin, int btc_value){
    (bitcoin->btc_tree)->btc_id = &(bitcoin->btc_id);
    (bitcoin->btc_tree)->owner = wallet->user_id;
    (bitcoin->btc_tree)->value = btc_value;
    add_Bitcoin_Treenodes_List_node(wallet->bitcoin_t_list, bitcoin->btc_tree); // Tree's root
}

Transactions_List *init_Transactions_List(Transactions_List *transaction_list){
    create_Transactions_List(&transaction_list);
    return transaction_list;
}

int check_transaction_parameters(Transactions_List *transactions_list, char *transaction_id, Hash_Table users, int size_of_hash_table, Hash_Table users2, int size_of_hash_table2, char *sender, char *receiver, int value){
    int num_of_btc;
    if (search_Transactions_List(transactions_list, transaction_id))                    // we want 1, did'nt found
        if (search_hash_table_for_name(users, sender, size_of_hash_table))              // we get sender
            if (search_hash_table_for_name(users2, receiver, size_of_hash_table2))        // we get receiver
                if ((num_of_btc = calc_senders_balance(users, sender, value, size_of_hash_table)))     // we want 1
                    return num_of_btc;
    return 0;
}

Transactions_List *read_transactionfile(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *filename, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char **Bigest_Transaction_id){
    int bucket_size;
    int i;
    //Transaction *Last_Valid_Transaction = NULL;

    char *InputLine = NULL;
    FILE *fp_in;
    size_t sz = 0;
    fp_in = fopen(filename,"r");
    char transaction_id[16], sender[51], receiver[51], ddate[11], ttime[6];
        int value = 0;
        int key = 1;
    while(getline(&InputLine, &sz, fp_in) != EOF){      //get the whole line from file
        char val[20];
        const char s[2] = " ";
        char *token;
        token = strtok(InputLine, s);
        key = 1;
        while( token != NULL) {
            switch (key){
                case 1: strcpy(transaction_id, token);
                    break;
                case 2: strcpy(sender, token);
                    break;
                case 3: strcpy(receiver, token);
                    break;
                case 4: value = atoi(token);
                    break;
                case 5: strcpy(ddate, token);
                    break;
                case 6: sscanf(token, "%s", ttime);
                    break;
                default: printf("NOTHING\n");
                    break;
            }
            //printf("KEY: %d, TOKEN: %s\n", key, token);
            key++;
            token = strtok(NULL, s);
        }
        int number_of_bitcoins = 0;
        if((number_of_bitcoins = check_transaction_parameters(transactions_list, transaction_id, hash_table, size_of_hash_table, hash_table2, size_of_hash_table2, sender, receiver, value)) == 0)
            continue;        
        //printf("NUM_OF_BITCOINS: %d\n",number_of_bitcoins);
        char date_time[20];
        if (key == 5){
            time_t rawtime;
            struct tm *tm;
            time(&rawtime);
            tm = localtime(&rawtime);
            sprintf (date_time, "%02d-%02d-%04d %02d:%02d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min);
        }
        else
            sprintf (date_time, "%s %s", ddate, ttime);
        printf("TRYING TO INSERT NEW TRANSACTION WITH PARAMETERS:\n \t %s %s %s %d %s \n", transaction_id, sender, receiver, value, date_time);
        Transaction *new_transaction;
        if ( check_valid_date_time(*Last_Valid_Transaction, date_time) ){
            new_transaction = insert_new_transaction(transactions_list, transaction_id, date_time, number_of_bitcoins);
            inform_wallets_for_transaction(hash_table, size_of_hash_table, hash_table2, size_of_hash_table2, new_transaction, sender, receiver, value);
            *Last_Valid_Transaction = new_transaction;
            if (*Bigest_Transaction_id == NULL)
                *Bigest_Transaction_id = new_transaction->transaction_id;
            else{
                //printf("transaction_id: %s, Bigest_Transaction_id: %s \n", new_transaction->transaction_id, *Bigest_Transaction_id);
                if (compare_different_strings(new_transaction->transaction_id, *Bigest_Transaction_id)){    
                    *Bigest_Transaction_id = new_transaction->transaction_id;
                    //printf("NEW BIGEST: %s \n", *Bigest_Transaction_id);
                }
            }
            
        }
        else 
            printf("ERROR: \n \t LAST VALID TRANSACTION HAS DONE AFTER THIS ONE\n");
        
    }
    free(InputLine);
    fclose(fp_in);
    return transactions_list;
}

Transaction *insert_new_transaction(Transactions_List *transactions_list, char *transaction_id, char *date_time, int number_of_bitcoins){
    Transaction *new_transaction;
    create_Transaction(&new_transaction);
    init_Transaction(new_transaction, number_of_bitcoins, transaction_id, date_time);
    add_Transactions_List_node(transactions_list, new_transaction);
    return new_transaction;
}

Transactions_List *get_first_column(Transactions_List *transactions_list){
    while(transactions_list->previous != NULL)
        transactions_list = transactions_list->previous;
    return transactions_list;
}

int search_Transactions_List(Transactions_List *transactions_list, char *transaction_id){
    int ret = -1;
    Transactions_List *temp_l = transactions_list;
    
    while(temp_l != NULL){
        if (temp_l->transaction == NULL)
            return 1;
        ret = strcmp((temp_l->transaction)->transaction_id, transaction_id);
        if (ret == 0){
            printf("ERROR: \n \t THIS TRANSACTION ID ALREADY EXISTS IN TRANSACTION LIST\n");
            return 0;
        }
        temp_l = temp_l->next;  
    }
    return 1;
}

Wallet *get_Hash_Tables_record(Hash_Table hash_table, char *name, int size_of_hash_table){
    int i = hash_function(name, size_of_hash_table);
    Buckets_List *temp_l = hash_table[i].bucket_list;
    Wallet *w = NULL;
    do{
        if ((w = get_Buckets_record(temp_l->bucket, name, (hash_table[i]).bucket_size_of_bytes / sizeof(Bucket))) != NULL)
            return w;
    }while((temp_l = get_next_Buckets_List_Node(temp_l)) != NULL);
    return NULL;
}

int calc_senders_balance(Hash_Table users, char *sender, int value, int size_of_hash_table){
    Wallet *w = get_Hash_Tables_record(users, sender, size_of_hash_table);
    if (w != NULL){
        int sum = 0, number_of_bitcoins = 0;
        Bitcoin_Treenodes_List *btc_t_list = w->bitcoin_t_list;
        while(btc_t_list != NULL){
            if (btc_t_list->bitcoin == NULL)
                return 0;
            sum += btc_t_list->bitcoin->value;
            number_of_bitcoins++;
            if (sum >= value)
                return number_of_bitcoins;
            btc_t_list = btc_t_list->next;
        }
        return 0;
    }
}


void get_substring(char *string_name, int start, int number_of_characters, char *substring){
    memcpy(substring, &string_name[start], number_of_characters);
    (substring)[number_of_characters] = '\0';
}

int check_strings(char *name1, char *name2){
    int ret = strcmp(name1, name2);
    return ret;
}
int check_valid_date_time(Transaction *Last_Valid_Transaction, char *date_time){
    // get date time and checking
    char year[5], month[3], day[3], ttime[6];
    char l_year[5], l_month[3], l_day[3], l_ttime[6];
    if (Last_Valid_Transaction == NULL)
        return 1;
    get_substring(date_time, 6, 4, year);
    get_substring(Last_Valid_Transaction->date_time, 6, 4, l_year);
    int res = check_strings(l_year, year);
    if (res < 0)
        return 1;
    else if (res == 0){
        get_substring(date_time, 3, 2, month);
        get_substring(Last_Valid_Transaction->date_time, 3, 2, l_month);
        res = check_strings(l_month, month);
        if (res < 0)
            return 1;
        else if (res == 0){
            get_substring(date_time, 0, 2, day);
            get_substring(Last_Valid_Transaction->date_time, 0, 2, l_day);
            res = check_strings(l_day, day);
            if (res < 0)
                return 1;
            else if (res == 0){
                get_substring(date_time, 11, 5, ttime);
                get_substring(Last_Valid_Transaction->date_time, 11, 5, l_ttime);
                res = check_strings(l_ttime, ttime);
                if (res < 0)
                    return 1;
                else if (res == 0)
                    return 1;
            }
        }
    }
    return 0;
}

void inform_wallets_for_transaction(Hash_Table senders_table, int size_of_senders, Hash_Table receivers_table, int size_of_receivers, Transaction *new_transaction, char *sender, char *receiver, int value){
    Wallet *w_sender = get_Hash_Tables_record(senders_table, sender, size_of_senders),
        *w_receiver  = get_Hash_Tables_record(receivers_table, receiver, size_of_receivers);
    
    add_Transactions_List_node(w_sender->transactions_list, new_transaction);
    add_Transactions_List_node(w_receiver->transactions_list, new_transaction);

    int i;
    Bitcoin_Treenodes_List *sender_btc;
    Bitcoin_Tree *pointer_to_bitcoin_tree_new_node;
    for (i = 0; i < new_transaction->size_of_btc_ids; i++){
        sender_btc = w_sender->bitcoin_t_list;

        pointer_to_bitcoin_tree_new_node = create_btc_tree_children(sender_btc->bitcoin, &value, w_receiver->user_id, new_transaction);
        (new_transaction->array_of_pointers_to_btc)[i] = pointer_to_bitcoin_tree_new_node;
        add_Bitcoin_Treenodes_List_node(w_receiver->bitcoin_t_list, pointer_to_bitcoin_tree_new_node); // add receivers bitcoin treenode list new tree_node_list 
        
        if((sender_btc->bitcoin)->right_child != NULL)
            sender_btc->bitcoin = (sender_btc->bitcoin)->right_child;
        else
            w_sender->bitcoin_t_list = remove_Bitcoin_Treenodes_List_node(w_sender->bitcoin_t_list, sender_btc->bitcoin);
    }
}

Bitcoin_Tree *create_btc_tree_children(Bitcoin_Tree *btc_tree_node, int *value, char *receiver, Transaction *transaction){
    
    Bitcoin_Tree *new_tree_node;
    new_tree_node = init_Bitcoin_Tree(new_tree_node);
    btc_tree_node->left_child = new_tree_node;
    new_tree_node->father = btc_tree_node;
    new_tree_node->owner = receiver;
    new_tree_node->pointer_to_transaction = transaction;
    new_tree_node->btc_id = btc_tree_node->btc_id;
    
    int rest_value = btc_tree_node->value - *value;
    if (rest_value <= 0){
        // all to receiver
        new_tree_node->value = btc_tree_node->value;
        *value = rest_value * (-1);
    }
    else{
        // rest_value to sender
        new_tree_node->value = *value;
        Bitcoin_Tree *senders_rest_btc;
        senders_rest_btc = init_Bitcoin_Tree(senders_rest_btc);
        btc_tree_node->right_child = senders_rest_btc;
        //senders_rest_btc->father = btc_tree_node;
        senders_rest_btc->owner = btc_tree_node->owner;
        //senders_rest_btc->pointer_to_transaction = transaction;
        senders_rest_btc->btc_id = btc_tree_node->btc_id;
        senders_rest_btc->value = rest_value;

        new_tree_node->value = *value;
    }
    return new_tree_node;

}

Transactions_List *request_Transaction(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *InputLine, 
                                        Hash_Table sender_hash_table, int size_of_sender_hash_table, 
                                        Hash_Table receiver_hash_table, int size_of_receiver_hash_table, char **Bigest_Transaction_id){

    char sender[51], receiver[51], date_time[20], ddate[11], ttime[6], ttime_and_semicolon[7];
    const char s[2] = " ";
    char *token, val[50];
    token = strtok(InputLine, s);
    int key = 0;
    int value = 0;
    while( token != NULL) {
        switch (key){
            case 0: key = 0;
                break;
            case 1: strcpy(sender, token);
                break;
            case 2: strcpy(receiver, token);
                break;
            case 3: value = atoi(token);
                break;
            case 4: strcpy(ddate, token);
                break;
            case 5: sscanf(token, "%s", ttime_and_semicolon);
                break;
            default: printf("NOTHING\n");
                break;
        }
        key++;
        token = strtok(NULL, s);
    }
    if (key == 4){        
        time_t rawtime;
        struct tm *tm;
        time(&rawtime);
        tm = localtime(&rawtime);
        sprintf (date_time, "%02d-%02d-%04d %02d:%02d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min);
    }
    else{
        memcpy(ttime, ttime_and_semicolon, 5);
        ttime[5] = '\0';
        sprintf (date_time, "%s %s", ddate, ttime);
    }
    int number_of_bitcoins = 0;
    char *new_tramsaction_id = NULL;
    generate_transaction_id(*Bigest_Transaction_id, &new_tramsaction_id);
    printf("TRYING TO INSERT NEW TRANSACTION WITH PARAMETERS:\n \t %s %s %s %d %s \n", new_tramsaction_id, sender, receiver, value, date_time);
    if((number_of_bitcoins = check_transaction_parameters(transactions_list, new_tramsaction_id, sender_hash_table, size_of_sender_hash_table, receiver_hash_table, size_of_receiver_hash_table, sender, receiver, value)) == 0){
        printf("ERROR: \n \t PARAMETERS DOES NOT MATCH WITH APPLICATION INFORMATION\n");
        free(new_tramsaction_id);
        return transactions_list;       
    }
    //printf("NUM_OF_BITCOINS: %d\n",number_of_bitcoins);
    
    Transaction *new_transaction;
    if ( check_valid_date_time(*Last_Valid_Transaction, date_time) ){
        new_transaction = insert_new_transaction(transactions_list, new_tramsaction_id, date_time, number_of_bitcoins);
        inform_wallets_for_transaction(sender_hash_table, size_of_sender_hash_table, receiver_hash_table, size_of_receiver_hash_table, new_transaction, sender, receiver, value);
        *Last_Valid_Transaction = new_transaction;
        if (*Bigest_Transaction_id == NULL)
            *Bigest_Transaction_id = new_transaction->transaction_id;
        else{
            //printf("transaction_id: %s, Bigest_Transaction_id: %s \n", new_transaction->transaction_id, *Bigest_Transaction_id);
            if (compare_different_strings(new_transaction->transaction_id, *Bigest_Transaction_id)){    
                *Bigest_Transaction_id = new_transaction->transaction_id;
                //printf("NEW BIGEST: %s \n", *Bigest_Transaction_id);
            }
        }        
    }
    else 
        printf("ERROR: \n \t LAST VALID TRANSACTION HAS DONE AFTER THIS ONE\n");
    free(new_tramsaction_id);
    return transactions_list; 
}

Transactions_List *read_requestTransactions_from_file(Transactions_List *transactions_list, Transaction **Last_Valid_Transaction, char *input_line, Hash_Table hash_table, int size_of_hash_table, Hash_Table hash_table2, int size_of_hash_table2, char **Bigest_Transaction_id){
    int bucket_size;
    int i;

    char *InputLine = NULL;
    FILE *fp_in;
    size_t sz = 0;
    char filename[50];
    sscanf(input_line, "%*s %s", filename);
    fp_in = fopen(filename,"r");
    char sender[51], receiver[51], date_time[20], ddate[11], ttime[6], ttime_and_semicolon[7];
    int value = 0;
    int key = 1;
    while(getline(&InputLine, &sz, fp_in) != EOF){      //get the whole line from file   
        char p_line[200];
        strcpy(p_line, "/requestTransaction ");
        strcat(p_line, InputLine);
        transactions_list = request_Transaction(transactions_list, Last_Valid_Transaction, p_line, hash_table, size_of_hash_table, hash_table2, 
            size_of_hash_table2, Bigest_Transaction_id);
    }
    free(InputLine);
    fclose(fp_in);
    return transactions_list;
}

int request_Transactions(Hash_Table sender_hash_table, int size_of_sender_hash_table, Hash_Table receiver_hash_table, 
                    int size_of_receiver_hash_table, Transactions_List **transactions_list, Transaction **Last_Valid_Transaction,
                    char **Biggest_Transaction_id, char *line_input) {

    char *len = NULL;
    len = strstr(line_input, ";");
    //printf("len : %s\n", len);
    if (len == NULL){
        
        *transactions_list = read_requestTransactions_from_file(*transactions_list, Last_Valid_Transaction, line_input, 
                            sender_hash_table, size_of_sender_hash_table, 
                            receiver_hash_table, size_of_receiver_hash_table, Biggest_Transaction_id);
    }
    else{
        //printf("line_input: %s", line_input);
        *transactions_list = request_Transaction(*transactions_list, Last_Valid_Transaction, line_input, 
                                sender_hash_table, size_of_sender_hash_table, 
                                receiver_hash_table, size_of_receiver_hash_table, Biggest_Transaction_id);
        char *InputLine = NULL;
        size_t sz = 0;
        while(getline(&InputLine, &sz, stdin) != -1){
            if (InputLine[0] == '\n')
                break;
            //printf("INPUTLINE: %s", InputLine);
            char p_line[200];
            strcpy(p_line, "/requestTransactions ");
            strcat(p_line, InputLine);
            //printf("p_line: %s", p_line);
            *transactions_list = request_Transaction(*transactions_list, Last_Valid_Transaction, p_line, 
                                sender_hash_table, size_of_sender_hash_table, 
                                receiver_hash_table, size_of_receiver_hash_table, Biggest_Transaction_id);
        }
        free(InputLine);
    }
}

int check_date(Transaction *Last_Valid_Transaction, char *date){    // DATE > LAST_TRANSACTION
    // get date time and checking
    char year[5], month[3], day[3];
    char l_year[5], l_month[3], l_day[3];
    if (Last_Valid_Transaction == NULL)
        return 1;
    get_substring(date, 6, 4, year);
    get_substring(Last_Valid_Transaction->date_time, 6, 4, l_year);
    int res = check_strings(l_year, year);
    if (res < 0)
        return 1;
    else if (res == 0){
        get_substring(date, 3, 2, month);
        get_substring(Last_Valid_Transaction->date_time, 3, 2, l_month);
        res = check_strings(l_month, month);
        if (res < 0)
            return 1;
        else if (res == 0){
            get_substring(date, 0, 2, day);
            get_substring(Last_Valid_Transaction->date_time, 0, 2, l_day);
            res = check_strings(l_day, day);
            if (res < 0)
                return 1;
            else if (res == 0){
                //check for time if exists
                //else if time not exists return 1;
                // if (ttime[0] != 0){
                //     check_time(Last_Valid_Transaction, ttime);
                // }
                // else
                    return 0;  
            }
        }
    }
    return -1;
}

int check_time(Transaction *Last_Valid_Transaction, char *ttime){    // DATE > LAST_TRANSACTION
    char l_ttime[6];
    if (Last_Valid_Transaction == NULL)
        return 1;
    get_substring(Last_Valid_Transaction->date_time, 11, 5, l_ttime);
    int res = check_strings(l_ttime, ttime);
    if (res < 0)
        return 1;
    else if (res == 0)
        return 1;
    else
        return 0;
}

int find_Transactions(Wallet *w, int EARN_OR_PAY, char *start_time, char *finish_time, char *start_date, char *finish_date){
    Transactions_List *temp_l = w->transactions_list;
    if (temp_l->transaction == NULL)
        return 0;
    int sum = 0;
    do{
        char year[5], month[3], day[3], ttime[6];
        char l_year[5], l_month[3], l_day[3], l_ttime[6];
        int res = -1;
        int st_date = 0, fn_date = 0, st_time = 0, fn_time = 0;
        if (start_date[0] != 0){    // if date
            st_date = check_date(temp_l->transaction, start_date);
            if (st_date == 0){     // if start date == date 
                if (start_time[0] != 0){    // if exists time check time
                    if (check_time(temp_l->transaction, start_time) == 0)
                        res = 1;
                    else
                        res = 0;
                }
                else    // if time not exist then ok
                    res = 1;
            }
            else if (st_date == -1)
                res = 1;
            else
                res = 0;
            fn_date = check_date(temp_l->transaction, finish_date);
            if (fn_date == 0){     // if start date == date 
                if (finish_time[0] != 0){    // if exists time check time
                    if (check_time(temp_l->transaction, finish_time) == 1)
                        res = 1;
                    else
                        res = 0;
                }
                else    // if time not exist then ok
                    res = 1;
            }
            else if (fn_date == 1)
                res = 1;
            else
                res = 0;
        }
        else if (start_time[0] != 0){   // only if time
            if (check_time(temp_l->transaction, start_time) == 0 && check_time(temp_l->transaction, finish_time) == 1)
                res = 1;
            else 
                res = 0;
        }
        else
            res = 1;
        
        
        if (res){
            // check for sender or receiver and ++
            int key = 0;
            if (EARN_OR_PAY){ // SENDER
                if (strcmp(w->user_id, ((((temp_l->transaction)->array_of_pointers_to_btc)[0])->father)->owner) == 0)
                    key = 1;
                else
                    key = 0;
                
            }
            else{   // RECEIVER
                if (strcmp(w->user_id, (((temp_l->transaction)->array_of_pointers_to_btc)[0])->owner) == 0)
                    key = 1;
                else
                    key = 0;
            }
            int i, temp_sum = 0;
            if (key){
                for (i = 0; i < (temp_l->transaction)->size_of_btc_ids; i++){
                    temp_sum += (((temp_l->transaction)->array_of_pointers_to_btc)[i])->value;
                }
                sum += temp_sum;
                printf("TRANSACTION: %s USER: %s SENT TO USER: %s VALUE: %d ON DATE: %s\n", temp_l->transaction->transaction_id, ((((temp_l->transaction)->array_of_pointers_to_btc)[0])->father)->owner, (((temp_l->transaction)->array_of_pointers_to_btc)[0])->owner, temp_sum, (temp_l->transaction)->date_time);    
            }
        }
        temp_l = temp_l->next;
    }while (temp_l != NULL);
    if (sum != 0){
        if (EARN_OR_PAY)
            printf("SUM OF PAYMENTS: %d\n", sum);
        else
            printf("SUM OF EARNINGS: %d\n", sum);
        return 1;
    }
    return 0;
}

int find_Earnings_Payments(Hash_Table hash_table, int sizeof_hash_table, char *input_line, int EARN_OR_PAY){
    char wallet_id[51], start_time[6] = "", start_date[11] = "", finish_time[6] = "", finish_date[11] = "", date_or_time[20] = "";
    const char s[2] = " ";
    char *token;
    token = strtok(input_line, s);
    int key = 0;
    int value = 0;
    while( token != NULL) {
        switch (key){
            case 0: key = 0;
                break;
            case 1: sscanf(token, "%s", wallet_id);
                break;
            case 2: sscanf(token, "%s", date_or_time);
                break;
            case 3: sscanf(token, "%s", date_or_time);
                break;
            case 4: sscanf(token, "%s", date_or_time);
                break;
            case 5: sscanf(token, "%s", date_or_time);
                break;
            default: printf("NOTHING\n");
                break;
        }
        //printf("key: %d, token: %s \n", key, token);
        if (key > 1){
            if (date_or_time[3] == ':'){
                if (start_time[2] != ':'){
                    memcpy(start_time, &date_or_time[1], 5);
                    start_time[5] = '\0';
                }
                else {
                    memcpy(finish_time, &date_or_time[1], 5);
                    finish_time[5] = '\0';
                }
            }
            else if (date_or_time[3] == '-'){
                if (start_date[2] != '-'){
                    memcpy(start_date, &date_or_time[1], 10);
                    start_date[10] = '\0';
                }
                else {
                    memcpy(finish_date, &date_or_time[1], 10);
                    finish_date[10] = '\0';
                }
            }
        }
        key++;
        token = strtok(NULL, s);
    }
    printf("WALLET: %s", wallet_id);
    if (start_time[0] != 0)
        printf(", START TIME: %s", start_time);
    if (start_date[0] != 0)
        printf(", START DATE: %s", start_date);
    if (finish_time[0] != 0)
        printf(", FINISH TIME: %s", finish_time);
    if (finish_date[0] != 0)
        printf(", FINISH DATE: %s", finish_date);
    printf("\n");
    Wallet *w = get_Hash_Tables_record(hash_table, wallet_id, sizeof_hash_table);
    if (w != NULL){
        if(!find_Transactions(w, EARN_OR_PAY, start_time, finish_time, start_date, finish_date))
            printf("THERE ARE NOT TRANSACTIONS BETWEEN %s %s %s %s\n", start_time, start_date, finish_time, finish_date);
    }
    else 
        printf("ERROR: \n \t THERE IS NOT WALLET FOR USER: %s\n", wallet_id);
}

int calc_wallet_balance(Wallet *w){
    Bitcoin_Treenodes_List *temp_l = w->bitcoin_t_list;
    int sum = 0;
    while(temp_l != NULL){
        if (temp_l->bitcoin == NULL)
            break;
        sum += (temp_l->bitcoin)->value;
        temp_l = temp_l->next;
    }
    return sum;
}

int walletStatus(Hash_Table hash_table, int sizeof_hash_table, char *wallet_id){
    Wallet *w = get_Hash_Tables_record(hash_table, wallet_id, sizeof_hash_table);
    if (w != NULL){
        int balance = calc_wallet_balance(w);
        printf("USER: %s HAS WALLET STATUS: %d\n", w->user_id, balance);
    }
    else 
        printf("ERROR: \n \t THERE IS NOT WALLET FOR USER: %s\n", wallet_id);
    
}
Bitcoin *get_Bitcoin(Bitcoins_List *bitcoin_list, int btc_id){
    Bitcoins_List *btc_l = bitcoin_list;
    while(btc_l != NULL){
        if ((btc_l->bitcoin)->btc_id == btc_id)
            return btc_l->bitcoin;
        btc_l = btc_l->next;
    }
    return NULL;
}

void go_bfs_Bitcoin_Tree(Bitcoin_Tree *btc_tree, int *num_of_transactions){
    if (btc_tree->left_child != NULL){
        (*num_of_transactions)++;
        go_bfs_Bitcoin_Tree(btc_tree->left_child, num_of_transactions);
    }
    if (btc_tree->right_child != NULL)
        go_bfs_Bitcoin_Tree(btc_tree->right_child, num_of_transactions);
    return;
}

void find_bitcoin_unspent_value(Bitcoin_Tree *btc_tree, int *unspent_value){
    if (btc_tree->right_child != NULL)
        find_bitcoin_unspent_value(btc_tree->right_child, unspent_value);
    if (btc_tree->left_child == NULL)
        *unspent_value = btc_tree->value;
}

int bitCoinStatus(Bitcoins_List *bitcoin_list, int btc_id){
    Bitcoin *btc = get_Bitcoin(bitcoin_list, btc_id);
    if (btc == NULL)
        printf("ERROR: \n \t THERE IS NOT BITCOIN WITH ID: %d\n", btc_id);
    else{
        printf("BITCOIN: %d INITIAL VALUE: %d ", btc->btc_id, (btc->btc_tree)->value);
        Bitcoin_Tree *btc_tree = btc->btc_tree;
        int num_of_transactions = 0;
        go_bfs_Bitcoin_Tree(btc_tree, &num_of_transactions);
        printf("NUMBER OF TRANSACTIONS: %d ", num_of_transactions);
        int unspent_value = 0;
        find_bitcoin_unspent_value(btc_tree, &unspent_value);
        printf("UNSPENT VALUE: %d", unspent_value);
        printf("\n");  
    }
}
void go_bfs_Transaction_Bitcoin_Tree(Bitcoin_Tree *btc_tree){
    if (btc_tree->father != NULL){
        int sum = 0;
        int i;
        for (i = 0; i < btc_tree->pointer_to_transaction->size_of_btc_ids; i++)
            sum += (((btc_tree->pointer_to_transaction)->array_of_pointers_to_btc)[i])->value;
        printf("TRANSACTION: %s USER: %s SENT TO USER: %s VALUE: %d ON DATE: %s\n", btc_tree->pointer_to_transaction->transaction_id, btc_tree->father->owner, btc_tree->owner, sum, btc_tree->pointer_to_transaction->date_time);
    }
    if (btc_tree->left_child != NULL)   
        go_bfs_Transaction_Bitcoin_Tree(btc_tree->left_child);
    if (btc_tree->right_child != NULL)
        go_bfs_Transaction_Bitcoin_Tree(btc_tree->right_child);
    return;
}

int tracecoin(Bitcoins_List *bitcoin_list, int btc_id){
     Bitcoin *btc = get_Bitcoin(bitcoin_list, btc_id);
    if (btc == NULL)
        printf("ERROR: \n \t THERE IS NOT BITCOIN WITH ID: %d\n", btc_id);
    else{
        Bitcoin_Tree *btc_tree = btc->btc_tree;
        go_bfs_Transaction_Bitcoin_Tree(btc_tree);
    }
}

int read_functions(int size_of_sender_hash_table, int size_of_receiver_hash_table, 
                    int bucket_size_of_bytes, int bitcoin_value, 
                    char *bitCoinBalancesFile, char *transactionsFile){

    // INIT HASH TABLES
    Hash_Table sender_hash_table, receiver_hash_table;    
    sender_hash_table = init_Hash_Table(sender_hash_table, size_of_sender_hash_table, bucket_size_of_bytes);
    receiver_hash_table = init_Hash_Table(receiver_hash_table, size_of_receiver_hash_table, bucket_size_of_bytes);
    // INIT BITCOINS LIST
    Bitcoins_List *bitcoin_list;                                    
    bitcoin_list = init_Bitcoins_List(bitcoin_list);
    // READ BITCOIN BALANCES FILE
    int bucket_size = read_from_file(bitCoinBalancesFile, sender_hash_table, size_of_sender_hash_table, 
        receiver_hash_table, size_of_receiver_hash_table, bitcoin_list, bitcoin_value);
    // INIT TRANSACTION LIST AND LAST VALID TRANSACTION
    Transactions_List *transactions_list;
    transactions_list = init_Transactions_List(transactions_list);
    Transaction *Last_Valid_Transaction = NULL;
    char *Biggest_Transaction_id = NULL;
    // READ TRANSACTIONS FILE
    transactions_list = read_transactionfile(transactions_list, &Last_Valid_Transaction, transactionsFile, 
        sender_hash_table, size_of_sender_hash_table, receiver_hash_table, size_of_receiver_hash_table, &Biggest_Transaction_id);
    

    char function[20], name2[20], name1[20];
    int key = 0;
    char *InputLine = NULL;
    size_t sz = 0;
    printf("TYPE A COMMAND:\n\t");
    while(getline(&InputLine, &sz, stdin) != -1){
        if(InputLine[0] == '\n'){
            free(InputLine);
            break;
        }
        else{
            sscanf(InputLine, "%s", function);
            // %*s to avoid
            if (!strcmp(function, "/requestTransaction")){
                printf("REQUEST TRANSACTION:\n");
                transactions_list = request_Transaction(transactions_list, &Last_Valid_Transaction, InputLine, 
                                        sender_hash_table, size_of_sender_hash_table, 
                                        receiver_hash_table, size_of_receiver_hash_table, &Biggest_Transaction_id);
            }
            else if (!strcmp(function, "/requestTransactions")){
                printf("REQUEST TRANSACTIONS:\n");
                request_Transactions(sender_hash_table, size_of_sender_hash_table, receiver_hash_table, size_of_receiver_hash_table, 
                            &transactions_list, &Last_Valid_Transaction, &Biggest_Transaction_id, InputLine);
            }
            else if (!strcmp(function, "/findEarnings")){
                printf("FIND EARNINGS:\n");
                find_Earnings_Payments(receiver_hash_table, size_of_receiver_hash_table, InputLine, 0);
            }
            else if (!strcmp(function, "/findPayments")){
                printf("FIND PAYMENTS:\n");
                find_Earnings_Payments(sender_hash_table, size_of_sender_hash_table, InputLine, 1);
            }
            else if (!strcmp(function, "/walletStatus")){
                printf("WALLET STATUS:\n");
                char wallet_id[51];
                sscanf(InputLine, "%*s %s", wallet_id);
                walletStatus(sender_hash_table, size_of_sender_hash_table, wallet_id);
            }
            else if (!strcmp(function, "/bitcoinStatus")){
                printf("BITCOIN STATUS:\n");
                char p_btc_id[20];
                sscanf(InputLine, "%*s %s", p_btc_id);
                int btc_id = atoi(p_btc_id);
                bitCoinStatus(bitcoin_list, btc_id);
            }
            else if (!strcmp(function, "/traceCoin")){
                printf("TRACE COIN: ");
                char p_btc_id[20];
                sscanf(InputLine, "%*s %s", p_btc_id);
                int btc_id = atoi(p_btc_id);
                printf("%d\n", btc_id);
                tracecoin(bitcoin_list, btc_id);
            }
            else if (!strcmp(function, "/exit")){
                printf("EXITING...\n");
                free(InputLine);
                break;
            }
            else{
                printf("ERROR: \n\t\t UNKOWN COMMAMD\n");
            }
        }
        printf("TYPE A COMMAND:\n\t");
    }

    ////
    // printf("---PRINTING BITCOIN LIST---\n");
    // print_Bitcoins_List(bitcoin_list);
    // printf("\n");
    // printf("---PRINTING TRANSATION LIST---\n");
    // print_Transactions_List(transactions_list);
    // printf("\n");
    // print_Hash_Table(sender_hash_table, size_of_sender_hash_table, bucket_size_of_bytes / sizeof(Bucket));
    // print_Hash_Table(receiver_hash_table, size_of_receiver_hash_table, bucket_size_of_bytes / sizeof(Bucket));
    ////
    //free(Biggest_Transaction_id);
    delete_all(sender_hash_table, size_of_sender_hash_table, 
        receiver_hash_table, size_of_receiver_hash_table, bitcoin_list, transactions_list);
}