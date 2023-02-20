#include "bpt.h"
#include <math.h>
#include "group.h"
#include "misc.h"
#include "queue.h"

// Deletes a key from a leaf node
void delete_key_from_leaf(node *n, int key) {
    int i = 0;
    while (n->keys[i] != key) {
        i++;
    }
    for (int j = i + 1; j < n->size; j++) {
        n->keys[j - 1] = n->keys[j];
        n->pointers[j - 1] = n->pointers[j];
    }
    n->size--;
}

// Merges two leaf nodes together
void merge_leaf_nodes(node *left, node *right) {
    for (int i = 0; i < right->size; i++) {
        left->keys[left->size] = right->keys[i];
        left->pointers[left->size] = right->pointers[i];
        left->size++;
    }
    left->pointers[NUM_KEYS] = right->pointers[NUM_KEYS];
    left->next = right->next;
    free(right);
}

// Borrows a key from a neighbor leaf node
void borrow_key_from_leaf(node *n, int idx, bool left) {
    node *neighbor = (left) ? n->prev : n->next;
    int borrowed_key = neighbor->keys[neighbor->size - 1];
    void *borrowed_pointer = neighbor->pointers[neighbor->size - 1];
    neighbor->size--;
    if (left) {
        for (int i = n->size; i > 0; i--) {
            n->keys[i] = n->keys[i - 1];
            n->pointers[i] = n->pointers[i - 1];
        }
        n->keys[0] = borrowed_key;
        n->pointers[0] = borrowed_pointer;
    } else {
        n->keys[n->size] = borrowed_key;
        n->pointers[n->size] = borrowed_pointer;
    }
    n->size++;
}

// Deletes a key from an internal node
void delete_key_from_internal(node *n, int key) {
    int i = 0;
    while (n->keys[i] != key) {
        i++;
    }
    for (int j = i + 1; j < n->size; j++) {
        n->keys[j - 1] = n->keys[j];
        n->pointers[j] = n->pointers[j + 1];
    }
    n->size--;
}

// Rebalances a B+ tree by redistributing keys between nodes or merging them
void rebalance(node *n) {
    if (n->size < MIN_KEYS) {
        if (n->parent == NULL) {
            if (n->size == 0) {
                node *new_root = n->pointers[0];
                new_root->parent = NULL;
                free(n);
            }
            return;
        }
        int idx = 0;
        node *left = NULL, *right = NULL;
        while (idx <= n->parent->size && n->parent->pointers[idx] != n) {
            idx++;
        }
        if (idx > 0) {
            left = n->parent->pointers[idx - 1];
        }
        if (idx < n->parent->size) {
            right = n->parent->pointers[idx + 1];
        }
        if (left && left->size > MIN_KEYS) {
            // Redistribute keys from left sibling to current node
            int left_idx = left->size - 1;
            insert_into_node(n, 0, left->keys[left_idx], left->pointers[left_idx + 1]);
            left->size--;
            n->parent->keys[idx - 1] = n->keys[0];
        } else if (right && right->size > MIN_KEYS) {
            // Redistribute keys from right sibling to current node
            insert_into_node(n, n->size, right->keys[0], right->pointers[0]);
            delete_from_node(right, 0);
            n->parent->keys[idx] = right->keys[0];
        } else if (left) {
            // Merge current node with left sibling
            merge_nodes(left, n, idx - 1);
            n = left;
        } else {
            // Merge current node with right sibling
            merge_nodes(n, right, idx);
        }
        rebalance(n->parent);
    }
}



// ANOTHER FORM// 

