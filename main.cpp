#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <set>
#include <tuple>
#include <cassert>
#include <chrono>

#include "Utils.hpp"
#include "ANNetwork.hpp"


#include <QApplication>
#include "mainwindow.hpp"

///*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//*/

/*
int main() {

    const auto t1 = std::chrono::high_resolution_clock::now();

    const std::size_t hidden_neurons = 45;
    const std::size_t max_epoch = 3000;
    const long double learning_rate = 0.35;
    volatile auto reset = false;
    const long double desired_accuracy = 1e-3;

    const auto X1 = linspace( 0.0l, 1.0l, 7 );
    const auto X2 = linspace( 0.0l, 1.0l, 7 );

    const auto X3 = linspace( 0.0l, 1.0l, 15 );
    const auto X4 = linspace( 0.0l, 1.0l, 15 );

    auto xx_yy = meshgrid( X1, X2 );
    const auto xx = std::move( std::get<0>( xx_yy ) );
    const auto yy = std::move( std::get<1>( xx_yy ) );

    const auto xxx = (xx + yy) * (long double)M_PI;
    const auto yyy = (xx - yy) * (long double)M_PI;
    const auto sin_xx_col = feval( []( const long double & v ) { return std::sin(v); }, colon( xxx ) );
    const auto sin_yy_col = feval( []( const long double & v ) { return std::cos(v); }, colon( yyy ) );
    const auto Y_train = dot_operator( sin_xx_col, sin_yy_col, std::plus< long double >() );

    auto xx_yy1 = meshgrid( X3, X4 );
    const auto xx1 = std::move( std::get<0>( xx_yy1 ) );
    const auto yy1 = std::move( std::get<1>( xx_yy1 ) );

    const auto xxx1 = (xx1 + yy1) * (long double)M_PI;
    const auto yyy1 = (xx1 - yy1) * (long double)M_PI;
    const auto sin_xx_col1 = feval( []( const long double & v ) { return std::sin(v); }, colon( xxx1 ) );
    const auto sin_yy_col1 = feval( []( const long double & v ) { return std::cos(v); }, colon( yyy1 ) );
    const auto Y_train1 = dot_operator( sin_xx_col1, sin_yy_col1, std::plus< long double >() );

    const auto test_out = Y_train1;
    auto train_out = Y_train;
    auto train_inp = merge( colon(xx), colon(yy) );

    auto train_test = merge( colon(xx1), colon(yy1));
    const auto bias = ones(size(train_test).first);
    const auto mu_test = mean( train_test );
    const auto sigma_test = stand( train_test );
    train_test = ( train_test - mu_test[ 0 ] ) / sigma_test[ 0 ];
    train_test = merge( train_test , bias);


    // Network testing and evaluation
    {
        const auto training_result = training(train_inp, train_out, hidden_neurons, learning_rate, max_epoch, desired_accuracy, &reset);

        const auto weight_hidden_output = std::get<0>(training_result);
        const auto weight_input_hidden = std::get<1>(training_result);
        const auto training_error = std::get<2>(training_result);

        const auto testing_result = eval(train_test, weight_hidden_output, weight_input_hidden);
        const auto testing_error = test_error(test_out, testing_result, train_out);
    }


    const auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "It took\n"
              << std::chrono::duration_cast<std::chrono::hours>(t2 - t1).count() << " h\n"
              << std::chrono::duration_cast<std::chrono::minutes>(t2 - t1).count() << " m\n"
              << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " sec\n"
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " msec\n";
    return 0;
}
*/
