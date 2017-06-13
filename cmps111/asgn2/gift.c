void 
sched_gift(struct proc *source, struct proc *target, uint64_t avg_source_ticket, uint64_t source_debt,  uint64_t avg_target_ticket, uint64_t bonus_tickets)
{
		
		uint64_t result;
		
		PROC_LOCK_ASSERT(source, MA_OWNED);
		
		//Time to subtract tickets from source
		FOREACH_THREAD_IN_PROC(source, td) {

			thread_lock(td);
			
			result =  td->tickets - (avg_source_ticket + source_debt);
			
			
			//gifting = 21
			
			//thd1 = 11
			//thd2 = 30
			
			//# threads = 2
			//average to give = 21 / 2 = 10.x
			
			//debt will be 21 % 2 = 1 tickets
			
			//double check: 20.x --> 20 
			//10 * 2 + debt = 21 AWESOME!
			
			//thd1 result = 11 - (10 + 1)  = 0
			
			//goes to if statement
			//debt = (10 + 1 + 1 - (0)) - 11		=	12 - 11 = 1;
			//new debt = 1 again
			
			//next thread
			// 30 - (10 + 1) = 19
			//since not neg, we can just store this as the value
			//thread2 = 19
			
			//thd2 + thd1 = 19 + 1 = 20 
			//we tried to give 
			//41 - 21 = 20 tickets, we all good!
						
			
			//if result is less than 1, add to current debt, set tickets to 1
			if (result < 1) {
				source_debt =  (avg_source_ticket + source_debt + 1 - result ) - td->tickets;
				td->tickets = 1;
			}
			else {
				source_debt = 0;
				td->tickets = result;
			}

			thread_unlock(td);

		}
		
		
		
		//Time to add tickets to target
		FOREACH_THREAD_IN_PROC(target, td) {
			
			//Checking subtracting from the source
			result = td->tickets + avg_target_tickets + bonus_tickets;

			//thread recieves over max tickets. Add extra tickets to bonus
			//set tickets in target to max.
			if (result > 100000) {
				bonus_tickets = (td->tickets + avg_target_tickets + bonus_tickets) - 100000;
				td->tickets = 100000;
			}
			
			//no more bonus tickets, zero it out
			else {
				bonus_tickets = 0;
				td->tickets = result;
			}
			
			thread_unlock(td);

		}
}

static int
dogift (struct thread *td, struct proc *source, struct proc *target, int t) {
	
	
	struct thread *knownthread;
	
	//source or target is NULL and can be found
	if ( (source == NULL) || (target == NULL)){
		//return failed
	}
		
	//checks if source is root
	sys_geteuid(td,0);		
	
	//trying to gift a root call, they dont have tickets!
	if (td->td_retval[0] == 0) {	
		//return failed
	}
	
	//find if target process is also root
	FOREACH_THREAD_IN_PROC(target,td) {
	thread_lock(td);
	
	knownthread = td;
	
	thread_unlock(td);
	break;
	}
	
	sys_getuid(knownthread, 0);
	
	//another check if target is root
	if (knownthread->knownthread_retval[0] == 0) {
	//return failed
	}
	
	else {
		
		//trying to steal tickets, no!
		if (t < 0) {
			//return availible tickets
		}
		
		uint64_t total_source_tickets = 0;
		uint64_t total_target_tickets = 0;
		
		uint64_t source_threads = 0;
		uint64_t target_threads = 0;
		
		PROC_LOCK_ASSERT(p, MA_OWNED);
		
		//count tickets and threads
		FOREACH_THREAD_IN_PROC(source, td) {
			thread_lock(td);
			total_source_tickets += td->tickets
			source_threads++;
			thread_unlock(td);
		}
		
		//count tickets and threads
		FOREACH_THREAD_IN_PROC(target, td) {
			thread_lock(td);
			total_target_tickets += td->tickets
			target_threads++;
			thread_unlock(td);
		}
		
		//check to see if source has enough tickets. At least 1 left per thread
		if (t > (total_source_tickets - source_threads)) {
			//return availible number of tickets
			return(t - total source tickets + sourcethreads);
		}
		
		//Trying to transfer too many tickets, target cant handle
		else if ( (int)((total_target_tickets + t) / target_threads )) > 100000) {
			//return availible number of tickets we can allocated
			return( (int)((100000 - (total target tickets / target threads)));
		}
		
		else {
			
			
			uint64_t source_debt = 0; 
			uint64_t bonus_tickets = 0; 

			
			//amount to add and pull
			avg_source_ticket = t / source_threads;
			source_debt = t % source_threads;
			
			avg_target_ticket = t / target_threads;
			bonus_tickets = t % target_threads;
			
			sched_gift(source, target, avg_source_ticket, source_debt, avg_target_ticket, bonus_tickets);
			
			return(0);
		}
		
	}
}