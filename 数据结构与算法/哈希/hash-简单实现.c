#ifndef _HASH_H_
#define _HASH_H_

typedef struct hash hash_t;
typedef unsigned int (*hashfunc_t)(unsigned int, void*);

hash_t* hash_alloc(unsigned int buckets, hashfunc_t hash_func);

void* hash_lookup_entry(hash_t *hash, void* key, unsigned int key_size);

void hash_add_entry(hash_t *hash, void *key, unsigned int key_size,
    void *value, unsigned int value_size);

void hash_free_entry(hash_t *hash, void *key, unsigned int key_size);


#endif /* _HASH_H_ */

#include "hash.h"
#include "common.h"
#include <assert.h>


typedef enum entry_status
{
    EMPTY,
    ACTIVE,
    DELETED
} entry_status_t;

typedef struct hash_node
{
    enum entry_status status;
    void *key;
    void *value;
} hash_node_t;

struct hash
{
    unsigned int buckets;
    hashfunc_t hash_func;
    hash_node_t *nodes;
};

unsigned int hash_get_bucket(hash_t *hash, void *key);
hash_node_t* hash_get_node_by_key(hash_t *hash, void *key, unsigned int key_size);

hash_t* hash_alloc(unsigned int buckets, hashfunc_t hash_func)
{
    hash_t *hash = (hash_t *)malloc(sizeof(hash_t));
    
    hash->buckets = buckets;
    hash->hash_func = hash_func;
    int size = buckets * sizeof(hash_node_t);
    hash->nodes = (hash_node_t *)malloc(size);
    memset(hash->nodes, 0, size);
    return hash;
}

void* hash_lookup_entry(hash_t *hash, void* key, unsigned int key_size)
{
    hash_node_t *node = hash_get_node_by_key(hash, key, key_size);
    if (node == NULL)
    {
        return NULL;
    }

    return node->value;
}

void hash_add_entry(hash_t *hash, void *key, unsigned int key_size, void *value, unsigned int value_size)
{
    if (hash_lookup_entry(hash, key, key_size))
    {
        fprintf(stderr, "duplicate hash key\n");
        return;
    }

    unsigned int bucket = hash_get_bucket(hash, key);
    unsigned int i = bucket;
    while (hash->nodes[i].status == ACTIVE)
    {
        i = (i + 1) % hash->buckets;
        if (i == bucket)
        {
            return; // 没找到，并且表满
        }
    }

    hash->nodes[i].status = ACTIVE;
    if (hash->nodes[i].key)
    {
        free(hash->nodes[i].key);
    }
    hash->nodes[i].key = malloc(key_size);
    memcpy(hash->nodes[i].key, key, key_size);
    if (hash->nodes[i].value)
    {
        free(hash->nodes[i].value);
    }
    hash->nodes[i].value = malloc(value_size);
    memcpy(hash->nodes[i].value, value, value_size);

}

void hash_free_entry(hash_t *hash, void *key, unsigned int key_size)
{
    hash_node_t *node = hash_get_node_by_key(hash, key, key_size);
    if (node == NULL)
        return;

    // 逻辑删除
    node->status = DELETED;
}

unsigned int hash_get_bucket(hash_t *hash, void *key)
{
    unsigned int bucket = hash->hash_func(hash->buckets, key);
    if (bucket >= hash->buckets)
    {
        fprintf(stderr, "bad bucket lookup\n");
        exit(EXIT_FAILURE);
    }

    return bucket;
}

hash_node_t* hash_get_node_by_key(hash_t *hash, void *key, unsigned int key_size)
{
    unsigned int bucket = hash_get_bucket(hash, key);
    unsigned int i = bucket;
    while (hash->nodes[i].status != EMPTY && memcmp(key, hash->nodes[i].key, key_size) != 0)
    {
        i = (i + 1) % hash->buckets;
        if (i == bucket)        // 探测了一圈
        {
            // 没找到，并且表满
            return NULL;
        }
    }
    if (hash->nodes[i].status == ACTIVE)
    {
        return &(hash->nodes[i]);
    }

    // 如果运行到这里，说明i为空位

    return NULL;
}

#include "hash.h"
#include "common.h"

typedef struct stu
{
    char sno[5];
    char name[32];
    int age;
}stu_t;

typedef struct stu2
{
    int sno;
    char name[32];
    int age;
}stu2_t;

unsigned int hash_str(unsigned int buckets, void *key)
{
    char *sno = (char *)key;
    unsigned int index = 0;

    while (*sno)
    {
        index = *sno + 4*index;
        sno++;
    }

    return index % buckets;
}

unsigned int hash_int(unsigned int buckets, void *key)
{
    int *sno = (int *)key;
    return (*sno) % buckets;
}

int main(void)
{
    stu2_t stu_arr[] =
    {
        { 1234, "AAAA", 20 },
        { 4568, "BBBB", 23 },
        { 6729, "AAAA", 19 }
    };

    hash_t *hash = hash_alloc(256, hash_int);

    int size = sizeof(stu_arr) / sizeof(stu_arr[0]);
    int i;
    for (i=0; i<size; i++)
    {
        hash_add_entry(hash, &(stu_arr[i].sno), sizeof(stu_arr[i].sno),
            &stu_arr[i], sizeof(stu_arr[i]));
    }

    int sno = 4568;
    stu2_t *s = (stu2_t *)hash_lookup_entry(hash, &sno, sizeof(sno));
    if (s)
    {
        printf("%d %s %d\n", s->sno, s->name, s->age);
    }
    else
    {
        printf("not found\n");
    }
    
    sno = 1234;
    hash_free_entry(hash, &sno, sizeof(sno));
    s = (stu2_t *)hash_lookup_entry(hash, &sno, sizeof(sno));
    if (s)
    {
        printf("%d %s %d\n", s->sno, s->name, s->age);
    }
    else
    {
        printf("not found\n");
    }

    return 0;
}