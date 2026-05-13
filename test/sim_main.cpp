#include <cstdint>
#include <iostream>
#include <verilated.h>
#include "Vtop.h"
#include "Vtop___024root.h"

int main(int argc, char** argv) {
    auto contextp = std::make_unique<VerilatedContext>();
    contextp->commandArgs(argc, argv);

    auto top = std::make_unique<Vtop>(contextp.get(), "");

    top->i_rst = 0;
    for (uint32_t i = 0; i < 10; ++i) {
        top->i_clk = 0;
        top->eval();
        top->i_clk = 1;
        top->eval();
    }
    top->i_rst = 1;
    for (uint32_t cycle = 0; cycle < 100 && !contextp->gotFinish(); ++cycle) {
        top->i_clk = !top->i_clk;
        top->eval();
        contextp->timeInc(1);
        std::cout << static_cast<int>(top->rootp->top__DOT__counter) << std::endl;
    }
    top->final();

    return 0;
}
