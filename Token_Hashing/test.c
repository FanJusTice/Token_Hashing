#include "Token_hashing.h"
#include "time.h"


int main(int argc, char* argv[])
{
    if (argc != 6)
    {
       printf("Useage: %s <level_size> <insert_data_1> <query_data_1> <insert_data_2> <query_data_2>.", argv[0]);
       return 0;
    }

    int level_size = atoi(argv[1]); // INPUT: the number of addressable buckets is 2^level_size
    FILE* insert_data_1 = fopen(argv[2],"r");
    FILE* query_data_1 = fopen(argv[3],"r");
    FILE* insert_data_2 = fopen(argv[4],"r");
    FILE* query_data_2 = fopen(argv[5],"r");
    clock_t start_time, end_time;

    level_hash *level = level_init(level_size);
    uint64_t inserted = 0, queried_1 = 0, queried_2 = 0;
    uint8_t key[KEY_LEN];
    uint8_t value[VALUE_LEN];
//////////////////////////////////////////////The first insertion.////////////////////////////////////////////////
    start_time = clock();
    while(fscanf(insert_data_1, "%s %s", key, value) != EOF)
    {
        if (!level_insert(level, key, value))
        {
            inserted ++;
        }else
        {
            printf("Expanding: space utilization & total entries: %f  %ld\n", \
                (float)(level->level_item_num[0]+level->level_item_num[1])/(level->total_capacity*ASSOC_NUM), \
                level->total_capacity*ASSOC_NUM);
            level_expand(level);
            level_insert(level, key, value);
            inserted ++;
        }
    }
    end_time = clock();
    double insert_time_1 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("First insertion complete, %ld items are inserted, it costs *** %lf sec ***\n", \
    inserted, insert_time_1);
//////////////////////////////////////////////The first query.//////////////////////////////////////////////////////////
    printf("The static search test begins ...\n");
    start_time = clock();
    while(fscanf(query_data_1, "%s %s", key, value) != EOF)
    {
        uint8_t* get_value = level_static_query(level, key);
        if(get_value == NULL){
            printf("Search the key %s: ERROR! \n", key);
        }
        else{
            // queried ++;
        }
    }
    end_time = clock();
    double static_query_1 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    fclose(query_data_1);

    printf("The dynamic search test begins ...\n");
    fopen(argv[3],"r");
    start_time = clock();
    while(fscanf(query_data_1, "%s %s", key, value) != EOF)
    {
        uint8_t* get_value = level_dynamic_query(level, key);
        if(get_value == NULL){
            printf("Search the key %s: ERROR! \n", key);
        }
        else{
            queried_1 ++;
        }
    }
    end_time = clock();
    double dynamic_query_1 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("The first query complete, %ld items are queried, it costs *** %lf + %lf = %lf sec ***\n", \
    queried_1, static_query_1, dynamic_query_1, static_query_1 + dynamic_query_1);
//////////////////////////////////////////////The second insertion.////////////////////////////////////////////////
    start_time = clock();
    while(fscanf(insert_data_2, "%s %s", key, value) != EOF)
    {
        if (!level_insert(level, key, value))
        {
            inserted ++;
        }else
        {
            printf("Expanding: space utilization & total entries: %f  %ld\n", \
                (float)(level->level_item_num[0]+level->level_item_num[1])/(level->total_capacity*ASSOC_NUM), \
                level->total_capacity*ASSOC_NUM);
            level_expand(level);
            level_insert(level, key, value);
            inserted ++;
        }
    }
    end_time = clock();
    double insert_time_2 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Second insertion complete, %ld items are inserted, it costs *** %lf sec ***\n", \
    inserted, insert_time_2);

/////////////////////////////////////////////The second query.//////////////////////////////////////////////////////////
    printf("The static search test begins ...\n");
    start_time = clock();
    while(fscanf(query_data_2, "%s %s", key, value) != EOF)
    {
        uint8_t* get_value = level_static_query(level, key);
        if(get_value == NULL){
            printf("Search the key %s: ERROR! \n", key);
        }
        else{
            // queried ++;
        }
    }
    end_time = clock();
    double static_query_2 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    fclose(query_data_2);

    printf("The dynamic search test begins ...\n");
    fopen(argv[5],"r");
    start_time = clock();
    while(fscanf(query_data_2, "%s %s", key, value) != EOF)
    {
        uint8_t* get_value = level_dynamic_query(level, key);
        if(get_value == NULL){
            printf("Search the key %s: ERROR! \n", key);
        }
        else{
            queried_2 ++;
        }
    }
    end_time = clock();
    double dynamic_query_2 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("The second query complete, %ld items are queried, it costs *** %lf + %lf = %lf sec ***\n", \
    queried_2, static_query_2, dynamic_query_2, static_query_2 + dynamic_query_2);

    printf("The number of items stored in the level hash table: %ld\n", level->level_item_num[0]+level->level_item_num[1]);
    level_destroy(level);
}
