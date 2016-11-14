// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "MatrixMath.h"
// Calling Row and Column Variables
int i, j, k = 0, m = 0;
float l;

/**************************
 * Matrix/Matrix Multiply *
 **************************/
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]){
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if(i == 0){
                if(j == 0){
                    result[0][0] = mat1[0][0] * mat2[0][0] + mat1[0][1] * mat2[1][0]
                                                          + mat1[0][2] * mat2[2][0];
                }
                else if(j == 1){
                    result[0][1] = mat1[0][0] * mat2[0][1] + mat1[0][1] * mat2[1][1]
                                                          + mat1[0][2] * mat2[2][1];
                }
                else if(j == 2){
                    result[0][2] = mat1[0][0] * mat2[0][2] + mat1[0][1] * mat2[1][2]
                                                          + mat1[0][2] * mat2[2][2];
                }
            }
            else if(i == 1){
                if(j == 0){
                    result[1][0] = mat1[1][0] * mat2[0][0] + mat1[1][1] * mat2[1][0]
                                                          + mat1[1][2] * mat2[2][0];
                }
                else if(j == 1){
                    result[1][1] = mat1[1][0] * mat2[0][1] + mat1[1][1] * mat2[1][1]
                                                          + mat1[1][2] * mat2[2][1];
                }
                else if(j == 2){
                    result[1][2] = mat1[1][0] * mat2[0][2] + mat1[1][1] * mat2[1][2]
                                                          + mat1[1][2] * mat2[2][2];
                }
            }
            else if(i == 2){
                if(j == 0){
                    result[2][0] = mat1[2][0] * mat2[0][0] + mat1[2][1] * mat2[1][0]
                                                          + mat1[2][2] * mat2[2][0];
                }
                else if(j == 1){
                    result[2][1] = mat1[2][0] * mat2[0][1] + mat1[2][1] * mat2[1][1]
                                                          + mat1[2][2] * mat2[2][1];
                }
                else if(j == 2){
                    result[2][2] = mat1[2][0] * mat2[0][2] + mat1[2][1] * mat2[1][2]
                                                          + mat1[2][2] * mat2[2][2];
                }
            }
        }
    }
}

/**************************
 * Matrix/Matrix Addition *
 **************************/
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]){
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

/*****************
 * Matrix Equals *
 *****************/
int MatrixEquals(float mat1[3][3], float mat2[3][3]){
    k = 0;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if((mat1[i][j] + FP_DELTA) >= mat2[i][j] && (mat1[i][j] - FP_DELTA) <= mat2[i][j]){
                k += 1;
            }else{
                k += 0;
            }
        }
    }
    return k;
}

/**************************
 * Matrix/Scalar Multiply *
 **************************/
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]){
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            result[i][j] = mat[i][j] * x;
        }
    }
}

/**************************
 * Matrix/Scalar Addition *
 **************************/
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]){
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            result[i][j] = mat[i][j] + x;
        }
    }
}

/**********************
 * Matrix Determinant *
 **********************/

float MatrixDeterminant(float mat[3][3]){ // Float
    int a, b, c;
    a = mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0]
                                         + mat[0][2] * mat[1][0] * mat[2][1];
    b = mat[0][2] * mat[1][1] * mat[2][0] + mat[1][2] * mat[2][1] * mat[0][0]
                                         + mat[2][2] * mat[0][1] * mat[1][0];
    c = a - b;
    return c;
}

/****************
 * Matrix Trace *
 ****************/
float MatrixTrace(float mat[3][3]){ // Float
    m = 0;
    for(i = 0; i < 3; i++){
        if(i == 0){
            m += mat[0][0];
        }
        else if(i == 1){
            m += mat[1][1];
        }
        else if(i == 2){
            m += mat[2][2];
        }
    }
    return m;
}

/********************
 * Matrix Transpose *
 ********************/
void MatrixTranspose(float mat[3][3], float result[3][3]){
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            result[i][j] = mat[j][i];
        }
    }
}

/******************
 * Matrix Inverse *
 ******************/
void MatrixInverse(float mat[3][3], float result[3][3]){
    int a, b, c, det;
    a = mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0]
                                         + mat[0][2] * mat[1][0] * mat[2][1];
    b = mat[0][2] * mat[1][1] * mat[2][0] + mat[1][2] * mat[2][1] * mat[0][0]
                                         + mat[2][2] * mat[0][1] * mat[1][0];
    c = a - b;
    det = 1/c;
    mat[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
    mat[0][1] = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
    mat[0][2] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
    mat[1][0] = mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1];
    mat[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
    mat[1][2] = mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0];
    mat[2][0] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
    mat[2][1] = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
    mat[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            result[i][j] = mat[i][j] * det;
        }
    }
}

/****************
 * Matrix Print *
 ****************/
void MatrixPrint(float mat[3][3]){
    printf("%3.2f %3.2f %3.2f\n%3.2f %3.2f %3.2f\n%3.2f %3.2f %3.2f\n", (double)mat[0][0],
               (double)mat[0][1], (double)mat[0][2], (double)mat[1][0], (double)mat[1][1],
              (double)mat[1][2], (double)mat[2][0], (double)mat[2][1], (double)mat[2][2]);
}

/*******************
 * Matrix Adjugate *
 *******************/
void MatrixAdjugate(float mat[3][3], float result[3][3]){
    for(;;);
}
