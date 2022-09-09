#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CLASS LIBRARY */
void remove_crlf(char *s){
    char *end = s + strlen(s); end--;
    while((end >= s) && (*end == "\n" || *end == "\r")){
        *end = "\0"; end--;
    }
}

int get_next_nonblank_line(FILE *ifp, char *buf, int max_length){
    buf[0] = '\0';
    while(!feof(ifp) && (buf[0] == '\0')){
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }
    if(buf[0] == '\0') return 0;
    if(buf[0] != '\0') return 1;
}

/* Tree Struct Definition */
struct tree_name_node_struct{
    char treeName[32];
    struct tree_name_node_struct *parent;
    struct tree_name_node_struct *left;
    struct tree_name_node_struct *right;
    struct item_node_struct *theTree;
};

typedef struct tree_name_node_struct tree_name_node;

/* Item Struct Definition */
struct item_node_struct{
    char name[32];
    int count;
    struct item_node_struct *iparent;
    struct item_node_struct *left;
    struct item_node_struct *right;
    struct tree_name_node_struct *tparent;
};

typedef struct item_node_struct item_node;

/* Tree Struct Constructor */
tree_name_node *new_tree_name_node(char *treeName){
    tree_name_node *t = malloc(sizeof(tree_name_node));
    strcpy(t->treeName, treeName);
    t->parent = NULL;
    t->left = NULL;
    t->right = NULL;
    t->theTree = NULL;

    return t;
}

/* Constructor for Item in Tree */
item_node *new_item_node(char *name, int count){
    item_node *i = malloc(sizeof(item_node));
    strcpy(i->name, name);
    i->count = count;
    i->iparent = NULL;
    i->left = NULL;
    i->right = NULL;
    i->tparent = NULL;

    return i;
}

/* Deep Destructor for Tree Struct */
void dispose_tree_name(tree_name_node *t){
    if(t->left) dispose_tree_name(t->left);
    if(t->right) dispose_tree_name(t->right);
    free(t);
}

/* Deep destructor for Item in Tree */
void dispose_item(item_node *item){
    if(item->left) dispose_item(item->left);
    if(item->right) dispose_item(item->right);
    free(item);
}

/* Deep destructor for both Item in Tree and Tree Struct */
void dispose_all(tree_name_node *tree){
    if(tree->left) dispose_all(tree->left);
    if(tree->right) dispose_all(tree->right);
    dispose_item(tree->theTree);
    free(tree);
}

/* Inserts Tree Child in Tree Parent */
tree_name_node *tree_insert(tree_name_node *parent, tree_name_node *child){
    // inserting on the left
    if(strcmp(child->treeName, parent->treeName) == -1){
        if(parent->left) return tree_insert(parent->left, child);
        if(!parent->left){
            parent->left = child; child->parent = parent; return child;
        }
    }
    // inserting on the right
    if(strcmp(child->treeName, parent->treeName) != -1){
        if(parent->right) return tree_insert(parent->right, child);
        if(!parent->right){
            parent->right = child; child->parent = parent; return child;
        }
    }
}

/* Insert function for Item */
item_node *item_insert(tree_name_node *tparent, item_node *iparent, item_node *child){
    // inserts items on the left
    if(strcmp(child->name, iparent->name) == -1){
        if(iparent->left) return item_insert(tparent, iparent->left, child);
        if(!iparent->left){
            iparent->left = child; child->iparent = iparent;
            iparent->tparent = tparent; child->tparent = tparent;
            return child;
        }
    }

    // inserts items on the right
    if(strcmp(child->name, iparent->name) != -1){
        if(iparent->right) return item_insert(tparent, iparent->right, child);
        if(!iparent->right){
            iparent->right = child; child->iparent = iparent;
            iparent->tparent = tparent; child->tparent = tparent;
            return child;
        }
    }
}


