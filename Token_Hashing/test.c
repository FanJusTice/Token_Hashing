#include "Token_hashing.h"
#include "time.h"
int main(int argc, char* argv[])
{
  FILE* insert_file = NULL;
  FILE* query_file = NULL;
  FILE* update_file = NULL;
  FILE* delete_file = NULL;
  int level_size = atoi(argv[1]);
  if (argc != 2 || level_size <= 0)
  {
    fprintf(stderr, "Please provide level_size as argument: %s <level_size>\n", argv[0]);
    return 1;
  }
  uint64_t i, j;
  clock_t start_time, end_time;
  level_hash *level = level_init(level_size);
  uint64_t insert_num = 0, query_num = 0, total_insert_num = 0, total_query_num = 0;
  uint64_t update_num = 0, delete_num = 0, total_update_num = 0, total_delete_num = 0;
  uint8_t key[KEY_LEN];
  uint8_t value[VALUE_LEN];

  do
  {
    printf("Insert, Query, Update or Delete ?\n");
    printf("1. Insert\n");
    printf("2. Query\n");
    printf("3. Update\n");
    printf("4. Delete\n");
    printf("q. Quit\n");

    int choice = getchar();

    switch (choice)
    {
      case '1': // insert key-value pair into hash table.
        printf("Enter the fill-name of the file to insert: \n");
        char insert_filename[100];
        scanf("%s", insert_filename);
        insert_file = fopen(insert_filename, "r");
        if (insert_file == NULL)
        {
          fprintf(stderr, "Error: unable to open insert file: %s\n", insert_filename);
        }
        start_time = clock();
        while (fscanf(insert_file, "%s %s", key, value) == 2)
        {
          if (!level_insert(level, key, value))
          {
            insert_num ++;
            total_insert_num ++;
            // printf("Key-value pair %s %s is inserted successfully!\n", key, value);
          }else
          {
            printf("Expanding: space utilization & total entries: %f  %ld\n", \
                (float)(level->level_item_num[0]+level->level_item_num[1])/(level->total_capacity*ASSOC_NUM), \
                level->total_capacity*ASSOC_NUM);
            level_expand(level);
            level_insert(level, key, value);
            insert_num ++;
            total_insert_num ++;
          }
        }
        end_time = clock();
        printf("Inserted %ld items in *** %.2f *** seconds.\n", insert_num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Total inserted items: %ld\n", total_insert_num);
        insert_num = 0;
        fclose(insert_file);
        insert_file = NULL;
        break;

      case '2': // query key-value pair from hash table, begin with static query and then dynamic query.
        printf("Enter the fill-name of the file to query: \n");
        char query_filename[100];
        scanf("%s", query_filename);
        query_file = fopen(query_filename, "r");
        if (query_file == NULL)
        {
          fprintf(stderr, "Error: unable to open query file: %s\n", query_filename);
          return 1;
        }
        start_time = clock();
        while (fscanf(query_file, "%s %s", key, value) == 2)
        {
          uint8_t *get_value = level_static_query(level, key);
          if (get_value == NULL)
          {
            printf("Key-value pair %s %s is not found in static query.\n", key, value);
          }
          else
          {
            query_num ++;
            total_query_num ++;
            // printf("Key-value pair %s %s is found in static query.\n", key, value);
          }
        }
        end_time = clock();
        printf("Static queried %ld items in *** %.2f *** seconds.\n", query_num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
        query_num = 0;
        rewind(query_file);
        start_time = clock();
        while (fscanf(query_file, "%s %s", key, value) == 2)
        {
          uint8_t *get_value = level_dynamic_query(level, key);
          if (get_value == NULL)
          {
            printf("Key-value pair %s %s is not found in dynamic query.\n", key, value);
          }
          else
          {
            query_num ++;
            total_query_num ++;
            // printf("Key-value pair %s %s is found in dynamic query.\n", key, value);
          }
        }
        end_time = clock();
        printf("Dynamic queried %ld items in *** %.2f *** seconds.\n", query_num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Total queried items: %ld\n", total_query_num);
        query_num = 0;
        fclose(query_file);
        query_file = NULL;
        break;

      case '3': // update key-value pair in hash table.
        printf("Enter the fill-name of the file to update: \n");
        char update_filename[100];
        scanf("%s", update_filename);
        update_file = fopen(update_filename, "r");
        if (update_file == NULL)
        {
          fprintf(stderr, "Error: unable to open update file: %s\n", update_filename);
          return 1;
        }
        start_time = clock();
        while (fscanf(update_file, "%s %s", key, value) == 2)
        {
          if (!level_update(level, key, value))
          {
            update_num ++;
            total_update_num ++;
            // printf("Key-value pair %s %s is updated successfully!\n", key, value);
          }else
          {
            printf("Key-value pair %s %s is not found in hash table for update.\n", key, value);
          }
        }
        end_time = clock();
        printf("Updated %ld items in *** %.2f *** seconds.\n", update_num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Total updated items: %ld\n", total_update_num);
        update_num = 0;
        fclose(update_file);
        update_file = NULL;
        break;

      case '4': // delete key-value pair from hash table.
        printf("Enter the fill-name of the file to delete: \n");
        char delete_filename[100];
        scanf("%s", delete_filename);
        delete_file = fopen(delete_filename, "r");
        if (delete_file == NULL)
        {
          fprintf(stderr, "Error: unable to open delete file: %s\n", delete_filename);
          return 1;
        }
        start_time = clock();
        while (fscanf(delete_file, "%s %s", key, value) == 2)
        {
          if (!level_delete(level, key))
          {
            delete_num ++;
            total_delete_num ++;
            // printf("Key-value pair %s %s is deleted successfully!\n", key, value);
          }else
          {
            printf("Key-value pair %s %s is not found in hash table for delete.\n", key, value);
          }
        }
        end_time = clock();
        printf("Deleted %ld items in *** %.2f *** seconds.\n", delete_num, (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Total deleted items: %ld\n", total_delete_num);
        delete_num = 0;
        fclose(delete_file);
        delete_file = NULL;
        break;


      case 'q': // Destroy hash table and quit program.
        printf("Quitting program...\n");
        level_destroy(level);
        return 0;


      default:
        printf("*** Invalid choice, please try again and input the correct parameter. ***\n");
        break;
    }
  } while (getchar()!= 'q');

  return 0;
}
