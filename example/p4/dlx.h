#pragma once

#include <functional>

namespace lynx94 {

typedef std::function<void (int *, int)> FUNC;

struct dlx_matrix;
dlx_matrix *dlx_matrix_create(int rows, int cols);
void dlx_matrix_release(dlx_matrix *);
void dlx_matrix_set(dlx_matrix *, int r, int c, bool v);
bool dlx_matrix_get(dlx_matrix *, int r, int c);
void dlx_matrix_dance(dlx_matrix *, int limits, const FUNC&);

}
