`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144

///////* DON'T CHANGE THIS PART *///////
module DE0_NANO(
	CLOCK_50,
	GPIO_0_D,
	GPIO_1_D,
	KEY,
	LED
);

//=======================================================
//  PARAMETER declarations
//=======================================================
localparam RED = 8'b111_000_00;
localparam GREEN = 8'b000_111_00;
localparam BLUE = 8'b000_000_11;
localparam BLACK = 8'b000_000_00;
localparam WHITE = 8'b111_111_11;
localparam YELLOW = 8'b111_111_00;
localparam PURPLE = 8'b111_000_11;
localparam LIGHTBLUE = 8'b000_010_11;
localparam BROWN = 8'b010_001_00;

//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK - DON'T NEED TO CHANGE THIS //////////
input 		          		CLOCK_50;

//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
output 		    [33:0]		GPIO_0_D;
//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
input 		    [33:0]		GPIO_1_D;
input 		     [1:0]		KEY;

// Debugging LEDs
output [7:0] LED;

///// PIXEL DATA /////
reg [7:0]	pixel_data_RGB332 = WHITE;

//The adapter takes pixel data in RGB 332 format (8 bits - 3 red, 3 green, 2 blue).
///// READ/WRITE ADDRESS /////
reg [14:0] X_ADDR;
reg [14:0] Y_ADDR;
wire [14:0] WRITE_ADDRESS;
reg [14:0] READ_ADDRESS; 

assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);

///// VGA INPUTS/OUTPUTS /////
wire 			VGA_RESET;
wire [7:0]	VGA_COLOR_IN;
wire [9:0]	VGA_PIXEL_X;
wire [9:0]	VGA_PIXEL_Y;
wire [7:0]	MEM_OUTPUT;
wire			VGA_VSYNC_NEG;
wire			VGA_HSYNC_NEG;
reg			VGA_READ_MEM_EN;

assign GPIO_0_D[5] = VGA_VSYNC_NEG;
assign VGA_RESET = ~KEY[0];

///// I/O for Img Proc /////
wire [3:0] RESULT;

/* WRITE ENABLE */
reg W_EN;
wire c0_sig;
wire c1_sig;
wire c2_sig;

// ALL CONNECTIONS HERE
wire [7:0] camera; 
//wire PCLK, HREF, VSYNC;
wire D0 = GPIO_1_D[32];
wire D1 = GPIO_1_D[25];
wire D2 = GPIO_1_D[30];
wire D3 = GPIO_1_D[27];
wire D4 = GPIO_1_D[28];
wire D5 = GPIO_1_D[29];
wire D6 = GPIO_1_D[26];
wire D7 = GPIO_1_D[31];
wire PCLK = GPIO_1_D[0];
wire HREF = GPIO_1_D[9];
wire VSYNC = GPIO_1_D[11];

//assign camera = {GPIO_1_D[33],GPIO_1_D[32],GPIO_1_D[31],GPIO_1_D[30],GPIO_1_D[29],GPIO_1_D[28],GPIO_1_D[27],GPIO_1_D[26]};
assign GPIO_0_D[0] = c0_sig; // Camera team needs the clock at pin number 2 on bank 1 



///////* CREATE ANY LOCAL WIRES YOU NEED FOR YOUR PLL *///////


///////* INSTANTIATE YOUR PLL HERE *///////
PLL	pll_inst (
	.inclk0 (CLOCK_50),
	.c0 ( c0_sig ),  //24 MHz
	.c1 ( c1_sig ), // 25 MhZ
	.c2 ( c2_sig ) // 50 MHz
	);

///////* M9K Module *///////
Dual_Port_RAM_M9K mem(
	.input_data(pixel_data_RGB332),
	.w_addr(WRITE_ADDRESS),
	.r_addr(READ_ADDRESS),
	.w_en(W_EN),
	.clk_W(CLOCK_50), // write frequency = 50 MHz
	.clk_R(c1_sig), // read frequency = 25 MHz
	.output_data(MEM_OUTPUT)
);



///////* VGA Module *///////
VGA_DRIVER driver (
	.RESET(VGA_RESET),
	.CLOCK(c1_sig),
	.PIXEL_COLOR_IN(VGA_READ_MEM_EN ? MEM_OUTPUT : GREEN), //(proc_res[1:0] == 2'b01 ? RED : (proc_res[1:0] == 2'b10 ? BLUE : GREEN))),
	.PIXEL_X(VGA_PIXEL_X),
	.PIXEL_Y(VGA_PIXEL_Y),
	.PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
   .H_SYNC_NEG(GPIO_0_D[7]),
   .V_SYNC_NEG(VGA_VSYNC_NEG)
);

wire [8:0] proc_res;

///////* Image Processor *///////
//IMAGE_PROCESSOR proc(
//	.PIXEL_IN(MEM_OUTPUT),
//	.CLK(c1_sig ),
//	.VGA_PIXEL_X(VGA_PIXEL_X),
//	.VGA_PIXEL_Y(VGA_PIXEL_Y),
//	.VGA_VSYNC_NEG(VGA_VSYNC_NEG),
//	.RESULT(proc_res)
//);
wire [7:0] shape;

IMAGE_PROCESSOR proc(
	.PIXEL_IN(MEM_OUTPUT),
	.CLK(c1_sig),
	.VGA_PIXEL_X(VGA_PIXEL_X),
	.VGA_PIXEL_Y(VGA_PIXEL_Y),
	.VSYNC(VSYNC),
	.RESULT(proc_res[1:0]),
	.SHAPE(shape),
	.HREF(HREF)
);

assign LED = shape;

///////* Update Read Address *///////
always @ (VGA_PIXEL_X, VGA_PIXEL_Y) begin
		READ_ADDRESS = (VGA_PIXEL_X + VGA_PIXEL_Y*`SCREEN_WIDTH);
		if(VGA_PIXEL_X>(`SCREEN_WIDTH-1) || VGA_PIXEL_Y>(`SCREEN_HEIGHT-1)) begin
				VGA_READ_MEM_EN = 1'b0;
		end
		else begin
				VGA_READ_MEM_EN = 1'b1;
		end
