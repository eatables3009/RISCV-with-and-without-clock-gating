#include <systemc.h>
#include <iostream>
#include <cstring>
#include <cstdint>

enum { MEM_WORDS=256, RF_SIZE=32 };

// ================= PIPELINE STRUCTS =================
struct IF_ID  { uint32_t pc=0,inst=0; bool v=false; };
struct ID_EX  { uint32_t pc=0,a=0,b=0,imm=0; uint8_t rs1=0,rs2=0,rd=0; bool v=false; };
struct EX_MEM { uint32_t pc=0,alu=0; uint8_t rd=0; bool v=false; };
struct MEM_WB { uint32_t alu=0; uint8_t rd=0; bool v=false; };

// ================= CPU =================
SC_MODULE(CPU){
    sc_in<bool> clk, rst;

    uint32_t imem[MEM_WORDS];
    uint32_t rf[RF_SIZE];

    IF_ID if_id;
    ID_EX id_ex;
    EX_MEM ex_mem;
    MEM_WB mem_wb;

    uint32_t pc = 0;

    // Signals for waveform
    sc_signal<uint32_t> sig_pc;
    sc_signal<bool> sig_if, sig_id, sig_ex, sig_mem;

    // ================= ALU =================
    uint32_t alu(uint32_t a, uint32_t b){
        return a + b;
    }

    // ================= PROGRAM =================
    void load_program(){
        memset(imem,0,sizeof imem);

        imem[0] = 0x00200093; // addi x1,x0,2
        imem[1] = 0x00300113; // addi x2,x0,3
        imem[2] = 0x002081B3; // add x3,x1,x2
        imem[3] = 0x00418213; // addi x4,x3,4
        imem[4] = 0x002202B3; // add x5,x4,x2
        imem[5] = 0x00000000; // stop
    }

    // ================= PIPELINE =================
    void tick(){
        if(rst.read()){
            pc = 0;
            memset(rf,0,sizeof rf);
            if_id = {}; id_ex = {}; ex_mem = {}; mem_wb = {};
            return;
        }

        // ===== CLOCK GATING ENABLES =====
        bool en_if  = true;
        bool en_id  = if_id.v;
        bool en_ex  = id_ex.v;
        bool en_mem = ex_mem.v;
        bool en_wb  = mem_wb.v;

        // ================= WB =================
        if(en_wb && mem_wb.rd){
            rf[mem_wb.rd] = mem_wb.alu;
        }

        // ================= EX =================
        EX_MEM ex_mem_n = {};
        if(en_ex){
            uint32_t res = alu(id_ex.a, id_ex.b);
            ex_mem_n = {id_ex.pc, res, id_ex.rd, true};
        }

        // ================= MEM =================
        MEM_WB mem_wb_n = {};
        if(en_mem){
            mem_wb_n = {ex_mem.alu, ex_mem.rd, true};
        }

        // ================= ID =================
        ID_EX id_ex_n = {};
        if(en_id){
            uint32_t inst = if_id.inst;

            uint8_t opcode = inst & 0x7F;
            uint8_t rs1 = (inst >> 15) & 0x1F;
            uint8_t rs2 = (inst >> 20) & 0x1F;
            uint8_t rd  = (inst >> 7)  & 0x1F;
            int32_t imm = (int32_t)inst >> 20;

            uint32_t a = rf[rs1];
            uint32_t b = (opcode == 0x13) ? imm : rf[rs2]; // ADDI vs ADD

            id_ex_n = {if_id.pc, a, b,(uint32_t)imm, rs1, rs2, rd, true};
        }

        // ================= IF =================
        IF_ID if_id_n = {};
        if(en_if){
            uint32_t instr = imem[pc >> 2];

            if(instr != 0){
                if_id_n = {pc, instr, true};
                pc += 4;
            }
        }

        // ================= UPDATE =================
        if_id = if_id_n;
        id_ex = id_ex_n;
        ex_mem = ex_mem_n;
        mem_wb = mem_wb_n;

        // ================= SIGNALS =================
        sig_pc.write(pc);
        sig_if.write(if_id.v);
        sig_id.write(id_ex.v);
        sig_ex.write(ex_mem.v);
        sig_mem.write(mem_wb.v);

        // ================= DEBUG =================
        std::cout << "PC=" << pc
                  << " IF=" << if_id.v
                  << " ID=" << id_ex.v
                  << " EX=" << ex_mem.v
                  << " MEM=" << mem_wb.v
                  << std::endl;

        // ================= STOP =================
        if(!if_id.v && !id_ex.v && !ex_mem.v && !mem_wb.v){
            sc_stop();
        }
    }

    SC_CTOR(CPU){
        load_program();
        SC_METHOD(tick);
        sensitive << clk.pos();
    }
};

// ================= TESTBENCH =================
SC_MODULE(TB){
    sc_signal<bool> clk, rst;
    CPU cpu;

    void run(){
        rst = 1;
        clk = 0;
        wait(1, SC_NS);

        rst = 0;

        for(int i=0;i<50;i++){
            clk = 1; wait(1, SC_NS);
            clk = 0; wait(1, SC_NS);
        }

        sc_stop();
    }

    SC_CTOR(TB): cpu("cpu"){
        cpu.clk(clk);
        cpu.rst(rst);
        SC_THREAD(run);
    }
};

// ================= MAIN =================
int sc_main(int argc, char* argv[]){
    TB tb("tb");

    sc_trace_file *tf = sc_create_vcd_trace_file("gated");
    tf->set_time_unit(1, SC_NS);

    sc_trace(tf, tb.cpu.sig_pc, "PC");
    sc_trace(tf, tb.cpu.sig_if, "IF");
    sc_trace(tf, tb.cpu.sig_id, "ID");
    sc_trace(tf, tb.cpu.sig_ex, "EX");
    sc_trace(tf, tb.cpu.sig_mem, "MEM");
    sc_trace(tf, tb.clk, "clk");
    sc_trace(tf, tb.rst, "rst");

    sc_start();

    sc_close_vcd_trace_file(tf);

    return 0;
}