/* Searches for a Tree (based on the passed treeName */
tree_name_node *tree_find(tree_name_node *parent, char *treeName){
    if(parent == NULL) return NULL;
    if(strcmp(treeName, parent->treeName) == 0) return parent;
    if(strcmp(treeName, parent->treeName) == -1) return tree_find(parent->left, treeName);
    if(strcmp(treeName, parent->treeName) == 1) return tree_find(parent->right, treeName);
}

/* Looks for Item in Tree */
item_node *item_find(item_node *iparent, char *itemName){
    if(iparent == NULL) return NULL;
    if(strcmp(itemName, iparent->name) == 0) return iparent;
    if(strcmp(itemName, iparent->name) == -1) return item_find(iparent->left, itemName);
    if(strcmp(itemName, iparent->name) == 1) return item_find(iparent->right, itemName);
}

/* Reads the tree information */
tree_name_node *tree_input(FILE *ifp, char *buf, int ntrees){
    int i; char treeName[32];
    tree_name_node *tree_name_tree;

    for(i = 0; i < ntrees; i++){
        get_next_nonblank_line(ifp, buf, 63);
        sscanf(buf, "%s", treeName);
        if(i == 0) tree_name_tree = new_tree_name_node(treeName);
        if(i != 0) tree_insert(tree_name_tree, new_tree_name_node(treeName));
    }

    return tree_name_tree;
}

/* Prints Tree Names */
void print_tree_names(FILE *ofp, tree_name_node *tree){
    if(!tree) return;

    print_tree_names(ofp, tree->left);
    fprintf(ofp, "%s ", tree->treeName);
    print_tree_names(ofp, tree->right);
}

/* Prints Item Names */
void print_item_names(FILE *ofp, item_node *theTree){
    if(!theTree) return;

    print_item_names(ofp, theTree->left);
    fprintf(ofp, "%s ", theTree->name);
    print_item_names(ofp, theTree->right);
}

/* Prints Tree Data */
void print_tree_data(FILE *ofp, tree_name_node *tree){
    if(!tree) return;

    print_tree_data(ofp, tree->left);
    fprintf(ofp, "\n===%s===\n", tree->treeName);
    print_item_names(ofp, tree->theTree);
    print_tree_data(ofp, tree->right);
}

/* Reads the Item input */
void item_input(tree_name_node *tree, FILE *ifp, char *buf, int nitems){
    int i; char treeName[32], itemName[32]; int itemCount;
    tree_name_node *tempTree;

    for(i = 0; i < nitems; i++){
        get_next_nonblank_line(ifp, buf, 63);
        sscanf(buf, "%s %s %d", treeName, itemName, &itemCount);

        tempTree = tree_find(tree, treeName);

        if(tempTree->theTree) item_insert(tempTree, tempTree->theTree, new_item_node(itemName, itemCount));
        else tempTree->theTree = new_item_node(itemName, itemCount);
    }
}

/* Reads User Input */
tree_name_node *read_input(FILE *ifp, char *buf, int *ncommands){
    tree_name_node *tree; int ntrees, nitems;

    get_next_nonblank_line(ifp, buf, 63);
    sscanf(buf, "%d %d %d", &ntrees, &nitems, &*ncommands);

    tree = tree_input(ifp, buf, ntrees);
    item_input(tree, ifp, buf, nitems);

    return tree;
}

/* Search for item i in tree t*/
void search_command(FILE *ofp, tree_name_node *tree, char *treeName, char *itemName){
    tree_name_node *tempTree; item_node *tempItem;
    tempTree = tree_find(tree, treeName);

    // two conditions: item exist vs does not exist
    if(!tempTree) fprintf(ofp, "%s does not exist", treeName);
    if(tempTree){
        tempItem = item_find(tempTree->theTree, itemName);
        if(!tempItem || tempItem->count == 0) fprintf(ofp, "%s not found in %s", itemName, treeName);
        else fprintf(ofp, "%d %s found in %s", tempItem->count, itemName, treeName);
    }
}

/* Looks for Item in Tree */
void item_before(item_node *iparent, char *itemName, int *itemBefore){
    if(iparent->left) item_before(iparent->left, itemName, itemBefore);
    if(iparent->right) item_before(iparent->right, itemName, itemBefore);
    if(strcmp(iparent->name, itemName) == -1) *itemBefore += 1;
}

