#include "unit_tests.h"

TestSuite(student_tests, .timeout=TEST_TIMEOUT);

Test(student_tests, create_matrix_basic) {

    matrix_sf *A = create_matrix_sf('A', "2 2 [1 2; 3 4;]");
    cr_assert_not_null(A);

    cr_assert_eq(A->num_rows, 2);
    cr_assert_eq(A->num_cols, 2);

    cr_assert_eq(A->values[0], 1);
    cr_assert_eq(A->values[1], 2);
    cr_assert_eq(A->values[2], 3);
    cr_assert_eq(A->values[3], 4);

    free(A);
}

Test(student_tests, add_matrices) {
    int vals1[] = {1,2,3,4};
    matrix_sf *A = copy_matrix(2,2,vals1);
    A->name='A';

    int vals2[] = {5,6,7,8};
    matrix_sf *B = copy_matrix(2,2,vals2);
    B->name='B';

    matrix_sf *C = add_mats_sf(A,B);

    cr_assert_eq(C->values[0],6);
    cr_assert_eq(C->values[1],8);
    cr_assert_eq(C->values[2],10);
    cr_assert_eq(C->values[3],12);

    free(A);
    free(B);
    free(C);
}

Test(student_tests, mult_matrices) {
    int vA[] = {1,2,3,4};
    matrix_sf *A = copy_matrix(2,2,vA);
    A->name='A';

    int vB[] = {2,0,1,2};
    matrix_sf *B = copy_matrix(2,2,vB);
    B->name='B';

    matrix_sf *C = mult_mats_sf(A,B);

    cr_assert_eq(C->values[0],4);
    cr_assert_eq(C->values[1],4);
    cr_assert_eq(C->values[2],10);
    cr_assert_eq(C->values[3],8);

    free(A);
    free(B);
    free(C);
}

Test(student_tests, transpose_matrix) {
    int vv[] = {1,2,3,4,5,6};
    matrix_sf *A = copy_matrix(2,3,vv);
    A->name='A';

    matrix_sf *T = transpose_mat_sf(A);

    cr_assert_eq(T->num_rows,3);
    cr_assert_eq(T->num_cols,2);

    cr_assert_eq(T->values[0],1);
    cr_assert_eq(T->values[1],4);
    cr_assert_eq(T->values[2],2);
    cr_assert_eq(T->values[3],5);
    cr_assert_eq(T->values[4],3);
    cr_assert_eq(T->values[5],6);

    free(A);
    free(T);
}

Test(student_tests, bst_insert_and_find) {
    int a1[]={1};
    matrix_sf *A = copy_matrix(1,1,a1);
    A->name='M';

    int a2[]={2};
    matrix_sf *B = copy_matrix(1,1,a2);
    B->name='D';

    int a3[]={3};
    matrix_sf *C = copy_matrix(1,1,a3);
    C->name='T';

    bst_sf *root=NULL;
    root = insert_bst_sf(A,root);
    root = insert_bst_sf(B,root);
    root = insert_bst_sf(C,root);

    cr_assert(find_bst_sf('M',root)->values[0] == 1);
    cr_assert(find_bst_sf('D',root)->values[0] == 2);
    cr_assert(find_bst_sf('T',root)->values[0] == 3);

    free_bst_sf(root);
}

Test(student_tests, infix_to_postfix_basic) {
    char exp1[]="A+B*C";
    char *p = infix2postfix_sf(exp1);
    cr_assert_str_eq(p,"ABC*+");
    free(p);
}

Test(student_tests, evaluate_expression_simple_add) {
    int x[]={1,1};
    int y[]={2,2};

    matrix_sf *A = copy_matrix(1,2,x);
    A->name='A';

    matrix_sf *B = copy_matrix(1,2,y);
    B->name='B';

    bst_sf *root = NULL;
    root = insert_bst_sf(A,root);
    root = insert_bst_sf(B,root);

    matrix_sf *C = evaluate_expr_sf('C',"A + B",root);

    cr_assert_eq(C->values[0],3);
    cr_assert_eq(C->values[1],3);

    free_bst_sf(root);
}

Test(student_tests, evaluate_expression_with_transpose_and_mult) {
    int a[]={1,2};
    int b[]={3,4};

    matrix_sf *A = copy_matrix(1,2,a);
    A->name='A';

    matrix_sf *B = copy_matrix(2,1,b);
    B->name='B';

    bst_sf *root=NULL;
    root = insert_bst_sf(A,root);
    root = insert_bst_sf(B,root);

    matrix_sf *C = evaluate_expr_sf('C',"A * B",root);

    cr_assert_eq(C->values[0],11);

    free_bst_sf(root);
}

Test(student_tests, execute_script_basic) {

    FILE *fp = fopen("test_script.txt","w");
    fprintf(fp,"A = 2 2 [1 2; 3 4;]\n");
    fprintf(fp,"B = 2 2 [5 6; 7 8;]\n");
    fprintf(fp,"C = A + B\n");
    fclose(fp);

    matrix_sf *C = execute_script_sf("test_script.txt");

    cr_assert_not_null(C);
    cr_assert_eq(C->values[0],6);
    cr_assert_eq(C->values[1],8);
    cr_assert_eq(C->values[2],10);
    cr_assert_eq(C->values[3],12);

    remove("test_script.txt");
}
