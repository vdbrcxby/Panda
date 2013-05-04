#include <syscall.h>
#include <asm.h>
#include <task.h>
#include <string.h>
#include <screen.h>
#include <exec.h>
#include <sysfile.h>

typedef int (*sysc_func) (struct registers_t* r);
extern void stub_ret(void);
static sysc_func sys_routines[NSYSC];

int errno = 0;

int nosys(struct registers_t* r) {
    return -1;
}

int sys_exec(struct registers_t* regs) {
    char *path = (char*)regs->ebx;
    char **argv = (char**)regs->ecx;
    return do_exec(path, argv);
}

int sys_fork(struct registers_t* regs) {
    struct task* t;
    struct registers_t* r = 0;
    t = spawn(&stub_ret);
    r = (struct registers_t*)((u32)t + PAGE_SIZE) - 1;
    *r = *regs;
    r->eax = 0;  //child return this 0
    t->p_context.esp = (u32)r;
    t->p_trap = r;
    t->stat = RUNNABLE;
    return t->pid;
}

int sys_write(struct registers_t* regs) {
    char v = regs->ebx;
    putch(v);
    return 1;
}

int sys_read(struct registers_t* regs) {
    char* buf = (char*)regs->ecx;
    u32 fd    = regs->ebx;
    u32 cnt   = regs->edx;
    if(vm_verify((u32)buf, cnt) < 0) {
        return -1;
    }
    return do_read(fd, buf, cnt);
}

void do_syscall(struct registers_t* regs){
    int ret;
    sysc_func func = 0;

    if (regs->eax > NSYSC) {
        PANIC("bad syscall");
    }
    current_task->p_error = 0;
    func = sys_routines[regs->eax];
    if (func == NULL)
        func = &nosys;
    ret = (*func)(regs);
    //
    if(ret < 0 )
        regs->eax = -current_task->p_error;
    else
        regs->eax = ret;
}

void syscall_init() {
    puts("syscall_init ... \n");
    irq_install_handler(0x80, (isq_t)(&do_syscall));
    sys_routines[NR_setup] = &nosys;
    sys_routines[NR_fork]  = &sys_fork;
    sys_routines[NR_exec]  = &sys_exec;
    sys_routines[NR_write] = &sys_write;
    sys_routines[NR_read]  = &sys_read;
}
