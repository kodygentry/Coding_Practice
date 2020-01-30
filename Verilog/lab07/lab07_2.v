module lab07_2(
    input clk,
    input rst,
    input shift,
    input split,
    output [3:0] vgaRed,
    output [3:0] vgaGreen,
    output [3:0] vgaBlue,
    output hsync,
    output vsync
    );

    wire [11:0] data;
    wire clk_25MHz;
    wire clk_22;
    wire [16:0] pixel_addr;
    wire [11:0] pixel;
    wire valid;
    wire [9:0] h_cnt; //640
    wire [9:0] v_cnt;  //480
    wire show_black;

    assign {vgaRed, vgaGreen, vgaBlue} = (valid==1'b1 && show_black==1'b0) ? pixel:12'h0;

    clock_divisor clk_wiz_0_inst(
        .clk(clk),
        .clk1(clk_25MHz),
        .clk22(clk_22)
    );

    mem_addr_gen_2 mem_addr_gen_inst(
        .clk(clk_22),
        .rst(rst),
        .shift(shift),
        .split(split),
        .h_cnt(h_cnt),
        .v_cnt(v_cnt),
        .pixel_addr(pixel_addr),
        .show_black(show_black)
    );
     
 
    blk_mem_gen_0 blk_mem_gen_0_inst(
        .clka(clk_25MHz),
        .wea(0),
        .addra(pixel_addr),
        .dina(data[11:0]),
        .douta(pixel)
    ); 

    vga_controller   vga_inst(
        .pclk(clk_25MHz),
        .reset(rst),
        .hsync(hsync),
        .vsync(vsync),
        .valid(valid),
        .h_cnt(h_cnt),
        .v_cnt(v_cnt)
    );
      
endmodule
