
#include "tinyos.h"
#include "kernel_sched.h"
#include "kernel_proc.h"

/** 
  @brief Create a new thread in the current process.
  */
Tid_t sys_CreateThread(Task task, int argl, void* args)
{
 /*  Implementation
  
   TCB* tcb  = spawn_thread(CURPROC, start_main_thread_multithread);
   PTCB *ptcb = xmalloc(sizeof(PTCB));


	   tcb->ptcb = ptcb;
	   ptcb->tcb = tcb
	
	 *
	 *Initialization of PTCB 
	 *
	 ptcb->task = task; 
	 ptcb->argl = argl;
	 ptcb->args = args;
	 //ptcb->exitval = 0;
	 ptcb->exited = 0;
	 ptcb->detached = 0;
	 ptcb->exit_cv = COND_INIT;
    rlnode_init(&ptcb->ptcb_list_node, ptcb); 
    rlist_push_back(&CURPROC->ptcb_list, &ptcb->ptcb_list_node);
    CURPROC->thread_count = CURPROC->thread_count + 1;
    wakeup(tcb);

	return (Tid_t)ptcb;

*/

	  return NOTHREAD;
}

/**
  @brief Return the Tid of the current thread.
 */
Tid_t sys_ThreadSelf()
{
	return (Tid_t) cur_thread()->ptcb;
}

/**
  @brief Join the given thread.
  */
int sys_ThreadJoin(Tid_t tid, int* exitval)
{
	/* Implementation
	 	
	  Conditions for successful Thread Joining

	  1. Both threads should belong in the same process - they should belong in the same ptcb_list in the process
	  rlist_find(CURPROC->ptcb_list,(PTCB*)tid,NULL);

	  2. The threads which calls Thread_Join() cannot join itself
		if (cur_thread() == (PTCB*)tid);	

	  3. The thread we want to join should NOT be detached
		if ( (PTCB*)tid->detached == 1);
		
	  4. The thread we want to join should NOT be exited
	  	if ( (PTCB*)tid->exited == 1);

		ref_count ++; -- Because Thread_Join() was successful

    rlist_push_back(&ptcb->exit_cv, &ptcb->ptcb_list_node);

		kernel_wait();
		
	  5. We should check if the thread we wanna join is exited or not

		while(!((PTCB*)tid) ->exited == 1 || (PTCB*)tid)->detached == 1)
	 
		kernel_broadcast(&ptcb->exit_cv);
		ref_count --; --
	 */
	return -1;
}

/**
  @brief Detach the given thread.
  */
int sys_ThreadDetach(Tid_t tid)
{
	return -1;
}

/**
  @brief Terminate the current thread.
  */
void sys_ThreadExit(int exitval)
{


    /* Reparent any children of the exiting process to the 
       initial task */
    PCB* initpcb = get_pcb(1);
    while(!is_rlist_empty(& curproc->children_list)) {
      rlnode* child = rlist_pop_front(& curproc->children_list);
      child->pcb->parent = initpcb;
      rlist_push_front(& initpcb->children_list, child);
    }

    /* Add exited children to the initial task's exited list 
       and signal the initial task */
    if(!is_rlist_empty(& curproc->exited_list)) {
      rlist_append(& initpcb->exited_list, &curproc->exited_list);
      kernel_broadcast(& initpcb->child_exit);
    }

    /* Put me into my parent's exited list */
    rlist_push_front(& curproc->parent->exited_list, &curproc->exited_node);
    kernel_broadcast(& curproc->parent->child_exit);

  }

  assert(is_rlist_empty(& curproc->children_list));
  assert(is_rlist_empty(& curproc->exited_list));


  /* 
    Do all the other cleanup we want here, close files etc. 
   */

  /* Release the args data */
  if(curproc->args) {
    free(curproc->args);
    curproc->args = NULL;
  }

  /* Clean up FIDT */
  for(int i=0;i<MAX_FILEID;i++) {
    if(curproc->FIDT[i] != NULL) {
      FCB_decref(curproc->FIDT[i]);
      curproc->FIDT[i] = NULL;
    }
  }

  /* Disconnect my main_thread */
  curproc->main_thread = NULL;

  /* Now, mark the process as exited. */
  curproc->pstate = ZOMBIE;

  /* Bye-bye cruel world */
  kernel_sleep(EXITED, SCHED_USER);




}

