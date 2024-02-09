#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Don't let the machines win. You are humanity's last hope...
 **/
//define a node:
// a node has position, right-neighbour, bottom-neighbour, and next node in the list
struct position {
    int x;
    int y;
};
struct node {
    int id;
    struct position pos;
    struct node* next;
    struct node* right;
    struct node* bottom;
};

char NODE = '0';
char EMPTY = '.';

void printError(char* err){
    fprintf(stderr, "\n%s\n", err);
}

void assignNode(struct node *mynode, int id_, struct position pos_, struct node *next_, struct node *right_, struct node *bottom_){
    printError("\t$Assinging Node");
    mynode->id = id_;
    mynode->next = next_;
    mynode->right = right_;
    mynode->bottom = bottom_;
    mynode->pos = pos_;
}

struct position empty_pos = {.x=-1, .y=-1};

void initArray(struct node* arr){
    printError("\t$Initalize Array");
    int array_length = (sizeof(arr) / sizeof(arr[0]));
    for(int i=0; i<array_length;i++){
        struct node *empty;
        assignNode(empty, 0, empty_pos, NULL, NULL, NULL);
        arr[i] = *empty;
    }
}

int string_length = 13;

void writeLine(struct node* node_){
    int x0 = node_->pos.x;
    int y0 = node_->pos.y;

    int x1 = node_->right == NULL ? -1 : node_->right->pos.x;
    int y1 = node_->right == NULL ? -1 : node_->right->pos.y;

    int x2 = node_->bottom == NULL ? -1 : node_->bottom->pos.x;
    int y2 = node_->bottom == NULL ? -1 : node_->bottom->pos.y;


    printf("%d %d %d %d %d %d\n", x0, y0, x1, y1, x2, y2);

}



void addToList(struct node* list, struct node* node_){
    printError("\t$addToList");
    if(list->next==NULL){
        fprintf(stderr, "!\n");
        list->next = node_;
        list = node_;
        return;
    }
    list = list->next;
    fprintf(stderr, "\nlist: %d \n", list->id);
    addToList(list, node_);

}



void printList(struct node* list){
    printError("\t$Printing List");
    if(list == NULL){
        fprintf(stderr, "\nno more nodes\n");
        return;
    }
    fprintf(stderr, "\n#PRINTLIST id: %d, node: %d %d", list->id, list->pos.x, list->pos.y);
    printList(list->next);
}

void printRowList(struct node* list){
    printError("\t$Printing Row List");
    if(list == NULL){
        fprintf(stderr, "\nno more nodes\n");
        return;
    }

    fprintf(stderr, "\n#PRINTROWLIST id: %d, node: %d %d\n", list->id, list->pos.x, list->pos.y);
    printRowList(list->right);
}

void printColList(struct node* list){
    printError("\t$Printing Col List");
    if(list == NULL){
        fprintf(stderr, "\nno more nodes\n");
        return;
    }
    fprintf(stderr, "\n#PRINTCOLLIST id: %d, node: %d %d\n", list->id, list->pos.x, list->pos.y);
    printColList(list->bottom);
}

void addToRowArrayList(struct node* arr, struct node* node_, int index){
    printError("\t$addToRowArrayList");
    struct node* current = &arr[index];

    printRowList(current);

    while(current->right!=NULL){
        fprintf(stderr, "\ncurrent->right: %d %d\n", current->right->pos.x, current->right->pos.y);
        current = current->right;
        printError("current right is nost null");
    }
    current->right = node_;
    //printRowList(&arr[index]);
}

void addToColArrayList(struct node* arr, struct node* node_, int index){
    printError("\t$addToColArrayList");
    fprintf(stderr, "\nCol Index = %d\n", index);
    struct node* current = &arr[index];
    fprintf(stderr, "\nCurrent = %d\n", current->id);
    //printColList(current);
    while(current->bottom!=NULL){
        current = current->bottom;
    }
    current->bottom = node_;
}

