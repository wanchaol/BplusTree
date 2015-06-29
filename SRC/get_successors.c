#include "def.h"

extern int btReadCount;
extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern struct PageHdr *FetchPage(PAGENO Page);
extern void printKey(struct KeyRecord *p);
extern int FreePage(struct PageHdr *PagePtr);

int get_successors(char *key, int k, char *result[]) {
   
   	struct KeyRecord *KeyListTraverser;
   	int i;

    PAGENO pg = treesearch_page(ROOT, key);

    // fetch the first page
    struct PageHdr *PagePtr = FetchPage(pg);
    btReadCount ++;
    // get the key list
	KeyListTraverser = PagePtr->KeyListPtr;

	//find the key in keylist, store the pointer

	while(KeyListTraverser != NULL) {

		if (strcmp(KeyListTraverser->StoredKey, key) == 0)
		{	
            KeyListTraverser = KeyListTraverser->Next;
			break;	
		}
        else if (strcmp(KeyListTraverser->StoredKey, key) > 0)
        {
            break;
        }

		KeyListTraverser = KeyListTraverser->Next;
	}

	// then iterate each page and keyList to print the keys

    int count = 0;

    

    while(count < k) {
    	while(KeyListTraverser != NULL && count < k) {
    		
    		strcpy(result[count], KeyListTraverser->StoredKey);
    		//result[count] = KeyListTraverser->StoredKey;
    		//printKey(KeyListTraverser);
    		count ++;

    		KeyListTraverser = KeyListTraverser->Next;
    	}
    	if (count < k && PagePtr->PgNumOfNxtLfPg != NULLPAGENO)
    	{
    		FreePage(PagePtr);
    		btReadCount++;
    		PagePtr = FetchPage(PagePtr->PgNumOfNxtLfPg);
            //printf("the number of keys in this node: %d\n", PagePtr->NumKeys);
    		KeyListTraverser = PagePtr->KeyListPtr;
    		
    	}

    }

    printf("found %d successors:\n", count);

    for (i = 0; i < count; ++i)
    {	
    	printf("%s\n", result[i]);
    }


    FreePage(PagePtr);

   	

    return 0;
}