node* delete_entry(node* root, node* n, unsigned int key, void* pointer) {
    int min_keys;
    node* neighbor;
    int neighbor_index;
    int k_prime_index, k_prime;
    int capacity;
    bool underflow = false;

    remove_entry_from_node(n, key, pointer);

    if (n == root) {
        return adjust_root(root);
    }

    min_keys = n->isLeaf ? cut(LEAF_ORDER - 1) : cut(NON_LEAF_ORDER - 1);

    if (n->size >= min_keys) {
        return root;
    }

    neighbor_index = get_neighbor_index(n);
    k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
    k_prime = n->parent->keys[k_prime_index];
    neighbor = neighbor_index == -1 ? get_node(n->parent->pointers[1]) : get_node(n->parent->pointers[neighbor_index]);

    capacity = n->isLeaf ? LEAF_ORDER : NON_LEAF_ORDER - 1;

    if (neighbor->size + n->size < capacity) {
        return coalesce_nodes(root, n, neighbor, neighbor_index, k_prime);
    } else {
        return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
    }
}

#include <stdbool.h>
#include "bptree.h"

// Returns the index of the key to the right of the pointer to a node.
int get_neighbor_index(node* n) {
    int i;

    for (i = 0; i <= n->parent->size; i++) {
        if (n->parent->pointers[i] == n)
            return i - 1;
    }

    // Error state.
    exit(EXIT_FAILURE);
}

// Removes the key and pointer from a node, and adjusts its size and pointers array.
void remove_entry_from_node(node* n, unsigned int key, void* pointer) {
    int i, num_pointers;

    // Remove the key and shift other keys accordingly.
    i = 0;
    while (n->keys[i] != key)
        i++;
    for (++i; i < n->size; i++)
        n->keys[i - 1] = n->keys[i];

    // Remove the pointer and shift other pointers accordingly.
    num_pointers = n->isLeaf ? n->size : n->size + 1;
    i = 0;
    while (n->pointers[i] != pointer)
        i++;
    for (++i; i < num_pointers; i++)
        n->pointers[i - 1] = n->pointers[i];

    // Reduce the size of the node.
    n->size--;

    // Set the other pointers to NULL for tidiness.
    // A leaf uses the last pointer to point to the next leaf.
    if (n->isLeaf) {
        for (i = n->size; i < LEAF_ORDER - 1; i++)
            n->pointers[i] = NULL;
    } else {
        for (i = n->size + 1; i < NON_LEAF_ORDER; i++)
            n->pointers[i] = NULL;
    }
}

// Adjusts the root of the tree in case of underflow.
node* adjust_root(node* root) {
    node* new_root;

    // If the root node has no more pointers, make the first child the new root.
    if (root->size == 0) {
        new_root = root->isLeaf ? NULL : get_node(root->pointers[0]);
        free(root->keys);
        free(root->pointers);
        free(root);
        if (new_root != NULL)
            new_root->parent = NULL;
        return new_root;
    }

    // If the root node has at least one pointer, the root is fine.
    return root;
}

// Merges two nodes and returns the updated root of the tree.
node* coalesce_nodes(node* root, node* n, node* neighbor, int neighbor_index, int k_prime) {
    int i, j, neighbor_insertion_index, n_end;
    node* tmp;
    bool split;

    if (neighbor_index == -1) {
        tmp = n;
        n = neighbor;
        neighbor = tmp;
    }

    neighbor_insertion_index = neighbor->size;

    split = false;

    if (!n->isLeaf) {
        neighbor->keys[neighbor_insertion_index] = k_prime;
        neighbor->size++;

        n_end = n->size;

        for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
            neighbor->keys[i] = n->keys[j];
            neighbor->pointers[i] = n->pointers[j];
            neighbor->size++;
            n->size--;
        }

        neighbor->pointers[i] = n->pointers[j];

        for (i = 0; i < neighbor->size + 1; i++) {
            tmp = get_node(neighbor->pointers[i]);
            tmp->parent = neighbor;
        }
    } else {
        for (i = neighbor_insertion_index, j = 0; j < n->size; i++, j++) {
            neighbor->keys[i] = n->keys[j];
            neighbor->pointers[i] = n->pointers[j];
            neighbor->size++;
        }
        neighbor->pointers[LEAF_ORDER - 1] = n->pointers[LEAF_ORDER - 1];
    }

    root = delete_entry(root, n->parent, k_prime, n);

    free(n);

    if (root == NULL) {
        return neighbor;
    }

    return root;
}