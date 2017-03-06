/**
 * file :	bs_hashmap.h
 * author :	bushaofeng
 * create :	2016-07-05 20:13
 * func : 
 * history:
 */

#ifndef	__BS_HASHMAP_H_
#define	__BS_HASHMAP_H_

#include "bs_type.h"
#include "bs_error.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
#define HMAP_E_KEYUSED  (BS_HASEXIST)       /* Key already existed */
#define HMAP_E_OUTMEM   (BS_NOBUFFER)       /* Out of Memory */
#define HMAP_E_NOTFOUND (BS_NOTFOUND)       /* No such element */
#define HMAP_E_OVERFLOW (BS_FULL)           /* Hashmap is full */
#define HMAP_E_FAIL     (BS_INVALID)        /* Hashmap api fail */
#define HMAP_S_OK       (BS_SUCCESS)        /* Success */
    
/**
* void_ptr is a pointer. This allows you to put arbitrary structures in the hashmap.
*/
typedef void* void_ptr;

/**
* hmap_t is a pointer to an internally maintained data structure.
* Clients of this package do not need to know how hashmaps are
* represented. They see and manipulate only hmap_t's.
*/
typedef void_ptr hmap_t;

/**
* hmap_callback_func is a pointer to a function that can take two void_ptr arguments
* and return an integer. Returns status code..
*/
typedef int (*hmap_callback_func)(void_ptr, void_ptr);

/**
* prototype for map element type
*/
typedef struct _hmap_pair_t {
    char *key;
    void_ptr data;
} hmap_pair_t;

/**
* Return an empty hashmap. Returns NULL if empty.
*/
extern hmap_t hashmap_create();

/**
* Iteratively call fn with argument (value, arg) for each element data
* in the hashmap. The function returns anything other than HMAP_S_OK
* the traversal is terminated. fn must not modify any hashmap functions.
*/
extern int hashmap_iterate(hmap_t in, hmap_callback_func fnIterValue, void_ptr arg);

/**
* Add an element to the hashmap.
* Return HMAP_S_OK, HMAP_E_KEYUSED or HMAP_E_OUTMEM.
*/
extern int hashmap_put(hmap_t in, char* key, void_ptr elem);

/**
* Get an element from the hashmap. Return HMAP_S_OK or HMAP_E_NOTFOUND.
*/
extern int hashmap_get(hmap_t in, const char* key, void_ptr *elem);

/**
* Remove an element from the hashmap. Return HMAP_S_OK or HMAP_E_NOTFOUND.
*/
extern int hashmap_remove(hmap_t in, char* key, void_ptr *outValue);

/**
* Free the hashmap
*/
extern void hashmap_destroy(hmap_t in, hmap_callback_func fnFreeValue,
            void_ptr arg);  

/** 
* Get the current size of a hashmap 
*/  
extern int hashmap_size(hmap_t in);  
    
#if defined(__cplusplus)  
}  
#endif  

#endif /* _HASHMAP_H_INCLUDED */