/* Counts the number of items before item in tree tree */
void item_before_command(FILE *ofp, tree_name_node *tree, char *treeName, char *itemName){
    tree_name_node *tempTree; int itemBefore = 0;

    tempTree = tree_find(tree, treeName);

    item_before(tempTree->theTree, itemName, &itemBefore);
    fprintf(ofp, "item before %s: %d", itemName, itemBefore);
}

/* Calculates the left height */
int calculate_left(item_node *theTree){
    int left = 0;
    if(theTree->left) left = calculate_left(theTree->left) + 1;
    else left--;
    return left;
}

/* Calculates the right height */
int calculate_right(item_node *theTree){
    int right = 0, left = 0;
    if(!theTree->right) right--;
    if(theTree->left) right = calculate_right(theTree->left) + 1;
    if(theTree->right) right = calculate_right(theTree->right) + 1;
    return right;
}

/* Calculate the left and right subtrees of tree tree */
void height_balance_command(FILE *ofp, tree_name_node *tree, char *treeName){
    int left, right, difference;

    tree_name_node *tempTree;
    tempTree = tree_find(tree, treeName);

    left = calculate_left(tempTree->theTree);
    right = calculate_right(tempTree->theTree);
    difference = right - left;

    if(difference <= 1){
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced", treeName, left, right, difference);
    } else {
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced", treeName, left, right, difference);
    }
}

// counts num item in tree
int count_item(item_node *theTree){
    // counter var
    int count = theTree->count;

    // adds left and right count
    if(theTree->left) count += count_item(theTree->left);
    if(theTree->right) count += count_item(theTree->right);

    // returns total num of item in tree
    return count;
}

/* Count command function */
void count_command(FILE *ofp, tree_name_node *tree, char *treeName){
    int count; tree_name_node *tempTree;
    tempTree = tree_find(tree, treeName);

    // calculates total num of animals
    count = count_item(tempTree->theTree);
    fprintf(ofp, "%s count %d", treeName, count);
}

// finds the left most item of item->right
item_node *leftMostOfR(item_node *rightChild){
    if(rightChild->left) leftMostOfR(rightChild->left);
    else return rightChild;
}

// swap items
void swap_item(tree_name_node *tempTree, item_node *item, item_node *leftMost){
    leftMost->left = item->left;
    leftMost->right = item->right;
    leftMost->iparent->left = NULL;
    leftMost->iparent = item->iparent;
    if(tempTree->theTree == item) tempTree->theTree = leftMost;
}

// promotes item after i is disposed
void promote_item(tree_name_node *tempTree, item_node *item, item_node *child){
    child->iparent = item->iparent;
    if(strcmp(item->iparent->left->name, item->name) == 0)
        item->iparent->left = child;
    else item->iparent->right = child;

    if(tempTree->theTree == item) tempTree->theTree = child;
}

// deletes item in tree
item_node *delete_item(tree_name_node *tempTree, item_node *item){
    item_node *iparent = item->iparent;
    item_node *right = item->right;
    item_node *left = item->left;

    if(left == NULL && right == NULL){
        if(!iparent) return item;

        if(iparent->left && strcmp(iparent->left->name, item->name) == 0)
            iparent->left = NULL;
        else iparent->right = NULL;

    } else if(left != NULL && right == NULL) {
        promote_item(tempTree, item, left);

    } else if(right != NULL && left == NULL) {
        promote_item(tempTree, item, right);

    } else {
        item_node *leftMost = leftMostOfR(right);
        swap_item(tempTree, item, leftMost);
    }

    return item;
}

/* Deletes item in tree */
void delete_command(FILE *ofp, tree_name_node *tree, char *treeName, char *itemName){
    tree_name_node *tempTree = tree_find(tree, treeName);
    item_node *tempItem = item_find(tempTree->theTree, itemName);

    tempItem = delete_item(tempTree, tempItem);
    free(tempItem);

    fprintf(ofp, "%s deleted from %s", itemName, treeName);
}

