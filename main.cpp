#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <set>
#include <tuple>
#include <cassert>

#include "Utils.hpp"
#include "Network.hpp"


auto generate_range = []( const long double start, const long double step, int & i ) {
    return start + step * i++;
};


int main() {

    /* Sin X
    constexpr unsigned int number_of_points = 500;
    constexpr double a = 0.0;
    constexpr double b = 1.0;
    constexpr double step = ( b - a ) / (number_of_points - 1);

    std::vector< double > x( number_of_points );
    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
        x[ i ] = step * i;
    }

    //    std::cerr << "x: " << x << std::endl;

    std::vector< double > sinx( x.size() );
    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
        sinx[ i ] = std::sin( M_PI * x[ i ] );
    }

    //    std::cerr << "sinx: " << sinx << std::endl;
    // Create dataset
    std::vector< std::pair< double, double > > dataset_vector = merge( x, sinx );
    std::cerr << "dataset_vector: " << dataset_vector << std::endl;

    // Split dataset

    auto splitted_dataset = split( dataset_vector, std::make_tuple( 0.7, 0.3 ) );

    const auto & training_res = std::move( std::get<0>( splitted_dataset ) );
    const auto & testing_res = std::move( std::get<1>( splitted_dataset ) );

//        std::cerr << "training_res: " << training_res.size() << ":: " << training_res << std::endl;
//        std::cerr << "testing_res: " << testing_res.size() << ":: " << testing_res << std::endl;

    const std::vector< unsigned int > layers_description = { 1, 20, 1 };
    network< double, double, TanhFunction, TanhFunctionDerivative > ( layers_description, training_res, testing_res, 1e-2, 60000, 0.01, 0.0001 );

//    std::vector< double > x1 = { 0, 0, 1, 1 };
//    std::vector< double > x2 = { 0, 1, 0, 1 };
//    std::vector< double > y1 = { 0, 1, 1, 0 };

    std::vector< double > x1 = { 0, 0 };
    std::vector< double > x2 = { 1, 0 };
    std::vector< double > y1 = { 1, 0 };


    const auto argument = merge_to_vector( x1, x2 );
    std::cerr << "argument: " << argument << std::endl;

    const auto training_dataset = merge( argument, y1 );
    std::cerr << "training_dataset: " << training_dataset << std::endl;

    const std::vector< unsigned int > layers_description = { 2, 4, 1 };
//    typename std::remove_cv <decltype( argument )>::type
    network< std::vector<double>, double, LogisticFunction, LogisticFunctionDerivative > ( layers_description,
                                                                              training_dataset, training_dataset, 1e-4, 9000, 0.9, 0.1 );
    */

    /*
    //    constexpr unsigned int number_of_points = 20;
    //    constexpr double a = 0.0;
    //    constexpr double b = 1.0;
    //    constexpr double step = ( b - a ) / (number_of_points - 1);

    //    std::vector< double > x( number_of_points );
    //    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
    //        x[ i ] = step * i;
    //    }

    //    std::cerr << "x: " << x << std::endl;

    //    std::vector< double > x1( number_of_points );
    //    std::generate( x1.begin(), x1.end(), std::bind( []( const double step, int & i ) {return step * 2 * i++;}, step, 0 ) );
    //    std::cerr << "x1: " << x1 << std::endl;


    //    const auto res = std::inner_product( x.cbegin(), x.cend(), x1.cbegin(), 0 );

    */
    //    std::cerr << "res: " << res << std::endl;
    const std::size_t hidden_neurons = 30;
    const std::size_t epochs = 500;
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
    const auto sin_xx_col = feval( []( const long double & v ) { return std::sin(v); }, colon(xxx) );
    const auto sin_yy_col = feval( []( const long double & v ) { return std::cos(v); }, colon(yyy) );

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
    std::cerr << "patterns: " << patterns << std::endl;

    auto bias = ones( patterns );
    train_inp = merge( train_inp, bias );
    //    std::cerr << "train_inp: " << train_inp << std::endl;

    const auto inputs = size( train_inp ).second;
    //    std::cerr << "inputs: " << inputs << std::endl;

    auto weight_input_hidden = ( randn( inputs, hidden_neurons) - 0.5 ) / 10.0;
    //std::cerr << "weight_input_hidden.size: " << size( weight_input_hidden ) << std::endl;
    //std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
    auto weight_hidden_output = ( randn( hidden_neurons ) - 0.5 ) / 10.0;
    //std::cerr << "weight_hidden_output.size: " << size( weight_hidden_output ) << std::endl;
    //std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;

    std::vector< long double > err(epochs);

    for( std::size_t i = 0; i < epochs; ++i ) {
        const auto alr = hlr;
        // std::cerr << "alr: " << alr << std::endl;
        const auto blr = alr / 10.0;
        // std::cerr << "blr: " << blr << std::endl;
        for( std::size_t j = 0; j < patterns; ++j ){
            auto patnum = (static_cast<std::size_t>( round( randd() * patterns + 0.5 ) )-1) % patterns;
            // std::cerr << "patnum: " << patnum << std::endl;
            const auto this_pat = train_inp[ patnum ];
            // std::cerr << "this_pat: " << this_pat << std::endl;
            const auto act = train_out[ patnum ];
            // std::cerr << "act: " << act << std::endl;
            const auto xx = this_pat * weight_input_hidden;
            // std::cerr << "xx: " << xx;
            // std::cerr << "size(xx): " << size( xx ) << std::endl;
            const auto tanhxx = feval( []( const long double & v ){ return std::tanh(v); }, xx );
            // std::cerr << "tanhxx: " << tanhxx ;
            // std::cerr << "size( tanhxx ): " << size( tanhxx ) << std::endl;
            const auto hval = tanhxx;
            // std::cerr << "hval: " << hval << std::endl;
            // std::cerr << "hval: " << hval;
            const auto pred = hval * weight_hidden_output;
            // std::cerr << "pred: " << pred << std::endl;
            const auto error = pred - act;
            // std::cerr << "error: " << error << std::endl;
            const auto delta_HO = hval * error * blr;
            // std::cerr << "delta_HO: " << delta_HO << std::endl;
            weight_hidden_output = weight_hidden_output - delta_HO;
            // std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;
            const auto m1 = weight_hidden_output * alr * error;
            // std::cerr << "m1: " << m1 << std::endl;
            const auto m2 = 1.0 - (hval^2);
            // std::cerr << "m3: " << m3 << std::endl;
            const auto m3 = dot_operator( m1, m2, std::multiplies< long double >());
            // std::cerr << "m3: " << m3 << std::endl;
            const auto m4 = vec_to_vecvec( m3 );
            const auto delta_IH =  m4 * this_pat;
            // std::cerr << "delta_IH: " << delta_IH << std::endl;
            weight_input_hidden = weight_input_hidden - trans( delta_IH );
            // std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
        }
        // std::cerr << "train_inp: " << train_inp << std::endl;
        // std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
        const auto p0 = train_inp * weight_input_hidden;
        // std::cerr << "p0: " << p0 << std::endl;
        const auto p1 = feval( []( const long double& v ){ return std::tanh( v ); }, p0 );

        // std::cerr << "train_inp: " << train_inp << std::endl;
        // std::cerr << "p1: " << p1 << std::endl;
        const auto pred = weight_hidden_output * trans( p1 );
        // std::cerr << "size(pred): " << size(pred) << std::endl;
        // std::cerr << "pred: " << pred << std::endl;
        // std::cerr << "train_out: " << train_out << std::endl;
        const auto error = pred - train_out;
        // std::cerr << "error: " << error << std::endl;
        const auto error_sq = error ^ 2;
        // std::cerr << "error_sq: " << err << std::endl;
        err[ i ] = std::sqrt( std::accumulate( error_sq.cbegin(), error_sq.cend(), 0.0, std::plus<long double> () ) );
    }
    // std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
    // std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;
    // std::cerr << "err: " << err << std::endl;

    auto xx_yy1 = meshgrid( X3, X4 );
    const auto xx1 = std::move( std::get<0>( xx_yy1 ) );
    const auto yy1 = std::move( std::get<1>( xx_yy1 ) );

    auto train_test = merge( colon(xx1), colon(yy1));
    const auto mu_test = mean( train_test );
    const auto sigma_test = stand( train_test );

    train_test = ( train_test - mu_test[ 0 ] ) / sigma_test[ 0 ];
    train_test = merge( train_test , bias);

    const auto pred = weight_hidden_output * trans( feval( []( const long double & v) { return std::tanh( v ); }, train_test * weight_input_hidden ) );

    const auto a = (train_out * sigma_out) + mu_out;
    const auto b = (pred * sigma_out) + mu_out;
    const auto act_pred_err = feval( []( const long double & v ){ return std::abs( v ); }, b - a );

    // std::cerr << "act_pred_err: " << act_pred_err << std::endl;

    const auto c = std::accumulate( act_pred_err.cbegin(), act_pred_err.cend(), 0.0, std::plus< long double >() ) / act_pred_err.size();
    const auto cc = std::max_element( act_pred_err.cbegin(), act_pred_err.cend() );
    std::cerr << "Average arror: " << c << std::endl;
    std::cerr << "Max error: " << *cc << std::endl;
    return 0;
}

