
rtl := ./rtl/dut_top.sv ./rtl/dut_reg.sv ./rtl/clk_gate.sv
tb  := ./tb/tb_top.sv 
exec := run.x
opt := --binary --trace

run: compile
	./obj_dir/$(exec)

compile:
	verilator $(opt) +incdir+./tb $(tb) $(rtl) -o $(exec)

clean:
	\rm -rf obj_dir
	\rm -rf ./waves.vcd 

