#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {

    // if the tree is empty, make a new node that becomes the root
    if (root == NULL) {
        bst_sf *newNode = malloc(sizeof(bst_sf));
        if (newNode == NULL) {
            
            return NULL;
        }
        newNode->mat = mat;
        newNode->left_child = NULL;
        newNode->right_child = NULL;
        return newNode;
    }

    // start wfrom the root
    bst_sf *current = root;
    bst_sf *parent = NULL;  //keeps track of the parent node 

    //loops until we actually attach the new node 
    while (current != NULL) {

        parent = current; //remember where we are before going down

        if (mat->name < current->mat->name) {

            
            if (current->left_child == NULL) {
                //left child doesn't exist, so we can insert here
                bst_sf *newNode = malloc(sizeof(bst_sf));
                if (newNode == NULL) {
                    // again, malloc 
                    return root;
                }
                newNode->mat = mat;
                newNode->left_child = NULL;
                newNode->right_child = NULL;
                current->left_child = newNode;
                break;
            }
             else {
                //he left subtree
                current = current->left_child;
            }

        } else {
     

            // go to the right side
            if (current->right_child == NULL) {
                //right child doesn't exist, insert here
                bst_sf *newNode = malloc(sizeof(bst_sf));
                if (newNode == NULL) {
                    return root;
                }
                newNode->mat = mat;
                newNode->left_child = NULL;
                newNode->right_child = NULL;
                current->right_child = newNode;
                break;
            } 
            else {
                // the right subtree
                current = current->right_child;
            }
        }
    }

    (void)parent;
    return root;
}


matrix_sf* find_bst_sf(char name, bst_sf *root) {
    bst_sf *cur = root;//starts from the root 

    while (cur != NULL) {
        if (cur->mat->name == name) {
            return cur->mat;
        } 
        else if (name < cur->mat->name) {
            cur = cur->left_child;
        } 
        else {
            cur = cur->right_child;
        }
    }
    
    return NULL; //matrix wasn't found
}

void free_bst_sf(bst_sf *root) {

    if (root == NULL) return;

    //here we use recursion to free the left side of the tree 
    free_bst_sf(root->left_child);
    //free the right subtree 
    free_bst_sf(root->right_child);

    //free the matrix 
    if (root->mat != NULL)
        free(root->mat);

    free(root);
}
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {

    //get row and column counts from the first matrix
    unsigned int r = mat1->num_rows;
    unsigned int c = mat1->num_cols;

 
    unsigned int total = r * c;

    //allocate enough memory for the struct + all its values
    matrix_sf *res = malloc(sizeof(matrix_sf) + total * sizeof(int));
    if (res == NULL) {
        return NULL;
    }

    res->name = '?';
    res->num_rows = r;
    res->num_cols = c;

    for (unsigned int i = 0; i < total; i++) {

        // values from mat1 and mat2 into separate variables
        int leftVal = mat1->values[i];
        int rightVal = mat2->values[i];

        
        int sum = leftVal + rightVal;

        //sum into the result
        res->values[i] = sum;
    }

    
    return res;
}



matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {

    //the dimensions of both matrices
    unsigned int r1 = mat1->num_rows;
    unsigned int c1 = mat1->num_cols;
    unsigned int r2 = mat2->num_rows;   
    unsigned int c2 = mat2->num_cols;

    // allocate enough space for the result 
 
    unsigned int totalSlots = r1 * c2;
    matrix_sf *res = malloc(sizeof(matrix_sf) + totalSlots * sizeof(int));
    if (res == NULL) {
       
        return NULL;
    }

   
    res->name = '?';   
    res->num_rows = r1;
    res->num_cols = c2;

 
    for (unsigned int i = 0; i < r1; i++) {

        unsigned int rowOffsetA = i * c1;
        unsigned int rowOffsetR = i * c2;

        for (unsigned int j = 0; j < c2; j++) {

            int sum = 0;

            //loop over the fist dimension
            for (unsigned int k = 0; k < c1; k++) {

                // get A[i][k]
                int leftVal = mat1->values[rowOffsetA + k];

                // get B[k][j]
                unsigned int rowOfB = k * c2;
                int rightVal = mat2->values[rowOfB + j];

                // multiply them and add to the total
                int product = leftVal * rightVal;
                sum += product;
            }

            // tore the computed value
            res->values[rowOffsetR + j] = sum;
        }
    }

    
    return res;
}


matrix_sf* transpose_mat_sf(const matrix_sf *mat) {

    unsigned int r = mat->num_rows;
    unsigned int c = mat->num_cols;
    unsigned int total = r * c;

    matrix_sf *t = malloc(sizeof(matrix_sf) + total*sizeof(int));
    t->name = '?';
    t->num_rows = c;
    t->num_cols = r;

    for (unsigned int i = 0; i < r; i++) {
        for (unsigned int j = 0; j < c; j++) {
            // get the value from (i, j)
            int v = mat->values[i*c + j];
            // store the value in the new locatio
            t->values[j*r + i] = v;
        }
    }

    return t;
}


