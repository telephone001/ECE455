# include <iostream>
# include <queue>
# include <thread>
# include <mutex>
# include <condition_variable>


constexpr int MAX_ITEMS = 10;
std :: queue <int > q ;
std :: mutex m ;
std :: condition_variable cv ; //schedules all the executions for us
bool done = false ;

//Condition variables

void producer () {
	for (int i = 0; i < 100; ++ i ) {
		std :: unique_lock < std :: mutex > lk ( m ) ;
		
                //wait until this condition is true then proceed
                //lk is the lock the condition variable is associated with.
                //cv needs a lock but it doesn't care if it is locked or unlocked
                cv . wait ( lk, [] { return (int ) q . size () < MAX_ITEMS ; }) ;
		
                q . push ( i ) ;
		
                std :: cout << " Produced : " << i << "\n";
		
                lk . unlock () ;
		
                //
                cv . notify_all () ;
	}

	{
                //don't want multiple flags setting the done condition to true
		std :: lock_guard < std :: mutex > lk ( m ) ;
		done = true ;
	}

	cv . notify_all () ;
}


void consumer () {
	while ( true ) {
		std :: unique_lock < std :: mutex > lk ( m ) ;
		cv . wait ( lk, [] { return ! q . empty () || done ; }) ;
		
                //producer has finished all its jobs.
                if ( q . empty () && done ) break ;
		
                int item = q . front () ;
		q . pop () ;
		lk . unlock () ;
		std :: cout << " Consumed : " << item << "\n";
		cv . notify_all () ;
	}
}


int main () {
	std :: thread p ( producer ) ;
	std :: thread c ( consumer ) ;

	p . join () ;
	c . join () ;
	
        return 0;
}