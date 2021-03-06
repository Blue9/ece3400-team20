`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144
`define SCREEN_SIZE 25344

///////* DON'T CHANGE THIS PART *///////
module DE0_NANO(
	CLOCK_50,
	GPIO_0_D,
	GPIO_1_D,
	KEY
);

//=======================================================
//  PARAMETER declarations
//=======================================================
localparam RED = 8'b111_000_00;
localparam GREEN = 8'b000_111_00;
localparam BLUE = 8'b000_000_11;

//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK - DON'T NEED TO CHANGE THIS //////////
input 		          		CLOCK_50;

//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
output 		    [33:0]		GPIO_0_D;
//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
input 		    [33:20]		GPIO_1_D;
input 		     [1:0]		KEY;

///// PIXEL DATA /////
reg [7:0]	pixel_data_RGB332 = 8'd0;

///// READ/WRITE ADDRESS /////
reg [14:0] X_ADDR;
reg [14:0] Y_ADDR;
reg [14:0] WRITE_ADDRESS;
reg [14:0] READ_ADDRESS; 

//assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);

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
wire [8:0] RESULT;

/* WRITE ENABLE */
reg W_EN;

wire c0_sig;
wire c1_sig;
wire c2_sig;

///////* CREATE ANY LOCAL WIRES YOU NEED FOR YOUR PLL *///////

///////* INSTANTIATE YOUR PLL HERE *///////

PLL	PLL_inst (
	.inclk0 ( CLOCK_50 ),
	.c0 ( c0_sig ),
	.c1 ( c1_sig ),
	.c2 ( c2_sig )
	);


///////* M9K Module *///////
Dual_Port_RAM_M9K mem(
	.input_data(pixel_data_RGB332),
	.w_addr(WRITE_ADDRESS),
	.r_addr(READ_ADDRESS),
	.w_en(W_EN),
	.clk_W(CLOCK_50),
	.clk_R(c1_sig), // DO WE NEED TO READ SLOWER THAN WRITE??
	.output_data(MEM_OUTPUT)
);

///////* VGA Module *///////
VGA_DRIVER driver (
	.RESET(VGA_RESET),
	.CLOCK(c1_sig),
	.PIXEL_COLOR_IN(VGA_READ_MEM_EN ? MEM_OUTPUT : BLUE),
	.PIXEL_X(VGA_PIXEL_X),
	.PIXEL_Y(VGA_PIXEL_Y),
	.PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
   .H_SYNC_NEG(GPIO_0_D[7]),
   .V_SYNC_NEG(VGA_VSYNC_NEG)
);

///////* Image Processor *///////
IMAGE_PROCESSOR proc(
	.PIXEL_IN(MEM_OUTPUT),
	.CLK(/*REPLACE THIS THE 25MHZ SIGNAL FROM YOUR PLL*/),
	.VGA_PIXEL_X(VGA_PIXEL_X),
	.VGA_PIXEL_Y(VGA_PIXEL_Y),
	.VGA_VSYNC_NEG(VGA_VSYNC_NEG),
	.RESULT(RESULT)
);


///////* Update Read Address *///////
always @ (VGA_PIXEL_X, VGA_PIXEL_Y) begin
		READ_ADDRESS = (VGA_PIXEL_X + VGA_PIXEL_Y*`SCREEN_WIDTH);
		if(VGA_PIXEL_X>(`SCREEN_WIDTH-1) || VGA_PIXEL_Y>(`SCREEN_HEIGHT-1))begin
				VGA_READ_MEM_EN = 1'b0;
		end
		else begin
				VGA_READ_MEM_EN = 1'b1;
		end
end

always @ (posedge c2_sig) begin
//	if (X_ADDR < 15'd20)begin
//		pixel_data_RGB332 = GREEN;
//	end
//	else begin
//		pixel_data_RGB332 = RED;
//	end
//	X_ADDR = X_ADDR + 15'b1;
////	pixel_data_RGB332 <= (X_ADDR < 15'd20) ? BLUE : RED;
//	X_ADDR <= (X_ADDR + 15'd1 >= `SCREEN_WIDTH) ? 15'd0 : (X_ADDR + 15'd1);
//	Y_ADDR <= (X_ADDR + 15'd1 >= `SCREEN_WIDTH) ? Y_ADDR + 15'd1 :
//					((Y_ADDR >= `SCREEN_HEIGHT) ? 15'd0 : Y_ADDR);	
//	if (X_ADDR >= `SCREEN_WIDTH) begin
//		X_ADDR = 15'b0;
//		Y_ADDR = Y_ADDR + 15'b1;
//	end
//	if (Y_ADDR >= `SCREEN_HEIGHT) begin
//		Y_ADDR = 15'b0;
//	end
	W_EN <= 1'd1;
   WRITE_ADDRESS <= (WRITE_ADDRESS < `SCREEN_SIZE) ? WRITE_ADDRESS + 1 : WRITE_ADDRESS;
	pixel_data_RGB332 <= (WRITE_ADDRESS < `SCREEN_SIZE / 2) ? RED : BLUE;
end

//always @ (X_ADDR, Y_ADDR) begin
//	pixel_data_RGB332 <= (X_ADDR < 15'd20) ? BLUE : RED;
//end

//always @ (negedge CLOCK_50) begin
//	W_EN = 1'b1;
//end


	
endmodule 