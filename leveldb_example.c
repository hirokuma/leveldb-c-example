// $ gcc -ggdb -W -Wall -I./include -o tst leveldb_example.c -L . -lleveldb -pthread -lstdc++

#include <leveldb/c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    leveldb_t *db;
    leveldb_options_t *options;
    leveldb_readoptions_t *roptions;
    leveldb_writeoptions_t *woptions;
    char *err = NULL;
    char *read;
    size_t read_len;

    /******************************************/
    /* OPEN */

    options = leveldb_options_create();
    leveldb_options_set_create_if_missing(options, 1);
    db = leveldb_open(options, "testdb", &err);

    if (err != NULL) {
      fprintf(stderr, "Open fail.\n");
      return(1);
    }

    /* reset error var */
    leveldb_free(err); err = NULL;

    /******************************************/
    /* WRITE */

    woptions = leveldb_writeoptions_create();
    leveldb_put(db, woptions, "key", 3, "value", 5, &err);

    if (err != NULL) {
      fprintf(stderr, "Write fail.\n");
      return(1);
    }

    leveldb_free(err); err = NULL;

    /******************************************/
    /* READ */

    roptions = leveldb_readoptions_create();
    read = leveldb_get(db, roptions, "key", 3, &read_len, &err);

    if (err != NULL) {
      fprintf(stderr, "Read fail.\n");
      return(1);
    }

    char *readbuf = (char *)malloc(read_len + 1);
    memcpy(readbuf, read, read_len);
    readbuf[read_len] = '\0';
    free(read); read = NULL;
    printf("key: %s\n", readbuf);
    free(readbuf); readbuf = NULL;

    leveldb_free(err); err = NULL;

    /******************************************/
    /* DELETE */

    leveldb_delete(db, woptions, "key", 3, &err);

    if (err != NULL) {
      fprintf(stderr, "Delete fail.\n");
      return(1);
    }

    leveldb_free(err); err = NULL;

    /******************************************/
    /* CLOSE */

    leveldb_close(db);

    /******************************************/
    /* DESTROY */

    leveldb_destroy_db(options, "testdb", &err);

    if (err != NULL) {
      fprintf(stderr, "Destroy fail.\n");
      return(1);
    }

    leveldb_free(err); err = NULL;

    leveldb_options_destroy(options); options = NULL;
    leveldb_writeoptions_destroy(woptions); woptions = NULL;
    leveldb_readoptions_destroy(roptions); roptions = NULL;


    return(0);
}