void collectTheInput(int height, int width, char grid[height+1][width+1] ){
    // collect the input
    printError("\t$Collecting the input");
    for (int i = 0; i < height; i++) {
        fprintf(stderr, "\n\n\t\t***loop***\n\n");
        char line[32];
        scanf("%[^\n]", line); fgetc(stdin); //scan the line
        strcpy(grid[i], line);
    }

}

int addNode(int no_nodes, struct node* prev_node, int row, int col, struct node* rows_arr, struct node* cols_arr){
    no_nodes++;
    printError("\t$Adding Node");
    fprintf(stderr, "\nNODE! in row %d and col %d\n", row, col);
    struct position cell_pos = {col, row};
    struct node *this_node = (struct node*)malloc(sizeof(struct node));
    assignNode(this_node, no_nodes, cell_pos, NULL, NULL, NULL);
    addToList(prev_node, this_node);
    addToRowArrayList(rows_arr, this_node, row);
    addToColArrayList(cols_arr, this_node, col);
    return no_nodes;

}
int scanNodes(struct node** first_node, int height, int width, int no_nodes, char grid[height+1][width+1], struct node* rows_arr, struct node* cols_arr){
    printError("\t$Scanning Nodes");
    // assign nodes to lists
    struct node* prev_node = *first_node;
    assignNode(prev_node, no_nodes, empty_pos , NULL, NULL, NULL);
    for (int row = 0; row < height; row++){
        fprintf(stderr, "\n\n\t\t***2nd loop, row is %d***\n\n", row);
        for(int col = 0; col < width; col++ ){
            fprintf(stderr, "\n\n\t\t***2nd loop inner, row is %d***\n\n", row);
            char cell = grid[row][col];
            fprintf(stderr, "\ncell is %c and row is %d\n", cell, row);
            if(cell!=NODE){
                continue;
            }
            no_nodes = addNode(no_nodes, prev_node, row, col, rows_arr, cols_arr);
        }
    }
    printError("\t#Finished Scan");
    printList(prev_node);
    return no_nodes;
}

void updateRightNode(int height, struct node* rows_arr){
    printError("\t$Updating Right Nodes");
    for(int row=0; row<=height; row++){
        struct node* current = &rows_arr[row];
        while(current->next != NULL){
            current->right = current->next;
            current = current->next;
        }
    }
}

void updateBottomNode(int width, struct node* cols_arr){
    printError("\t$Updating Bottom Nodes");
    for(int col=0; col<=width; col++){
        struct node* current = &cols_arr[col];
        while(current->next != NULL){
            current->bottom = current->next;
            current = current->next;
        }   
    }
}

void updateNodesInList(int height, int width, struct node* rows_arr, struct node* cols_arr){
    printError("\t$Updating Nodes in List");
    updateRightNode(height, rows_arr);
    updateBottomNode(width, cols_arr);
}

void printAnswer(struct node* list){
    printError("\t$Printing Answer");
    while(list != NULL){
        writeLine(list);
        list = list->next;
    }
}


int main(){
    
    // the number of cells on the X axis
    int width = 0;
    scanf("%d", &width);
    // the number of cells on the Y axis
    int height = 0;
    scanf("%d", &height); fgetc(stdin);
    
    struct node* rows_arr = (struct node*)malloc(sizeof(struct node)*(height+1)); //saving the 0 position for an empty node to start to list
    struct node* cols_arr = (struct node*)malloc(sizeof(struct node)*(width+1));
    
    int no_nodes = 0;

    initArray(rows_arr);
    initArray(cols_arr);

    char grid[height+1][width+1];

    collectTheInput(height, width, grid);
    printError("Input Collected");

    struct node* first_node = (struct node*)malloc(sizeof(struct node));

    no_nodes = scanNodes(&first_node, height, width, no_nodes, grid, rows_arr, cols_arr);
    
    updateNodesInList(height, width, rows_arr, cols_arr);

    printAnswer(first_node->next);
    
    return 0;
}