// Promotes tree after disposing t
void promote_tree(tree_name_node *tempTree, tree_name_node *child){
    child->parent = tempTree->parent;
    if(strcmp(tempTree->parent->left->treeName, tempTree->treeName) == 0)
        tempTree->parent->left = child;
    else tempTree->parent->right = child;
}

// finds the left most tree of tree->right
tree_name_node *leftMostOfRTree(tree_name_node *rightChild){
    if(rightChild->left) leftMostOfRTree(rightChild->left);
    else return rightChild;
}

// function used to swap tree
void swap_tree(tree_name_node *tempTree, tree_name_node *leftMost){
    leftMost->left = tempTree->left;
    leftMost->right = tempTree->right;
    leftMost->parent->left = NULL;
    leftMost->parent = tempTree->parent;
}

/* Makes the appropriate changes before deleting tree node */
tree_name_node *delete_tree(tree_name_node *tempTree){
    tree_name_node *parent = tempTree->parent;
    tree_name_node *left = tempTree->left;
    tree_name_node *right = tempTree->right;

    if(left == NULL && right == NULL){
        if(!parent) return tempTree;
        if(parent->left && strcmp(tempTree->left->treeName, tempTree->treeName) == 0) parent->left = NULL;
        else parent->right = NULL;

    } else if(left != NULL && right == NULL) {
        promote_tree(tempTree, left);

    } else if(right != NULL && left == NULL) {
        promote_tree(tempTree, right);

    } else {
        tree_name_node *leftMost = leftMostOfRTree(right);
        swap_tree(tempTree, leftMost);
    }

    return tempTree;
}

/* Deletes single tree node */
void delete_tree_command(FILE *ofp, tree_name_node *tree, char *treeName){
    tree_name_node *tempTree = tree_find(tree, treeName);

    dispose_item(tempTree->theTree);
    tempTree = delete_tree(tempTree);
    free(tempTree);

    fprintf(ofp, "%s deleted", treeName);
}

/* Procces user commands */
void process_commands(FILE *ifp, FILE *ofp, tree_name_node *tree, char *buf, int ncommands){
    int i;
    char commands[15], treeName[32], itemName[32];

    fprintf(ofp, "\n=====Processing Commands=====\n");

    // scans ncommands times for user input
    for(i = 0; i < ncommands; i++){
        get_next_nonblank_line(ifp, buf, 63);
        sscanf(buf, "%s %s %s", commands, treeName, itemName);

        // depending on user input, this function runs the appropriate functions
        if(strcmp(commands, "search") == 0) search_command(ofp, tree, treeName, itemName);
        if(strcmp(commands, "item_before") == 0) item_before_command(ofp, tree, treeName, itemName);
        if(strcmp(commands, "height_balance") == 0) height_balance_command(ofp, tree, treeName);
        if(strcmp(commands, "count") == 0) count_command(ofp, tree, treeName);
        if(strcmp(commands, "delete") == 0) delete_command(ofp, tree, treeName, itemName);
        if(strcmp(commands, "delete_tree") == 0) delete_tree_command(ofp, tree, treeName);

        fprintf(ofp, "\n");
    }
}

// deals with the ofp file
void output_data(FILE *ifp, FILE *ofp, tree_name_node *tree, char *buf, int ncommands){
    print_tree_names(ofp, tree);
    print_tree_data(ofp, tree);
    process_commands(ifp, ofp, tree, buf, ncommands);
}

int main()
{
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("cop3502-as4-input.txt", "r");
    ofp = fopen("cop3502-as4-output-tshielekeja mbuy-gradi.txt", "w");

    int ncommands; tree_name_node *tree;

    char buf[63];

    tree = read_input(ifp, buf, &ncommands);
    output_data(ifp, ofp, tree, buf, ncommands);
    dispose_all(tree);

    fclose(ifp);
    fclose(ofp);

    return 0;
}
