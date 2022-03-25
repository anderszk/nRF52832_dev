#include "kalman_filter.h"

int8_t KALMAN(int8_t U){
    U = (double)U;
    //constants (static)
    static const double R = 40;
    static const double H = 1.00;
    static double Q = 10;
    static double P = 0;
    static double U_hat = 0;
    static double K = 0;

    //begin
    K = P*H/(H*P*H+R); // update kalman gain
    U_hat = U_hat + K*(U-H*U_hat); // update estimated

    //Update error covariance
    P = (1-K*H)*P+Q;

    U_hat = (int8_t)U_hat;
    return U_hat;


}