matrix_sf* create_matrix_sf(char name, const char *expr) {

    //pointer used to walk through the expression string
    const char *p = expr;

    while (*p && isspace((unsigned char)*p)) {
        p++;
    }

    //read the number of rows from the string
    
    unsigned int rows = (unsigned int)strtoul(p, (char**)&p, 10);

    //skip spaces before reading the number of columns
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }

    //read the number of columns
    unsigned int cols = (unsigned int)strtoul(p, (char**)&p, 10);

    //now we need to find the '['
    while (*p && *p != '[') {
        p++;
    }

    //move past the '[' if we actually stopped on it
    if (*p == '[') {
        p++;
    }

    //allocate memory for the matrix struct + the array of values
    //rows * cols = total number of integers in the matrix
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows * cols * sizeof(int));
    if (m == NULL) {
        return NULL;
    }


    m->name = name;
    m->num_rows = rows;
    m->num_cols = cols;

    unsigned int idx = 0;

    //parse integer values until ']' or until we fill the matrix
    while (*p && idx < rows * cols) {

        //skip whitespace before each value
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }

        //if we hit ']' or end of string the reading stops 
        if (*p == ']' || *p == '\0') {
            break;
        }

        int val = (int)strtol(p, (char**)&p, 10);

    
        m->values[idx++] = val;

        while (*p && (isspace((unsigned char)*p) || *p == ';')) {
            p++;
        }
    }

    return m;
}


static int precedence(char op) {
    if (op == '\'') return 3;
    if (op == '*')  return 2;
    if (op == '+')  return 1;
    return 0;
}

char* infix2postfix_sf(char *infix) {

    int n = strlen(infix);

     //allocate enough room for the postfix expression
    char *post = malloc(n*2 + 20);
    int pi = 0;

    char *stack = malloc(n + 20);
    int top = -1;

    //loop through every character in the infix expression
    for (int i = 0; i < n; i++) {

        char c = infix[i];

        if (isspace((unsigned char)c)) continue;

        if (isalpha((unsigned char)c) && isupper((unsigned char)c)) {
            post[pi++] = c;
        }
        //opening parenthesis push to stack
        else if (c == '(') {
            stack[++top] = c;
        }

        //closing parenthesis  pop until matching '('

        else if (c == ')') {
            // pop everything until '(' is found
            while (top >= 0 && stack[top] != '(') {
                post[pi++] = stack[top--];
            }
            if (top >= 0) top--;
        }

        else if (c == '+' || c == '*' || c == '\'') {
            int p = precedence(c);

            while (top >= 0 && precedence(stack[top]) >= p) {
                post[pi++] = stack[top--];
            }
             //push this operator to the stack
            stack[++top] = c;
        }
    }

    //once weve scanned the whole infix string,
    //put remaining operators from the stack
    while (top >= 0) {
        post[pi++] = stack[top--];
    }

    post[pi] = '\0';

    free(stack);
    return post;
}

matrix_sf *execute_script_sf(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    bst_sf *root = NULL;
    char *line = NULL;
    size_t cap = MAX_LINE_LEN;
    matrix_sf *last = NULL;

    while (getline(&line, &cap, fp) != -1) {
        char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;

        char name = *p;
        p++;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '=') p++;
        while (*p && isspace((unsigned char)*p)) p++;

        matrix_sf *m = NULL;
        if (isdigit((unsigned char)*p)) {
            m = create_matrix_sf(name, p);
        } else {
            m = evaluate_expr_sf(name, p, root);
        }

        if (root == NULL) {
            bst_sf *n = malloc(sizeof(bst_sf));
            n->mat = m;
            n->left_child = NULL;
            n->right_child = NULL;
            root = n;
        } else {
            insert_bst_sf(m, root);
        }

        last = m;
    }

    free(line);
    fclose(fp);

    if (last == NULL) {
        if (root != NULL) free_bst_sf(root);
        return NULL;
    }

    // Make a standalone copy of the final matrix
    matrix_sf *result = copy_matrix(last->num_rows, last->num_cols, last->values);
    result->name = last->name;

    // Free the entire BST and all original matrices
    if (root != NULL) {
        free_bst_sf(root);
    }

    return result;
}


matrix_sf *execute_script_sf(char *filename) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        // if the file can't be opened, there's nothing to do
        return NULL;
    }

    bst_sf *root = NULL;   //this will store all matrices created so far
    char *line = NULL;     //buffer that getline will allocate/resize
    size_t cap = MAX_LINE_LEN;  // initial suggested size

    matrix_sf *last = NULL;    // 

    while (getline(&line, &cap, fp) != -1) {

        //pointer to the current line
        char *p = line;

        while (*p && isspace((unsigned char)*p)) {
            p++;
        }

        // empty or whitespace-only line, so skip it
        if (*p == '\0') {
            continue;
        }

        char name = *p;
        p++;

        while (*p && isspace((unsigned char)*p)) {
            p++;
        }

        //skip the '=' 
        if (*p == '=') {
            p++;
        }

        //skip spaces after '='
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }

        //now p should point to either:
        //a digit meaning rows/cols definition
        //or an operator/matrix name meaning a formula
        if (isdigit((unsigned char)*p)) {

            matrix_sf *m = create_matrix_sf(name, p);

            //if tree is empty, we need to create the root manually
            if (root == NULL) {
                bst_sf *node = malloc(sizeof(bst_sf));
                if (node == NULL) {
                    free(m);
                    break;
                }
                node->mat = m;
                node->left_child = NULL;
                node->right_child = NULL;
                root = node;
            }
            else {
                insert_bst_sf(m, root);
            }

            last = m;
        }
        else {

            //formula expression (like A+B*C)
            matrix_sf *m = evaluate_expr_sf(name, p, root);

            
            if (root == NULL) {
                bst_sf *node = malloc(sizeof(bst_sf));
                if (node == NULL) {
                    free(m);
                    break;
                }
                node->mat = m;
                node->left_child = NULL;
                node->right_child = NULL;
                root = node;
            }
            else {
                insert_bst_sf(m, root);
            }

            last = m;
        }
    }

    free(line);

   
    fclose(fp);

    // last now points to the final matrix created (as the spec requires)
    return last;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
