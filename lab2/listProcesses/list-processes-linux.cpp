#include <iostream>

#include <sys/ptrace.h>

int main()
{
    auto val = ptrace(PTRACE_PEEKDATA, 0, 0);
}

