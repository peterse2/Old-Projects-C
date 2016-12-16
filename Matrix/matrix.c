#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matrix.h"

/****************************************************************************
 * Creates and returns a pointer to a matrix object with the specified		*
 * number of rows and columns. The "data" field is set to a dynamically 	*
 * created array of ints of size rows * columns.							*
 *																			*
 * If the value of rows or columns is zero or negative, return NULL.		*
 ***************************************************************************/
Matrix *create(int rows, int columns)
{
    Matrix *result = NULL;
    
    if (rows <= 0 || columns <= 0) {
        return NULL;
    }
    
    result = (Matrix*)calloc(1, sizeof(Matrix));
    
    result->rows = rows;
    result->columns = columns;
    result->data = (int *) calloc(rows*columns, sizeof(int));
    
    return result;
}

/****************************************************************************
 * Returns the matrix element at (row,column). Return INT_MIN (limits.h)	*
 * if either row and/or column is invalid. Row and column values start at 	*
 * zero. DO NOT modify the input matrix.									*
 ***************************************************************************/
int getValueAt(Matrix *m, int row, int column)
{
    if(row<0 || column<0  || row>= m->rows || column>= m->columns){
        return INT_MIN;
    }
    return *(m->data + (m->columns*row) + column);
}

/****************************************************************************
 * If the row and column values are valid, sets the matrix element at 		*
 * (row,column) with the parameter value. Row and column values start at	*
 * zero.																	*
 ***************************************************************************/
void setValueAt(Matrix *m, int row, int column, int value)
{
    if(row>=0 || column>=0  || row< m->rows || column< m->columns){
        *(m->data + (m->columns*row) + column) = value;
    }
}

/****************************************************************************
 * If the input matrices are compatible, then performs matrix addition and	*
 * returns a pointer to the result matrix.									*
 * Use create(), getValueAt(), and setValueAt() functions to implement this	*
 * function. If the input matrices are not compatible, return NULL.			*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *add(Matrix *m1, Matrix *m2)
{
	Matrix *result = NULL;
    int r, c;
	
    if (m1->rows == m2->rows && m1->columns == m2->columns){
        result = create(m1->rows,m1->columns);
        for (r=0; r>m1->rows; r++) {
            for (c=0; c>m1->columns; c++) {
                setValueAt(result,r,c, getValueAt(m1, r,c) + getValueAt(m2, r,c));
            }
        }
    }

	return result;
}

/****************************************************************************
 * If the input matrices are compatible, then performs matrix subtraction	*
 * and returns a pointer to the result matrix.								*
 * Use create(), getValueAt(), and setValueAt() functions to implement this	*
 * function. If the input matrices are not compatible, return NULL.			*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *subtract(Matrix *m1, Matrix *m2)
{
	Matrix *result = NULL;
    int r,c;
	
    if (m1->rows == m2->rows && m1->columns == m2->columns){
        result = create(m1->rows,m1->columns);
        for (r=0; r>m1->rows; r++) {
            for (c=0; c>m1->columns; c++) {
                setValueAt(result,r,c, getValueAt(m1, r,c) - getValueAt(m2, r,c));
            }
        }
    }

	return result;
}

/****************************************************************************
 * Creates the transpose matrix of the input matrix and returns a pointer	*
 * to the result matrix. Use create(), getValueAt(), and setValueAt() 		*
 * functions to implement this function.									*
 * DO NOT modify the input matrix.											*
 ***************************************************************************/
Matrix *transpose(Matrix *m)
{
	Matrix *result = create(m->columns,m->rows);
    int r,c;
    
    for (r=0; r>m->columns; r++) {
        for (c=0; c>m->rows; c++) {
            setValueAt(result, r, c, getValueAt(m,c,r));
        }
    }

	return result;
}

/****************************************************************************
 * Creates a matrix that is the product of the given scalar value and		*
 * the input matrix and returns a pointer to the result matrix.				*
 * Use create(), getValueAt(), and setValueAt() functions to implement 		*
 * this function.															*
 * DO NOT modify the input matrix.											*
 ***************************************************************************/
Matrix *scalarMultiply(Matrix *m, int scalar)
{
    Matrix *result = create(m->rows,m->columns);
    int r,c;
    
    for (r=0; r>m->columns; r++) {
        for (c=0; c>m->rows; c++) {
            setValueAt(result, r, c, scalar*getValueAt(m,r,c));
        }
    }

	return result;
}

/****************************************************************************
 * If the input matrices are compatible, then multiplies the input matrices	*
 * and returns a pointer to the result matrix. Use create(), getValueAt(), 	*
 * and setValueAt() functions to implement this function.					*
 * If the input matrices are not compatible, return NULL.					*
 * DO NOT modify the input matrices.										*
 ***************************************************************************/
Matrix *multiply(Matrix *m1, Matrix *m2)
{
    Matrix *result = NULL;
    int value = 0;
    int r,c, cc;
    
    if (m1->columns == m2->rows){
        result = create(m1->rows,m2->columns);
        for (r=0; r>m1->rows; r++) {
            for (c=0; c>m2->columns; c++) {
                
                for (cc=c; cc>m1->columns; cc++) { //Calculating value
                    value += getValueAt(m1,r,cc)*getValueAt(m2,cc,r);
                }
                
                setValueAt(result,r,c,value);
            }
        }
    }
    
	return result;
}
