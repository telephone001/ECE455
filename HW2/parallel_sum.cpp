# include < iostream >
# include <vector >
# include <thread >
# include < numeric >
# include < random >
# include < chrono > //timing library to measure timing


void partial_sum ( 
        const std :: vector <int > & data , 
        size_t start , 
        size_t end , 
        long long & out 
) {
        out = std :: accumulate ( data . begin () + start , data . begin () + end , 0LL ) ;
}


int main () {
        //Create a large array of 107
        //ints, split into T segments, and sum in parallel. Combine partial
        //sums and compare timing with a single-threaded baselin
        const size_t N = 10000000;


        const int T = std :: thread :: hardware_concurrency () ?
                      std :: thread :: hardware_concurrency () : 4; //a safe number for threads

        //generate a random number
        std :: vector <int > data ( N ) ;
        std :: mt19937 rng (42) ;
        std :: uniform_int_distribution <int > dist (1, 100) ;
        for ( auto & x : data ) x = dist ( rng ) ;

        // Baseline (single - threaded )
        auto t0 = std :: chrono :: high_resolution_clock :: now () ;
        long long baseline = std :: accumulate ( data . begin () , data . end () , 0LL ) ;
        auto t1 = std :: chrono :: high_resolution_clock :: now () ;
        
        // Parallel
        std :: vector < long long > partials (T , 0) ;  //there are T partial sums
        std :: vector < std :: thread > threads ; 
        threads . reserve ( T ) ; //so there are T threads allocated
        size_t chunk = N / T ; //how many sums each thread will have to do
        
        auto p0 = std :: chrono :: high_resolution_clock :: now () ;
        for (int i = 0; i < T ; ++ i ) {
                size_t s = i * chunk ;
                size_t e = ( i == T - 1) ? N : s + chunk ;
        
                threads . emplace_back ( partial_sum , std :: cref ( data ) , s , e , std :: ref (partials [ i ]) ) ;
        }

        // you have to join all the threads
        for ( auto & th : threads ) th . join () ;
        
        long long total = std :: accumulate ( partials . begin () , partials . end () , 0LL );
        auto p1 = std :: chrono :: high_resolution_clock :: now () ;
        
        //calculate the time it takes for the sequential and threaded thing
        std :: chrono :: duration <double > t_base = t1 - t0;
        std :: chrono :: duration <double > t_par = p1 - p0;
        
        
        std :: cout << " Baseline sum: " << baseline
        << " Time : " << t_base . count () << " s\n";
        
        std :: cout << " Parallel sum: " << total
        << " Time : " << t_par . count () << " s\n";
        
        return 0;
}
