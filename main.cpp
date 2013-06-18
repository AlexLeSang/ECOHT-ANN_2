#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

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

    //    std::cerr << "res: " << res << std::endl;
    const std::size_t hidden_neurons = 30;
    std::size_t epochs = 30; // TODO should be constant

    const auto X1 = linspace( 0.0, 0.5, 10 );
    const auto X2 = linspace( 0.0, 0.5, 10 );

    auto xx_yy = meshgrid( X1, X2 );
    const auto xx = std::move( std::get<0>( xx_yy ) );
    const auto yy = std::move( std::get<1>( xx_yy ) );

    const auto sin_xx_col = feval( []( const long double & v ) { return std::sin(v); }, colon(xx) );

    const auto sin_yy_col = feval( []( const long double & v ) { return std::sin(v); }, colon(yy) );

    const auto Y_train = dot_operator( sin_xx_col, sin_yy_col, std::multiplies< long double >() );

    auto train_inp = merge( colon(xx), colon(yy) );
    auto train_out = Y_train;

    const auto mu_inp = mean( train_inp );
    const auto sigma_inp = stand( train_inp );

    /* Stand dev test
    const auto x1 = range( 1.0, 1.0, 5.0 );
    const auto std_dev = stand( x1 );
    std::cerr << "std_dev: " << std_dev << std::endl;
    const auto x2 = range( 4.0, 2.0, 12.0 );
    const auto y = merge_to_vector( x1, x2 );
    std::cerr << "y: " << y << std::endl;
    const auto y_std_dev = stand( y );
    std::cerr << "y_std_dev: " << y_std_dev << std::endl;
    */

    // train_inp = (train_inp(:,:) - mu_inp(:,1)) / sigma_inp(:,1);
    /*
    {
        // train_inp(:,:) - mu_inp(:,1)
        auto new_train_input = train_inp;
        std::for_each( new_train_input.begin(), new_train_input.end(), [&]( decltype( new_train_input )::reference v ) {
            std::for_each( v.begin(), v.end(), [&]( std::remove_reference < decltype(v) >::type::reference t ) {
                t -= mu_inp[0];
            } );
        } );
        // (train_inp(:,:) - mu_inp(:,1)) / sigma_inp(:,1);
        std::for_each( new_train_input.begin(), new_train_input.end(), [&]( decltype( new_train_input )::reference v ) {
            std::for_each( v.begin(), v.end(), [&]( std::remove_reference < decltype(v) >::type::reference t ) {
                t /= sigma_inp[0];
            } );
        } );

        train_inp = new_train_input;
    }
    */
    train_inp = ( train_inp - mu_inp[ 0 ] ) / sigma_inp[ 0 ];

    /* Operator test
    const auto x1 = range( 1.0, 1.0, 5.0 );
    std::cerr << "x1: " << x1 << std::endl;
    const auto mutated_x1_minus = x1 - 2.0;
    std::cerr << "mutated_x1_minus: " << mutated_x1_minus << std::endl;
    const auto mutated_x1_plus = mutated_x1_minus + 2.0;
    std::cerr << "mutated_x1_plus: " << mutated_x1_plus << std::endl;
    const auto mutated_x1_mul = mutated_x1_plus * 2.0;
    std::cerr << "mutated_x1_mul: " << mutated_x1_mul << std::endl;
    const auto mutated_x1_div = mutated_x1_mul / 2.0;
    std::cerr << "mutated_x1_div: " << mutated_x1_div << std::endl;

    const auto y = merge( x1, x1 );
    std::cerr << "y: " << y << std::endl;
    const auto y_plus = y + 2.0;
    std::cerr << "y_plus: " << y_plus << std::endl;
    const auto y_minus = y_plus - 2.0;
    std::cerr << "y_minus: " << y_minus << std::endl;
    const auto y_mul = y_minus * 2.0;
    std::cerr << "y_mul: " << y_mul << std::endl;
    const auto y_div = y_minus / 2.0;
    std::cerr << "y_div: " << y_div << std::endl;


    const std::vector< double > x = { 2, 3, 4 };
    const auto one_m_x = 1.0 - x;
    std::cerr << "one_m_x: " << one_m_x << std::endl;
    const auto one_p_x = 1.0 + x;
    std::cerr << "one_p_x:" << one_p_x << std::endl;
    const auto two_mul_x1 = 2.0 * x;
    std::cerr << "two_mul_x1: " << two_mul_x1 << std::endl;

    const std::vector< double > bb = { 1, 2, 3, 4 };
    std::cerr << "bb: " << bb << std::endl;
    const auto bbt = vec_to_vecvec( bb );
    std::cerr << "bbt: " << bbt << std::endl;

    std::cerr << "y_div: " << y_div << std::endl;

    const auto y_div_t = trans( y_div );
    std::cerr << "y_div_t: " << y_div_t << std::endl;

    const auto y_div_t_2 = y_div_t * 2.0;
    std::cerr << "y_div_t_2: " << y_div_t_2 << std::endl;

    const auto dd = y_div_t_2 - y_div_t;
    std::cerr << "dd: " << dd << std::endl;

    const auto dd_1 = y_div_t_2 + y_div_t;
    std::cerr << "dd_1: " << dd_1 << std::endl;


    std::cerr << "y_div: " << y_div << std::endl;
    std::cerr << "y_div_t: " << y_div_t << std::endl;

    const auto y_d_d = y_div * y_div_t;
    std::cerr << "y_d_d: " << y_d_d << std::endl;

    const auto y_d_d_10 = feval( []( double & v ) { v *= 10.0; }, y_d_d );
    std::cerr << "y_d_d_10: " << y_d_d_10 << std::endl;

    std::cerr << "y_div: " << y_div << std::endl;
    const std::vector< double > va = { 1, 2, 3, 4, 5 };
    std::cerr << "va: " << va << std::endl;
    const auto va_m_y_div = va * y_div;
    std::cerr << "va_m_y_div: " << va_m_y_div << std::endl;

    exit( -1 );
    */

    const auto mu_out = mean( train_out );
    const auto sigma_out = stand( train_out );
    train_out = ( train_out - mu_out ) / sigma_out;

    const auto patterns = size( train_inp ).first;
    std::cerr << "patterns: " << patterns << std::endl;

    /* Ones test
    const auto b = ones( 5 );
    std::cerr << "b: " << b << std::endl;
    const auto bb = ones( 5, 5 );
    std::cerr << "bb: " << bb << std::endl;
     */

    /* Zeros test
    const auto z = zeros( 5 );
    std::cerr << "z: " << z << std::endl;
    const auto zz = zeros( 5, 5 );
    std::cerr << "zz: " << zz << std::endl;
    */

    auto bias = ones( patterns );
    train_inp = merge( train_inp, bias );
