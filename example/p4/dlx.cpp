// [refer.](https://gist.github.com/diegode/856202)
#include "dlx.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <functional>
#include <climits>
#include <algorithm>

namespace lynx94 {

namespace {
struct data_object { //A module in the sparse matrix data structure.
    data_object()
        : L(nullptr) , R(nullptr)
        , U(nullptr) , D(nullptr)
        , C(nullptr) , x(-1) {}

    data_object* L;                //Link to next object left.
    data_object* R;                //         "          right.
    data_object* U;                //         "          up.
    data_object* D;                //         "          down.
    data_object* C;                //Link to column header.
    int x;                         //In a column header: number of ones
    //in the column. Otherwise: row index.
};

// helper for data_object
#define check_column(obj)   ((obj)->C == nullptr)
#define check_down(obj)     ((obj)->D == nullptr)
#define is_root(obj)    (check_column(obj) && check_down(obj))
#define is_column(obj)  (check_column(obj) && !check_down(obj))


// buf:  the matrix in row major order.
data_object *sparse_matrix_create(bool *buf, int rows, int cols) {
    //step 1: construct the linked-list structure.
    //We can do this by iterating through the rows and columns. Time is
    //linear in the number of entries (optimal).
    //Space used is linear in the number of columns + the number of rows
    // + the number of ones.
    data_object* root=new data_object; //root
    data_object* P=root;               //left-right walker
    data_object* Q;                    //top-down walker
    //array of pointers to column headers
    data_object** walkers=new data_object*[cols];
    //auxiliary stack for recursion
    int* st=new int[rows];
    for (int i=0; i<cols; i++) {
        //create a column header and L/R links
        (P->R=new data_object)->L=P;
        //store a pointer to the column header
        walkers[i]=Q=P=P->R;
        P->x=0; //reset popcount
        for (int j=0; j<rows; j++) {
            if (buf[i+cols*j]) {//a 1 in the current location?
                //create a data object and U/D links
                (Q->D=new data_object)->U=Q;
                Q=Q->D;  //advance pointer
                Q->C=P;  //link to the column header
                P->x++;  //increment popcount for this column
                Q->x=j;  //note the row number of this entry
            }
        }
        Q->D=P; //complete the column
        P->U=Q;
    }
    P->R=root; //complete the column list
    root->L=P;

    //now construct the L/R links for the data objects.
    P=new data_object;
    for (int i=0; i<rows; i++) {
        Q=P;
        for (int j=0; j<cols; j++) {
            if (buf[j+cols*i]) { //a one
                //in _this_ row...
                walkers[j]=walkers[j]->D;
                //create L/R links
                (Q->R=walkers[j])->L=Q;
                //advance pointer
                Q=Q->R;
            }
        }
        if (Q==P) continue;
        Q->R=P->R;       //link it to the first one in this row.
        P->R->L=Q;       //link the first one to the last one.
    }
    delete P;                //P is no longer needed
    delete []walkers;          //walkers are no longer needed

    assert(is_root(root));
    return root;
}

void sparse_matrix_release(data_object *root) {
    assert(is_root(root));
    for (data_object *P=root->R; P!=root; ) {
        for (data_object *Q=P->D; Q!=P; ) {
            Q=Q->D;
            delete Q->U;
        }
        P=P->R;
        delete P->L;
    }
    delete root;
}

void cover(data_object *column) {
    assert(is_column(column));
    data_object* i=column->D;
    data_object* j;
    column->R->L=column->L; column->L->R=column->R;
    while (i!=column) {
        j=i->R;
        while (j!=i) {
            j->D->U=j->U; j->U->D=j->D;
            j->C->x--;
            j=j->R;
        }
        i=i->D;
    }
}

void uncover(data_object *column) {
    assert(is_column(column));
    data_object* i=column->U;
    data_object* j;
    while (i!=column) {
        j=i->L;
        while (j!=i) {
            j->C->x++;
            j->D->U=j; j->U->D=j;
            j=j->L;
        }
        i=i->U;
    }
    column->R->L=column; column->L->R=column;
}

data_object *knuth_s_heuristic(data_object *root) {
    assert(is_root(root));
    data_object *P, *res = nullptr;
    int best=INT_MAX/2;
    for (P=root->R; P!=root; P=P->R) {
        if (P->x<best) {
            best=P->x;
            res=P;
        }
    }
    return res;
}

void dlx_search(data_object *root, int depth, int *result, const FUNC& f) {
    assert(is_root(root));
    if (root->R == root) {
        f(result, depth);
        return;
    }

    data_object *column = knuth_s_heuristic(root);
    cover(column);
    for (data_object *r = column->D; r != column; r = r->D) {
        result[depth] = r->x;
        for (data_object *j = r->R; j != r; j = j->R) {
            cover(j->C);
        }
        dlx_search(root, depth+1, result, f);
        for (data_object *j = r->L; j != r; j = j->L) {
            uncover(j->C);
        }
    }
    uncover(column);
}

} // anonymous namespace

struct dlx_matrix {
    dlx_matrix(int r, int c)
        :data(nullptr), rows(r), cols(c) {
        data = new bool[r*c];
        std::fill(data, data+r*c, false);
    }

    ~dlx_matrix() {
        if (data != nullptr) {
            delete []data;
        }
    }

    int rows;
    int cols;
    bool *data;
};

dlx_matrix *dlx_matrix_create(int rows, int cols) {
    dlx_matrix *mat = new dlx_matrix(rows, cols);
    return mat;
}

void dlx_matrix_release(dlx_matrix *mat) {
    if (mat != nullptr) {
        delete mat;
    }
}
void dlx_matrix_set(dlx_matrix *mat, int r, int c, bool v) {
    assert(r >= 0 && r < mat->rows);
    assert(c >= 0 && c < mat->cols);

    v = v == 0 ? 0 : 1;
    mat->data[r*mat->cols + c] = v;
}

bool dlx_matrix_get(dlx_matrix *mat, int r, int c) {
    assert(r >= 0 && r < mat->rows);
    assert(c >= 0 && c < mat->cols);
    return mat->data[r*mat->cols + c];
}

void dlx_matrix_dance(dlx_matrix *mat, const FUNC& f) {
    assert(mat != nullptr);
    int rows = mat->rows;
    int cols = mat->cols;
    data_object *root = sparse_matrix_create(mat->data, rows, cols);
    int *result = new int[rows];
    dlx_search(root, 0, result, f);
    delete []result;
    sparse_matrix_release(root);
}


} //namespace lynx94
