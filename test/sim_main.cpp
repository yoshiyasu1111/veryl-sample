#include <cstdint>
#include <iostream>
#include <verilated.h>
#include <verilated_fst_c.h>
#include "Vtop.h"
#include "Vtop___024root.h"

int main(int argc, char** argv) {
    auto contextp = std::make_unique<VerilatedContext>();
    contextp->commandArgs(argc, argv);

    auto top = std::make_unique<Vtop>(contextp.get(), "");

    // trace enable
    contextp->traceEverOn(true);
    // fst tracer
    auto tfp = std::make_unique<VerilatedFstC>();
    // hierarchy depth
    top->trace(tfp.get(), 99);
    // output filename
    tfp->open("wave.fst");

    top->i_rst = 0;
    for (uint32_t i = 0; i < 10; ++i) {
        top->i_clk = 0;
        top->eval();
        tfp->dump(contextp->time());
        contextp->timeInc(1);
        top->i_clk = 1;
        top->eval();
        tfp->dump(contextp->time());
        contextp->timeInc(1);
    }
    top->i_rst = 1;
    for (uint32_t cycle = 0; cycle < 100 && !contextp->gotFinish(); ++cycle) {
        top->i_clk = !top->i_clk;
        top->eval();
        tfp->dump(contextp->time());
        contextp->timeInc(1);
        std::cout << static_cast<int>(top->rootp->top__DOT__counter) << std::endl;
    }
    top->final();
    tfp->close();

    return 0;
}