//    std::cerr << "train_inp: " << train_inp << std::endl;

    const auto inputs = size( train_inp ).second;
//    std::cerr << "inputs: " << inputs << std::endl;

    const auto earlystop = false;
    const auto reset = false;
    const auto hlr = 0.5;


    /* Inner prod test
    const std::vector< double > a = { 1, 2, 3 };
    std::cerr << "a: " << a << std::endl;

    const std::vector< double > b1 = { 1, 2, 3 };
    const std::vector< double > b2 = { 4, 5, 6 };
    const std::vector< double > b3 = { 6, 7, 8 };
    const auto b12 = merge( b1, b2 );
    const auto b = merge( b12, b3 );
    std::cerr << "b: " << b << std::endl;
    const auto ab = a * b;
    std::cerr << "ab: " << ab << std::endl;
    exit( -1 );
     */

    /* Power test
    std::vector< double > x = { 1, 2, 3, 4 };
    std::cerr << "x: " << x << std::endl;
    const auto xto2 = x ^ 2;
    std::cerr << "xto2: " << xto2 << std::endl;
    exit( -1 );
    */


//    weight_input_hidden = (randn(inputs,hidden_neurons) - 0.5)/10;
//    weight_hidden_output = (randn(1,hidden_neurons) - 0.5)/10;

    auto weight_input_hidden = ( randn( inputs, hidden_neurons) - 0.5 ) / 10.0;
    // TODO check weight_input_hidden
    //std::cerr << "weight_input_hidden.size: " << size( weight_input_hidden ) << std::endl;
    //std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
    auto weight_hidden_output = ( randn( hidden_neurons ) - 0.5 ) / 10.0;
    // TODO check weight_hidden_output
    //std::cerr << "weight_hidden_output.size: " << size( weight_hidden_output ) << std::endl;
    //std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;

    std::vector< long double > err(epochs);

    for( std::size_t i = 0; i < epochs; ++i ) {
        const auto alr = hlr;
//        std::cerr << "alr: " << alr << std::endl;
        const auto blr = alr / 10.0;
//        std::cerr << "blr: " << blr << std::endl;

        for( std::size_t j = 0; j < patterns; ++j ){
            const auto patnum = (static_cast<std::size_t>( round( randd() * patterns + 0.5 ) )-1) % patterns;
            // TODO process all patterns in random order

           //std::cerr << "patnum: " << patnum << std::endl;
            const auto this_pat = train_inp[ patnum ];
            //std::cerr << "this_pat: " << this_pat << std::endl;
            const auto act = train_out[ patnum ];
//            std::cerr << "act: " << act << std::endl;

            const auto xx = this_pat * weight_input_hidden;
            // TODO check xx
            //std::cerr << "xx: " << xx;
            //std::cerr << "size(xx): " << size( xx ) << std::endl;
            const auto tanhxx = feval( []( const long double & v ){ return std::tanh(v); }, xx );
            // TODO check tanhxx
            //std::cerr << "tanhxx: " << tanhxx ;
            //std::cerr << "size( tanhxx ): " << size( tanhxx ) << std::endl;
            const auto hval = tanhxx;
            //std::cerr << "hval: " << hval << std::endl;
            //std::cerr << "hval: " << hval;
            // TODO check hval
            const auto pred = hval * weight_hidden_output;
            //std::cerr << "pred: " << pred << std::endl;
            const auto error = pred - act;
            //std::cerr << "error: " << error << std::endl;
            const auto delta_HO = hval * error * blr;
            //std::cerr << "delta_HO: " << delta_HO << std::endl;
            weight_hidden_output = weight_hidden_output - delta_HO;
            //std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;
            //delta_IH= alr.*error.*weight_hidden_output'.*(1-(hval.^2))*this_pat
            const auto m1 = weight_hidden_output * alr * error;
            //std::cerr << "m1: " << m1 << std::endl;
            const auto m2 = 1.0 - (hval^2);
            //std::cerr << "m3: " << m3 << std::endl;
            const auto m3 = dot_operator( m1, m2, std::multiplies< long double >());
            //std::cerr << "m3: " << m3 << std::endl;
            const auto m4 = vec_to_vecvec( m3 );
            const auto delta_IH =  m4 * this_pat;
            //std::cerr << "delta_IH: " << delta_IH << std::endl;
            weight_input_hidden = weight_input_hidden - trans( delta_IH );
            //std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
//            pred = weight_hidden_output*tanh(train_inp*weight_input_hidden)';
//                error = pred' - train_out;
//                err(iter) =  (sum(error.^2))^0.5;
        }
        const auto p1 = feval( []( const long double& v ){ return std::tanh( v ); }, train_inp * weight_input_hidden );

        //std::cerr << "train_inp: " << train_inp << std::endl;
        //std::cerr << "p1: " << p1 << std::endl;
        const auto pred = weight_hidden_output * trans( p1 );
        //std::cerr << "size(pred): " << size(pred) << std::endl;
        //std::cerr << "pred: " << pred << std::endl;
        const auto error = pred - train_out;
        const auto error_sq = error ^ 2;
        err[ i ] = std::sqrt( std::accumulate( error_sq.cbegin(), error_sq.cend(), 0.0, std::plus<long double> () ) );
    }
    std::cerr << "weight_input_hidden: " << weight_input_hidden << std::endl;
    std::cerr << "weight_hidden_output: " << weight_hidden_output << std::endl;
    std::cerr << "err: " << err << std::endl;
    return 0;
}

