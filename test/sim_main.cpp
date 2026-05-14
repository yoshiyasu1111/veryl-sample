#include <cstdint>
#include <iostream>
#include <verilated.h>
#include <verilated_fst_c.h>
#include "Vtop.h"
#include "Vtop___024root.h"

void tick(Vtop* top, VerilatedContext* contextp, VerilatedFstC* tfp) {
    top->i_clk = 0;
    top->eval();
    tfp->dump(contextp->time());
    contextp->timeInc(1);

    top->i_clk = 1;
    top->eval();
    tfp->dump(contextp->time());
    contextp->timeInc(1);
}

void reset(Vtop* top, VerilatedContext* contextp, VerilatedFstC* tfp, uint32_t cycles) {
    top->i_rst = 0;

    for (uint32_t i = 0; i < cycles; ++i) {
        tick(top, contextp, tfp);
    }

    top->i_rst = 1;
}

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
    top->i_clk = 0;

    reset(top.get(), contextp.get(), tfp.get(), 10);

    for (uint32_t cycle = 0; cycle < 100 && !contextp->gotFinish(); ++cycle) {
        tick(top.get(), contextp.get(), tfp.get());
        std::cout << static_cast<int>(top->rootp->top__DOT__counter) << std::endl;
    }
    top->final();
    tfp->close();

    return 0;
}
