#include "List.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct NodeObj *Node;

typedef struct NodeObj {
    void* data;  // Element in list, so ['1'] for example
    Node next; // The next element
    Node prev; // The previous element
} NodeObj;

typedef struct ListObj {
    Node cursor; // Cursor element
    Node front;  // Front element
    Node back;   // Back element
    int index;   // Index of cursor element
    int length;  // Lenght of the list
} ListObj;

// ------ Constructors-Destructors

Node newNode(void* data) {
    Node N = malloc(sizeof(NodeObj));
    if (N) {
        N->data = data;
        N->next = N->prev = NULL;
    }
    return N;
}

void freeNode(Node *pN) {
    if (pN && *pN) {
        free(*pN);
        *pN = NULL;
    }
    return;
}

List newList(void) {
    List L = malloc(sizeof(ListObj));
    if (L) {
        L->cursor = NULL;
        L->front = L->back = NULL;
        L->index = -1;
        L->length = 0;
    }
    return L;
}

void freeList(List *pL) {
    if (*pL == NULL) {
        fprintf(stderr, "List Error: calling freeList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node curN = (*pL)->front;
    while (curN != NULL) {
        Node nextN = curN->next;
        freeNode(&curN);
        curN = nextN;
    }
    free(*pL);
    *pL = NULL;
    return;
}

// ------ Access functions

int length(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling length() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }
    return L->length;
}

int index_pos(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling index() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor == NULL) {
        return -1;
    }
    return L->index;
}

void* front(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling front() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling front on an empty List.\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;
}

void* back(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling back() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling back on an empty List.\n");
        exit(EXIT_FAILURE);
    }
    return L->back->data;
}

void* get(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling get() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling get on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index_pos(L) < 0) {
        fprintf(stderr,
                "List Error: calling get on List with uninitialized cursor.\n");
        fprintf(stdout, "Length of the list is: %d\n", L->length);
        exit(EXIT_FAILURE);
    }

    return L->cursor->data;
}

bool isEmpty(List L) { return (L->length == 0); }

// ------ Manipulation functions

void clear(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling clear() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    // Delete all of the nodes
    Node cur = L->front;
    while (cur != NULL) {
        Node nextN = cur->next;
        freeNode(&cur);
        cur = nextN;
    }

    // Set all of the stats back to 0 and front and back to NULL
    L->cursor = L->front = L->back = NULL;
    L->index = -1;
    L->length = 0;
    return;
}

void set(List L, void* x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling set() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling set on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index_pos(L) < 0) {
        fprintf(stderr,
                "List Error: calling set on List with uninitialized cursor.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;
    return;
}

void moveFront(List L) {
    if (L == NULL) {
        fprintf(stderr,
                "List Error: calling moveFront() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling moveFront on empty List.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor = L->front;
    L->index = 0;

    return;
}

void moveBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling moveBack on empty List.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor = L->back;
    L->index = L->length - 1;

    return;
}

void movePrev(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor != NULL && L->cursor != L->front) {
        L->cursor = L->cursor->prev;
        L->index -= 1;
    }

    else if (L->cursor != NULL && L->cursor == L->front) {
        L->cursor = NULL;
        L->index = -1;
    }
    return;
}

void moveNext(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL && L->cursor != L->back) {
        L->cursor = L->cursor->next;
        L->index += 1;
        // printf("Cursor data is now: %d\n", get(L));
    }

    else if (L->cursor != NULL && L->cursor == L->back) {
        L->cursor = NULL;
        L->index = -1;
    }
    return;
}

void prepend(List L, void* x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling prepend() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if (length(L) > 0) {
        L->front->prev = N;
        N->next = L->front;
        L->front = N;
    }
    else {
        L->front = N;
        L->back = N;
    }
    L->length += 1;
    L->index += 1;
    return;
}

void append(List L, void* x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling append() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);
    if (length(L) > 0) {
        L->back->next = N;
        N->prev = L->back;
        L->back = N;
    }
    else {
        L->front = N;
        L->back = N;
    }
    L->length += 1;
    return;
}

void insertBefore(List L, void* x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertBefore() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling insertBefore on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index_pos(L) < 0) {
        fprintf(stderr,
                "List Error: calling insertBefore on List with uninitialized cursor.\n");
        exit(EXIT_FAILURE);
    }

    // If the cursor is at the front, prepend
    if (L->cursor == L->front) {
        prepend(L, x);
    }

    // Else, perform 4 operations
    else {
        Node N = newNode(x);
        Node cur = L->cursor;

        cur->prev->next = N;
        N->prev = cur->prev;
        cur->prev = N;
        N->next = cur;
        L->length += 1;
        if (L->cursor) {
            L->index += 1;
        }
    }

    return;
}

void insertAfter(List L, void* x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertAfter() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling insertAfter on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index_pos(L) < 0) {
        fprintf(stderr,
                "List Error: calling insertAfter on List with uninitialized cursor.\n");
        exit(EXIT_FAILURE);
    }

    // If the cursor is at the front, prepend
    if (L->cursor == L->back) {
        append(L, x);
    }

    // Else, perform 4 operations
    else {
        Node N = newNode(x);
        Node cur = L->cursor;

        cur->next->prev = N;
        N->next = cur->next;
        cur->next = N;
        N->prev = cur;
        L->length += 1;
    }

    return;
}

void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteFront() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling deleteFront on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (L && L->front) {
        Node p = L->front;
        bool at_front = L->cursor == L->front;
        L->front = L->front->next;
        freeNode(&p);
        if (length(L) > 1) {
            L->front->prev = NULL;
        }
        else {
            L->back = NULL;
        }
        L->length -= 1;
        if (at_front) {
            L->cursor = NULL;
            L->index = -1;
        }
        else {
            L->index -= 1;
        }
    }
    return;
}

void deleteBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling deleteBack on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (L && L->back) {
        Node p = L->back;
        bool at_back = L->cursor == L->back;
        L->back = L->back->prev;
        freeNode(&p);
        if (length(L) > 1) {
            L->back->next = NULL;
        }
        else {
            L->front = NULL;
        }
        if (at_back) {
            L->cursor = NULL;
            L->index = -1;
        }
        L->length -= 1;
    }
    return;
}

void delete(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling delete() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) <= 0) {
        fprintf(stderr, "List Error: calling delete on empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index_pos(L) < 0) {
        fprintf(stderr,
                "List Error: calling delete on List with uninitialized cursor.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor == L->back) {
        deleteBack(L);
    }
    else if (L->cursor == L->front) {
        deleteFront(L);
    }

    else {
        Node cursor = L->cursor;
        cursor->prev->next = cursor->next;
        cursor->next->prev = cursor->prev;
        freeNode(&cursor);
        L->length -= 1;
        L->cursor = NULL;
        L->index = -1;
    }

    return;
}

// ------ Other functions

void printList(FILE *out, List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling printList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node curN = L->front;
    while (curN != NULL) {
        Node nextN = curN->next;
        if (curN == L->front) {
            fprintf(out, "%p", curN->data);
        }
        else {
            fprintf(out, " %p", curN->data);
        }
        curN = nextN;
    }

    return;
}

List concatList(List A, List B) { return A; }
