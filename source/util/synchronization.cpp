#include "boxedwine.h"
#ifdef BOXEDWINE_MULTI_THREADED
#include <SDL.h>
BOXEDWINE_CONDITION waitPidCond;

BoxedWineCriticalSection::BoxedWineCriticalSection(BoxedWineMutex* mutex) {
    this->mutex = mutex;
    this->mutex->lock();
}
BoxedWineCriticalSection::~BoxedWineCriticalSection() {
    this->mutex->unlock();
}

BoxedWineCriticalSectionCond::BoxedWineCriticalSectionCond(BoxedWineCondition* cond) {
    this->cond = cond;
    this->cond->lock();
}
BoxedWineCriticalSectionCond::~BoxedWineCriticalSectionCond() {
    this->cond->unlock();
}

BoxedWineMutex::BoxedWineMutex() {
    this->m = SDL_CreateMutex();
}

BoxedWineMutex::~BoxedWineMutex() {
    SDL_DestroyMutex((SDL_mutex*)this->m);
}

void BoxedWineMutex::lock() {
    SDL_LockMutex((SDL_mutex*)this->m);
}

void BoxedWineMutex::unlock() {
    SDL_UnlockMutex((SDL_mutex*)this->m);
}

BoxedWineCondition::BoxedWineCondition() {
    this->m = SDL_CreateMutex();
    this->c = SDL_CreateCond();
}

BoxedWineCondition::~BoxedWineCondition() {
    SDL_DestroyMutex((SDL_mutex*)this->m);
    SDL_DestroyCond((SDL_cond*)this->c);
}

void BoxedWineCondition::lock() {
    SDL_LockMutex((SDL_mutex*)this->m);
}
 
void BoxedWineCondition::signal() {
    SDL_CondSignal((SDL_cond*)this->c);
}

void BoxedWineCondition::signalAll() {
    SDL_CondBroadcast((SDL_cond*)this->c);
}

void BoxedWineCondition::wait() {
    SDL_CondWait((SDL_cond*)this->c, (SDL_mutex*)this->m);
}

void BoxedWineCondition::waitWithTimeout(U32 ms) {
    SDL_CondWaitTimeout((SDL_cond*)this->c, (SDL_mutex*)this->m, ms);
}

void BoxedWineCondition::unlock() {
    SDL_UnlockMutex((SDL_mutex*)this->m);
}

#else 

bool BoxedWineConditionTimer::run() {
    this->cond->signalThread(true);
    return false; // signal will remove timer
}

BoxedWineCondition::BoxedWineCondition(std::string name) : name(name), parent(NULL) {
}

BoxedWineCondition::~BoxedWineCondition() {
}

void BoxedWineCondition::signalThread(bool all) {
    while (this->waitingThreads.size()) {
        KThread* thread = this->waitingThreads.front()->data;
        this->waitingThreads.front()->remove();
#ifdef _DEBUG
        if (!thread->waitingCond) {
            kpanic("shouldn't signal a thread that is not waiting");
        }
#endif
        thread->waitingCond = NULL;
        if (thread->condTimer.active) {
            removeTimer(&thread->condTimer);
            thread->condTimer.cond = NULL;
        }
        scheduleThread(thread);
        if (!all) {
            break;
        }
    }
    for (auto &child : this->children) {
        child->parent = NULL;
    }
    this->children.clear();
}

void BoxedWineCondition::signal() {
    if (this->parent) {
        this->parent->signalAll();
    }
    this->signalThread(false);
}

void BoxedWineCondition::signalAll() {
    if (this->parent) {
        this->parent->signalAll();
    }
    this->signalThread(true);
}

U32 BoxedWineCondition::wait() {
    this->waitingThreads.addToBack(&KThread::currentThread()->waitThreadNode);
    KThread::currentThread()->waitingCond = this;
    unscheduleThread(KThread::currentThread());
    return -K_WAIT;
}

U32 BoxedWineCondition::waitWithTimeout(U32 ms) {
    KThread* thread = KThread::currentThread();
    thread->condTimer.millies = ms+getMilliesSinceStart();
    thread->condTimer.cond = this;
    addTimer(&thread->condTimer);
    this->waitingThreads.addToBack(&KThread::currentThread()->waitThreadNode);
    KThread::currentThread()->waitingCond = this;
    unscheduleThread(KThread::currentThread());
    return -K_WAIT;    
}
 
U32 BoxedWineCondition::waitCount() {
    return this->waitingThreads.size();
}

void BoxedWineCondition::addChildCondition(BoxedWineCondition& cond) {
    cond.parent = this;
    this->children.push_back(&cond);
}

#endif