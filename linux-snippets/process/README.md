# 进程
## 进场介绍
进程的三种状态：
- 运行：正在执行或等待执行
- 停止：接收到 SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU 这四个信号之一而暂停执行。若接收到SIGCONT信号就会继续执行。
- 终结：进程被信号终结，或从`main()`退出，或是调用了`exit()`函数。

## 清理子进程
子进程终止时，并不会从系统中消失。父进程有义务清理子进程，否则子进程将成为僵尸进程，占据系统资源。
若父进程终止时，子进程还在运行，则这些子进程称为孤儿进程，这时系统的`init`进程(PID为1)将收养这些孤儿进程，并在它们终止时负责清理它们。
****
```C++
#include <sys/types.h>
#include <sys/wait.h>

// Returns: PID of child if OK, 0 (if WNOHANG), or -1 on error
pid_t waitpid(pid_t pid, int *statusp, int options);
```
清理子进程：
- `pid`为`0`：等待进程`pid`终止。
- `pid`为`-1`：等待所有子进程**之一**终止。
- `options`为`0`：阻塞等待。
- `options`为`WNOHANG`，非阻塞，若没有子进程终止，则返回`0`。
- `options`为`WUNTRACED`，阻塞等待进程终止或**停止**。
- `options`为`WCONTINUED`，阻塞等待进程终止，或等待停止的进程接收到`SIGCONT`信号。

获取进程退出状况：
- WIFEXITED(status). Returns true if the child terminated normally via a call to exit or a return.
- WEXITSTATUS(status). Returns the exit status of a normally terminated child. This status is only defined if WIFEXITED() returned true.
- WIFSIGNALED(status). Returns true if the child process terminated because of a signal that was not caught.
- WTERMSIG(status). Returns the number of the signal that caused the child process to terminate. This status is only defined if WIFSIQNALED() returned true.
- WIFSTOPPED(status). Returns true if the child that caused the return is currently stopped.
- WSTOPSIG(status). Return the number of the signal that caused the child to stop. This-status is on1y defined if WIFSTOPPED() return true.
- WIFCONTJNIJED(status). Returns true if the child process was restarted by receipt of a SIGCONT signal.

错误情况：
If the calling process has no children　or the process specified by pid does not exist, then waitpid returns -1 and sets errno to
ECHILD. If the wai tpid function was interrupted by a signal, then it returns -1
and sets errno to EINTR.


