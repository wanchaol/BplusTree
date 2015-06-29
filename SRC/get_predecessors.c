#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"

extern int btReadCount;
extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern struct PageHdr *FetchPage(PAGENO Page);
extern PAGENO FindPageNumOfChild(struct PageHdr *PagePtr,
                                 struct KeyRecord *KeyListTraverser, char *Key,
                                 NUMKEYS NumKeys);
extern void printKey(struct KeyRecord *p);
extern int FreePage(struct PageHdr *PagePtr);


// #define SIZEOF(a) sizeof(a) / sizeof((a)[0]) 

int count = 0;

int predecessor_search(PAGENO PageNo, char *key, char *result[], int k) {

	struct PageHdr *PagePtr = FetchPage(PageNo);
	btReadCount++;
	struct KeyRecord *KeyListTraverser;

    if (IsLeaf(PagePtr)) { /* found leaf */
        
		// KeyListTraverser = PagePtr->KeyListPtr;

		// while(KeyListTraverser != NULL) {

		// 	if (strcmp(KeyListTraverser->StoredKey, key) == 0)
		// 	{	
		// 		break;	
		// 	}

		// 	if (count < k)	
		// 	{
		// 		strcpy(result[count], KeyListTraverser->StoredKey);
		// 		count ++;
		// 	}
		// 	else {
		// 		return count;
		// 	}

		// 	KeyListTraverser = KeyListTraverser->Next;
		// }


		while (1) {

        	if (count >=k)
        	{
        		return count;
        	}

        	KeyListTraverser = PagePtr->KeyListPtr;
        	//PAGENO prevPage = NULLPAGENO;

        	char *prevKey = NULL;
	        //struct KeyRecord *prevKeyRecord = NULL;
	        while(KeyListTraverser != NULL) {

	        	if (strcmp(KeyListTraverser->StoredKey, key) == 0)
	        	{
	        		break;
	        	}
	        	prevKey = KeyListTraverser->StoredKey;
	        	//prevKeyRecord = KeyListTraverser;
	        	KeyListTraverser = KeyListTraverser->Next;
	        }

	        if (prevKey == NULL)
	        {
	        	return count;
	        }
	        else {
	        	
	        	strcpy(result[count], prevKey);
		 		count ++;
	        }

	        key = prevKey;

        }


    } else if (IsNonLeaf(PagePtr)) {
    	// first dfs to the leaf page
        PAGENO ChildPage = FindPageNumOfChild(PagePtr, PagePtr->KeyListPtr, key,
                                              PagePtr->NumKeys);
        int page_count = predecessor_search(ChildPage, key, result, k);

        // next we check if page_count > k, if not, dfs to another page by checking the keylistptr in the non-leaf page.
        

        while (1) {

        	if (count >=k)
        	{
        		return page_count;
        	}

        	KeyListTraverser = PagePtr->KeyListPtr;
        	PAGENO prevPage = NULLPAGENO;

	        //struct KeyRecord *prevKeyRecord = NULL;
	        while(KeyListTraverser != NULL) {

	        	if (KeyListTraverser->PgNum == ChildPage)
	        	{
	        		break;
	        	}
	        	prevPage = KeyListTraverser->PgNum;
	        	//prevKeyRecord = KeyListTraverser;
	        	KeyListTraverser = KeyListTraverser->Next;
	        }

	        if (prevPage == NULLPAGENO)
	        {
	        	return page_count;
	        }
	        else {
	        	// iterate the previous record.
		    	predecessor_search(prevPage, key, result, k);
	        }

	        ChildPage = prevPage;

        }


    } else {
        assert(0 && "this should never happen");
    }


    FreePage(PagePtr);
    //return result;

    return count;
}


// int comparator(const void* e1, const void* e2) { 
//    char* s1 = *(char**)e1; 
//    char* s2 = *(char**)e2; 
//    return strcmp(s1, s2); 
// } 

/* qsort C-string comparison function */ 
// int cstring_cmp(const void *a, const void *b) 
// { 
//     const char **ia = (const char **)a;
//     const char **ib = (const char **)b;
//     return strcmp(*ia, *ib);
// 	/* strcmp functions works exactly as expected from
// 	comparison function */ 
// } 


int get_predecessors(char *key, int k, char *result[]) {

	// calculate the result char array

	predecessor_search(ROOT, key, result, k);
	
	//PAGENO pg = treesearch_page(ROOT, key);

    // fetch the first page
    // struct PageHdr *PagePtr = FetchPage(pg);

    // struct KeyRecord *KeyListTraverser = PagePtr->KeyListPtr;


    // while(KeyListTraverser != NULL){
    // 	printf("%s\n", KeyListTraverser->StoredKey);
    // 	KeyListTraverser = KeyListTraverser->Next;
    // }

	// sort and print the array
	// int result_len = sizeof(result) / sizeof(char *);
	// qsort(result, result_len, sizeof(const char *), compare); 


	//manually sort the result array

	char temp[MAXWORDSIZE];

	int i, j;
	for (i = 0; i < count-1; ++i)
	{
		for (j = i+1; j < count; ++j)
		{
			if (strcmp(result[i], result[j]) > 0)
			{
				strcpy(temp, result[i]);
				strcpy(result[i], result[j]);
				strcpy(result[j], temp);
				
			}
			
		}
	}
	

	printf("found %d predecessors:\n", count);
	for (i = 0; i < count; ++i)
	{
		printf("%s\n", result[i]);
	}

    return 0;
}
