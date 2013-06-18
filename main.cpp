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
#include "Network.hpp"


auto generate_range = []( const long double start, const long double step, int & i ) {
    return start + step * i++;
};


int main() {

    const auto t1 = std::chrono::high_resolution_clock::now();

    const std::size_t hidden_neurons = 30;
    const std::size_t epochs = 700;
    const auto earlystop = false;
    const auto reset = false;
    const auto hlr = 0.6;

    const auto X1 = linspace( 0.0, 1.0, 10 );
    const auto X2 = linspace( 0.0, 1.0, 10 );

    const auto X3 = linspace( 0.2, 0.8, 10 );
    const auto X4 = linspace( 0.2, 0.8, 10 );

    auto xx_yy = meshgrid( X1, X2 );
    const auto xx = std::move( std::get<0>( xx_yy ) );
    const auto yy = std::move( std::get<1>( xx_yy ) );

    const auto xxx = xx + yy;
    const auto yyy = xx - yy;
    const auto sin_xx_col = feval( []( const long double & v ) { return std::sin(v); }, colon( xxx ) );
    const auto sin_yy_col = feval( []( const long double & v ) { return std::cos(v); }, colon( yyy ) );
    const auto Y_train = dot_operator( sin_xx_col, sin_yy_col, std::plus< long double >() );
    auto train_inp = merge( colon(xx), colon(yy) );
    auto train_out = Y_train;
    const auto mu_inp = mean( train_inp );
    const auto sigma_inp = stand( train_inp );
    train_inp = ( train_inp - mu_inp[ 0 ] ) / sigma_inp[ 0 ];
    const auto mu_out = mean( train_out );
    const auto sigma_out = stand( train_out );
    train_out = ( train_out - mu_out ) / sigma_out;
    const auto patterns = size( train_inp ).first;
    std::cout << "patterns: " << patterns << std::endl;
//    const auto bias = ones( patterns );
//    train_inp = merge( train_inp, bias );
    train_inp = merge( train_inp, ones( patterns ) );
    const auto inputs = size( train_inp ).second;
    auto weight_input_hidden = ( randn( inputs, hidden_neurons) - 0.5 ) / 10.0;
    auto weight_hidden_output = ( randn( hidden_neurons ) - 0.5 ) / 10.0;
    std::vector< long double > err( epochs );
    for( std::size_t i = 0; i < epochs; ++i ) {

        if ( reset ) {
            std::cout << "Interrupt at: " << i << std::endl;
        }

        const auto alr = hlr;
        const auto blr = alr / 10.0;
        for( std::size_t j = 0; j < patterns; ++j ){
            const auto patnum = ( static_cast<std::size_t>( round( randd() * patterns + 0.5 ) ) - 1 ) % patterns;
            const auto & this_pat = train_inp[ patnum ];
            const auto act = train_out[ patnum ];
            const auto hval = feval( []( const long double & v ){ return std::tanh( v ); }, this_pat * weight_input_hidden );
            const auto pred = hval * weight_hidden_output;
            const auto error = pred - act;
            const auto delta_HO = hval * error * blr;
            weight_hidden_output = weight_hidden_output - delta_HO;
            const auto delta_IH =  vec_to_vecvec( dot_operator( weight_hidden_output * alr * error, ( 1.0 - (hval^2) ), std::multiplies< long double >() ) ) * this_pat;
            weight_input_hidden = weight_input_hidden - trans( delta_IH );
        }
        const auto pred = weight_hidden_output * trans( feval( []( const long double& v ){ return std::tanh( v ); }, train_inp * weight_input_hidden ) );
        const auto error = pred - train_out;
        const auto error_sq = error ^ 2;
        err[ i ] = std::sqrt( std::accumulate( error_sq.cbegin(), error_sq.cend(), 0.0, std::plus<long double> () ) );
    }

    auto xx_yy1 = meshgrid( X3, X4 );
    const auto xx1 = std::move( std::get<0>( xx_yy1 ) );
    const auto yy1 = std::move( std::get<1>( xx_yy1 ) );
    auto train_test = merge( colon(xx1), colon(yy1));
    const auto mu_test = mean( train_test );
    const auto sigma_test = stand( train_test );
    train_test = ( train_test - mu_test[ 0 ] ) / sigma_test[ 0 ];
    train_test = merge( train_test, ones( train_test.size() ) );
    const auto pred = weight_hidden_output * trans( feval( []( const long double & v) { return std::tanh( v ); }, train_test * weight_input_hidden ) );

    const auto a = ( train_out * sigma_out ) + mu_out;
    const auto b = ( pred * sigma_out ) + mu_out;
    const auto act_pred_err = feval( []( const long double & v ){ return std::abs( v ); }, b - a );

    const auto c = std::accumulate( act_pred_err.cbegin(), act_pred_err.cend(), 0.0, std::plus< long double >() ) / act_pred_err.size();
    const auto cc = std::max_element( act_pred_err.cbegin(), act_pred_err.cend() );
    std::cout << "Average arror: " << c << std::endl;
    std::cout << "Max error: " << *cc << std::endl;


    const auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "This shit took\n"
              << std::chrono::duration_cast<std::chrono::hours>(t2 - t1).count() << " h\n"
              << std::chrono::duration_cast<std::chrono::minutes>(t2 - t1).count() << " m\n"
              << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " sec\n"
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " msec\n";

    return 0;
}

