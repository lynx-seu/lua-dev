#pragma once

#include <functional>

namespace lynx94 {

typedef std::function<void (int *, int)> FUNC;

struct dlx_matrix;
dlx_matrix *dlx_matrix_create(int rows, int cols);
void dlx_matrix_release(dlx_matrix *);
void dlx_matrix_addrow(dlx_matrix *, int row, const char *);
void dlx_matrix_addrow(dlx_matrix *, int row, int *row_set, int sz);
void dlx_matrix_dance(dlx_matrix *, const FUNC&);

}
