
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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


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
}*/


///*
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
//#include "Network.hpp"

#include "ANNetwork.hpp"

//std::tuple<std::vector<long double> , std::vector<std::vector<long double> > , std::vector<long double> >
//training(const std::size_t hidden_neurons,
//         const long double hlr,
//         const std::size_t epochs,
//         const std::vector< long double > train_ou,
//         const std::vector< std::vector< long double > >& train_in,
//         volatile bool * reset)
//{
//    auto train_inp = train_in;
//    auto train_out = train_ou;
//    const auto mu_inp = mean( train_inp );
//    const auto sigma_inp = stand( train_inp );
//    train_inp = ( train_inp - mu_inp[ 0 ] ) / sigma_inp[ 0 ];
//    const auto mu_out = mean( train_out );
//    const auto sigma_out = stand( train_out );
//    train_out = ( train_out - mu_out ) / sigma_out;
//    const auto patterns = size( train_inp ).first;

//    std::cout << "patterns: " << patterns << std::endl;
//    auto bias = ones( patterns );
//    train_inp = merge( train_inp, bias );
//    const auto inputs = size( train_inp ).second;

//    std::vector< long double > err( epochs );

//    auto weight_input_hidden = ( randn( inputs, hidden_neurons) - 0.5l ) / 10.0l;
//    auto weight_hidden_output = ( randn( hidden_neurons ) - 0.5l ) / 10.0l;

//    for( std::size_t i = 0; i < epochs; ++i ) {
//        if ( *reset ) {
//            break;
//        }
//        const auto alr = hlr;
//        const auto blr = alr / 10.0;
//        for( std::size_t j = 0; j < patterns/2; ++j ){
//            const auto patnum = ( static_cast<std::size_t>( round( randd() * patterns + 0.5 ) ) - 1 ) % patterns;
//            const auto this_pat = train_inp[ patnum ];
//            const auto act = train_out[ patnum ];
//            const auto hval = feval( []( const long double & v ){ return std::tanh( v ); }, this_pat * weight_input_hidden );
//            const auto pred = hval * weight_hidden_output;
//            const auto error = pred - act;
//            const auto delta_HO = hval * error * blr;
//            weight_hidden_output = weight_hidden_output - delta_HO;
//            const auto m1 = weight_hidden_output * alr * error;
//            const auto m2 = 1.0l - (hval^2);
//            const auto m3 = dot_operator( m1, m2, std::multiplies< long double >());
//            const auto m4 = vec_to_vecvec( m3 );
//            const auto delta_IH = m4 * this_pat;
//            weight_input_hidden = weight_input_hidden - trans( delta_IH );
//        }
//        const auto p1 = feval( []( const long double& v ){ return std::tanh( v ); }, train_inp * weight_input_hidden );
//        const auto pred = weight_hidden_output * trans( p1 );
//        const auto error = pred - train_out;
//        const auto error_sq = error ^ 2;
//        err[ i ] = std::sqrt( std::accumulate( error_sq.cbegin(), error_sq.cend(), 0.0, std::plus<long double> () ) );
//        std::cerr << "err[i]: " << err[i] << std::endl;
//    }
//    return std::move(std::make_tuple(weight_hidden_output, weight_input_hidden, err));
//}

//std::vector< long double > eval(std::vector<std::vector<long double> >& train_test,
//                                const std::vector<long double>& weight_hidden_output,
//                                const std::vector<std::vector<long double>> weight_input_hidden)
//{
//    auto pred = weight_hidden_output * trans( feval( []( const long double & v) { return std::tanh( v ); }, train_test * weight_input_hidden ) );
//    return std::move(pred);
//}

//void test_error(const std::vector< long double > & test_out,
//                const std::vector< long double > & pred,
//                const std::vector< long double > & train_out)
//{
//    const auto a = test_out;//(train_out * sigma_out) + mu_out;
//    const auto mu_out = mean( train_out );
//    const auto sigma_out = stand( train_out );
//    const auto b = (pred * sigma_out) + mu_out;
//   // std::cerr << "b: " << b << std::endl;
//    const auto act_pred_err = feval( []( const long double & v ){ return std::abs( v ); }, b - a );

//    const auto c = std::accumulate( act_pred_err.cbegin(), act_pred_err.cend(), 0.0, std::plus< long double >() ) / act_pred_err.size();
//    const auto cc = std::max_element( act_pred_err.cbegin(), act_pred_err.cend() );
//    std::cout << "Average arror: " << c << std::endl;
//    std::cout << "Max error: " << *cc << std::endl;
//}
/*
int main() {

    const auto t1 = std::chrono::high_resolution_clock::now();
    const std::size_t hidden_neurons = 50;
    const std::size_t epochs = 500;
    volatile auto reset = false;
    const long double hlr = 0.1;

    const auto X1 = linspace( 0.0l, 1.0l, 30 );
    const auto X2 = linspace( 0.0l, 1.0l, 30 );

    const auto X3 = linspace( 0.0l, 1.0l, 30 );
    const auto X4 = linspace( 0.0l, 1.0l, 30 );

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

    const auto tr_res = training(hidden_neurons, hlr, epochs, train_out, train_inp, &reset);

    const auto weight_hidden_output = std::get<0>(tr_res);
    const auto weight_input_hidden = std::get<1>(tr_res);
    auto train_test = merge( colon(xx1), colon(yy1));
    const auto bias = ones(size(train_test).first);
    const auto mu_test = mean( train_test );
    const auto sigma_test = stand( train_test );
    train_test = ( train_test - mu_test[ 0 ] ) / sigma_test[ 0 ];
    train_test = merge( train_test , bias);

    const auto pred = eval(train_test, weight_hidden_output, weight_input_hidden);
    //std::cerr << "pred: " << pred << std::endl;
    //std::cerr << "test_out: " << test_out << std::endl;
    test_error(test_out, pred, train_out);

    const auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "It took\n"
              << std::chrono::duration_cast<std::chrono::hours>(t2 - t1).count() << " h\n"
              << std::chrono::duration_cast<std::chrono::minutes>(t2 - t1).count() << " m\n"
              << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " sec\n"
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " msec\n";
    return 0;
}
*/