end

 ///// --- downsampler -----
reg prev_HREF = 1'b0;
reg prev_VSYNC = 1'b0;
reg [7:0] old_D = 8'd0;
reg toggle = 1'b0;
reg [11:0] vsync_counter_3 = 12'd0;
reg [11:0] vsync_counter_2 = 12'd0;
reg [11:0] vsync_counter_1 = 12'd0;
reg [1:0] vsync_hit = 1'b0;
reg [11:0] vsync_limit = 12'd25;

always @ (posedge PCLK) begin
		vsync_counter_3 = vsync_counter_3 + 1;
		if (vsync_counter_3 >= 1000) begin
			vsync_counter_2 = vsync_counter_2 + 1;
			vsync_counter_3 = 0;
		end
		if (vsync_counter_2 >= 1000 && vsync_counter_3 >= 1000) begin
			vsync_counter_1 = vsync_counter_1 + 1;
			vsync_counter_3 = 0;
			vsync_counter_2 = 0;
		end
		if (vsync_counter_1 >= vsync_limit) begin
			vsync_hit = 1'b0;
			vsync_counter_1 = 0;
			vsync_counter_2 = 0;
			vsync_counter_3 = 0;
		end

		if (VSYNC & !prev_VSYNC & vsync_hit <= 2) begin
			X_ADDR = 15'd0;
			Y_ADDR = 15'd1;
			vsync_hit = vsync_hit + 1;
			toggle = 1'b0;
		end
		else if (HREF & !prev_HREF) begin
			X_ADDR = 15'd0;
			Y_ADDR = Y_ADDR + 15'd1 >= `SCREEN_HEIGHT ? 15'd0 : Y_ADDR + 15'd1;
			toggle = 1'b0;
		end
		if (HREF & !toggle) begin
			W_EN = 1'b0;
			old_D = {D7, D6, D5, D4, D3, D2, D1, D0};
			toggle = 1'b1;
		end
		else if (HREF & toggle) begin
			W_EN = 1'b1;
			pixel_data_RGB332 = {D3&!old_D[3], D3&!old_D[3], D3&!old_D[3], old_D[7]|old_D[6], old_D[7]|old_D[6], old_D[7]|old_D[6], old_D[3]&!D3, old_D[3]&!D3}; // If RGB 444
			old_D = 8'd0;
			X_ADDR = X_ADDR + 15'd1;
			toggle = 1'b0;
		end
		prev_VSYNC = VSYNC;
		prev_HREF = HREF;
end



/////////* Update Write Address *///////
//always @ (posedge c2_sig) begin
//		W_EN = 1'b0;
//		Y_ADDR = (X_ADDR + 1 >= `SCREEN_WIDTH) ?
//			((Y_ADDR + 1 >= `SCREEN_HEIGHT) ?
//				15'd0 : Y_ADDR + 1) : Y_ADDR;
//		X_ADDR = (X_ADDR + 1 >= `SCREEN_WIDTH) ? 15'd0 : (X_ADDR + 1);
//		
//		pixel_data_RGB332 = (X_ADDR >= 25) ? RED : BLUE;
////		pixel_data_RGB332 = (X_ADDR < 25) ? RED :
////										((X_ADDR < 50) ? YELLOW :
////											((X_ADDR < 75) ? GREEN : 
////												((X_ADDR < 100) ? BLUE :
////													((X_ADDR < 125) ? PURPLE :
////														((X_ADDR < 150) ? BROWN :
////															BLACK)))));
//		W_EN = 1'b1;
//end



	
endmodule 
